/* ImHangul - Gtk+ 2.0 Input Method Module for Hangul
 * Copyright (C) 2002,2003,2004 Choe Hwanjin
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "gettext.h"
#include "gtkimcontexthangul.h"

enum {
  INPUT_MODE_DIRECT,
  INPUT_MODE_HANGUL,
  INPUT_MODE_HANJA
} IMHangulInputMode;

enum {
  OUTPUT_MODE_SYLLABLE = 0,
  OUTPUT_MODE_JAMO     = 1 << 1,
  OUTPUT_MODE_JAMO_EXT = 1 << 2
} IMHangulOutputMode;

enum {
  INPUT_MODE_INFO_NONE,
  INPUT_MODE_INFO_ENGLISH,
  INPUT_MODE_INFO_HANGUL
} IMHangulInputModeInfo;

typedef struct _CandidateItem CandidateItem;
typedef struct _StatusWindow  StatusWindow;

struct _Toplevel
{
  int input_mode;
  GtkWidget *widget;
  GtkWidget *status;
  GSList *contexts;
  guint destroy_handler_id;
  guint configure_handler_id;
};

/* Candidate window */
struct _Candidate {
  GtkIMContextHangul *hangul_context;
  GtkWidget *window;
  GdkWindow *parent;
  GdkRectangle cursor;
  gchar *label;
  GtkListStore *store;
  GtkWidget *treeview;
  const CandidateItem *data;
  int first;
  int n;
  int n_per_page;
  int current;
};

struct _CandidateItem {
    gunichar ch;
    gchar *comment;
};

#include "candidatetable.h"

static Candidate*  candidate_new             (char *label_str,
					      int n_per_page,
					      const CandidateItem *data,
					      GdkWindow *parent,
					      GdkRectangle *area,
					      GtkIMContextHangul *hcontext);
static void        candidate_prev            (Candidate *candidate);
static void        candidate_next            (Candidate *candidate);
static void        candidate_prev_page       (Candidate *candidate);
static void        candidate_next_page       (Candidate *candidate);
static gunichar    candidate_get_current     (Candidate *candidate);
static gunichar    candidate_get_nth         (Candidate *candidate, int index);
static void        candidate_delete          (Candidate *candidate);

static void	im_hangul_class_init	     (GtkIMContextHangulClass *klass);
static void	im_hangul_ic_init		     (GtkIMContextHangul *hcontext);
static void	im_hangul_ic_finalize	     (GObject *obj);

static void	im_hangul_ic_reset           (GtkIMContext *context);
static gboolean	im_hangul_ic_slave_filter_keypress (GtkIMContext *context,
					      GdkEventKey  *key);
static gboolean	im_hangul_ic_filter_keypress (GtkIMContext *context,
					      GdkEventKey  *key);

static void	im_hangul_get_preedit_string (GtkIMContext  *ic,
					      gchar	    **str,
					      PangoAttrList **attrs,
					      gint	    *cursor_pos);

static void	im_hangul_ic_focus_in	     (GtkIMContext *context);
static void	im_hangul_ic_focus_out	     (GtkIMContext *context);
static void	im_hangul_ic_set_client_window  (GtkIMContext *context,
					      GdkWindow    *client_window);
static void	im_hangul_ic_set_use_preedit (GtkIMContext *context,
    					      gboolean     use_preedit);
static void	im_hangul_ic_cursor_location (GtkIMContext *context,
    					      GdkRectangle *area);

/* asistant function for hangul composer */
static inline gboolean im_hangul_is_modifier  (guint state);
static inline gboolean im_hangul_is_choseong  (gunichar ch);
static inline gboolean im_hangul_is_jungseong (gunichar ch);
static inline gboolean im_hangul_is_jongseong (gunichar ch);
static inline gboolean im_hangul_ic_is_empty     (GtkIMContextHangul *hcontext);

static inline gboolean im_hangul_is_trigger   (GdkEventKey *key);
static inline gboolean im_hangul_is_backspace (GdkEventKey *key);
static inline void     im_hangul_ic_emit_preedit_changed (GtkIMContextHangul *hcontext);

static gboolean im_hangul_ic_composer_2      (GtkIMContextHangul *hcontext,
					      GdkEventKey *key);
static gboolean im_hangul_ic_composer_3      (GtkIMContextHangul *hcontext,
					      GdkEventKey *key);

/* stack functions */
static gunichar	im_hangul_ic_pop             (GtkIMContextHangul *hcontext);
static gunichar	im_hangul_ic_peek            (GtkIMContextHangul *hcontext);
static void	im_hangul_ic_push            (GtkIMContextHangul *hcontext,
					      gunichar		 ch);

/* commit functions */
static void	im_hangul_ic_clear_buf       (GtkIMContextHangul *hcontext);
static gboolean	im_hangul_ic_commit	     (GtkIMContextHangul *hcontext);
static void	im_hangul_ic_commit_unicode  (GtkIMContextHangul *hcontext,
					      gunichar ch);
static void     im_hangul_ic_commit_by_slave (GtkIMContext *context,
					      gchar *str, gpointer data);
static gboolean im_hangul_ic_process_nonhangul(GtkIMContextHangul *hcontext,
					      GdkEventKey *key);

/* for feedback (preedit attribute) */
static void	im_hangul_preedit_underline  (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_reverse    (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_shade      (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_foreground (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_background (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_color      (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);
static void	im_hangul_preedit_normal     (GtkIMContextHangul *hic,
					      PangoAttrList **attrs,
					      gint start, gint end);

static void     im_hangul_ic_show_status_window     (GtkIMContextHangul *hcontext);
static void     im_hangul_ic_hide_status_window     (GtkIMContextHangul *hcontext);
static int      im_hangul_ic_get_toplevel_input_mode(GtkIMContextHangul *hcontext);
static void     im_hangul_ic_set_toplevel_input_mode(GtkIMContextHangul *hcontext,
						  int mode);

static Toplevel*  toplevel_new(GtkWidget *toplevel_widget);
static Toplevel*  toplevel_get(GdkWindow *window);
static void       toplevel_append_context(Toplevel *toplevel,
					  GtkIMContextHangul *context);
static void       toplevel_remove_context(Toplevel *toplevel,
					  GtkIMContextHangul *context);
static void       toplevel_delete(Toplevel *toplevel);
static GtkWidget* status_window_new(GtkWidget *parent);

static void popup_candidate_window  (GtkIMContextHangul *hcontext);

static const IMHangulCombination compose_table_default[] = {
  { 0x11001100, 0x1101 }, /* choseong  kiyeok + kiyeok	= ssangkiyeok	*/
  { 0x11031103, 0x1104 }, /* choseong  tikeut + tikeut	= ssangtikeut	*/
  { 0x11071107, 0x1108 }, /* choseong  pieup  + pieup	= ssangpieup	*/
  { 0x11091109, 0x110a }, /* choseong  sios   + sios	= ssangsios	*/
  { 0x110c110c, 0x110d }, /* choseong  cieuc  + cieuc	= ssangcieuc	*/
  { 0x11691161, 0x116a }, /* jungseong o      + a	= wa		*/
  { 0x11691162, 0x116b }, /* jungseong o      + ae	= wae		*/
  { 0x11691175, 0x116c }, /* jungseong o      + i	= oe		*/
  { 0x116e1165, 0x116f }, /* jungseong u      + eo	= weo		*/
  { 0x116e1166, 0x1170 }, /* jungseong u      + e	= we		*/
  { 0x116e1175, 0x1171 }, /* jungseong u      + i	= wi		*/
  { 0x11731175, 0x1174 }, /* jungseong eu     + i	= yi		*/
  { 0x11a811a8, 0x11a9 }, /* jongseong kiyeok + kiyeok	= ssangekiyeok	*/
  { 0x11a811ba, 0x11aa }, /* jongseong kiyeok + sios	= kiyeok-sois	*/
  { 0x11ab11bd, 0x11ac }, /* jongseong nieun  + cieuc	= nieun-cieuc	*/
  { 0x11ab11c2, 0x11ad }, /* jongseong nieun  + hieuh	= nieun-hieuh	*/
  { 0x11af11a8, 0x11b0 }, /* jongseong rieul  + kiyeok	= rieul-kiyeok	*/
  { 0x11af11b7, 0x11b1 }, /* jongseong rieul  + mieum	= rieul-mieum	*/
  { 0x11af11b8, 0x11b2 }, /* jongseong rieul  + pieup	= rieul-pieup	*/
  { 0x11af11ba, 0x11b3 }, /* jongseong rieul  + sios	= rieul-sios	*/
  { 0x11af11c0, 0x11b4 }, /* jongseong rieul  + thieuth = rieul-thieuth	*/
  { 0x11af11c1, 0x11b5 }, /* jongseong rieul  + phieuph = rieul-phieuph	*/
  { 0x11af11c2, 0x11b6 }, /* jongseong rieul  + hieuh	= rieul-hieuh	*/
  { 0x11b811ba, 0x11b9 }, /* jongseong pieup  + sios	= pieup-sios	*/
  { 0x11ba11ba, 0x11bb }, /* jongseong sios   + sios	= ssangsios	*/
};


GType gtk_type_im_context_hangul = 0;

/* static variables for hangul immodule */
static GObjectClass *parent_class;

static GSList          *toplevels = NULL;

static guint		snooper_handler_id = 0;
static GtkIMContext    *current_focused_ic = NULL;

static gint		output_mode = OUTPUT_MODE_SYLLABLE;

/* preferences */
static gboolean		pref_use_capslock = FALSE;
static gboolean		pref_use_status_window = FALSE;
static gboolean		pref_use_dvorak = FALSE;
static gboolean		pref_use_manual_mode = FALSE;
static gboolean		pref_use_preedit_string = TRUE;
static void		(*im_hangul_preedit_attr)(GtkIMContextHangul *hic,
						  PangoAttrList **attrs,
						  gint start,
						  gint end) =
						im_hangul_preedit_foreground;
static GdkColor		pref_fg = { 0, 0xeeee, 0, 0 };
static GdkColor		pref_bg = { 0, 0xFFFF, 0xFFFF, 0xFFFF };

/* scanner */
static const GScannerConfig im_hangul_scanner_config = {
    (
     " \t\r\n"
    )                    /* cset_skip_characters */,
    (
     G_CSET_a_2_z
     "_"
     G_CSET_A_2_Z
    )                    /* cset_identifier_first */,
    (
     G_CSET_a_2_z
     "_"
     G_CSET_A_2_Z
     G_CSET_DIGITS
     G_CSET_LATINS
     G_CSET_LATINC
    )                    /* cset_identifier_nth */,
    ( "#\n" )             /* cpair_comment_single */,

    FALSE                 /* case_sensitive */,

    TRUE                  /* skip_comment_multi */,
    TRUE                  /* skip_comment_single */,
    TRUE                  /* scan_comment_multi */,
    TRUE                  /* scan_identifier */,
    FALSE                 /* scan_identifier_1char */,
    FALSE                 /* scan_identifier_NULL */,
    TRUE                  /* scan_symbols */,
    FALSE                 /* scan_binary */,
    TRUE                  /* scan_octal */,
    TRUE                  /* scan_float */,
    TRUE                  /* scan_hex */,
    FALSE                 /* scan_hex_dollar */,
    TRUE                  /* scan_string_sq */,
    TRUE                  /* scan_string_dq */,
    TRUE                  /* numbers_2_int */,
    FALSE                 /* int_2_float */,
    FALSE                 /* identifier_2_string */,
    TRUE                  /* char_2_token */,
    TRUE                  /* symbol_2_token */,
    FALSE                 /* scope_0_fallback */,
    FALSE                 /* store_int64 */,
};

enum {
    TOKEN_FALSE = G_TOKEN_LAST,
    TOKEN_TRUE,
    TOKEN_ENABLE_STATUS_WINDOW,
    TOKEN_ENABLE_PREEDIT,
    TOKEN_ENABLE_CAPSLOCK,
    TOKEN_ENABLE_DVORAK,
    TOKEN_PREEDIT_STYLE,
    TOKEN_PREEDIT_STYLE_FG,
    TOKEN_PREEDIT_STYLE_BG
};

static const struct {
    char *name;
    guint token;
} symbols[] = {
    { "false", TOKEN_FALSE },
    { "true", TOKEN_TRUE },
    { "off", TOKEN_FALSE },
    { "on", TOKEN_TRUE },
    { "enable_status_window", TOKEN_ENABLE_STATUS_WINDOW },
    { "enable_preedit", TOKEN_ENABLE_PREEDIT },
    { "enable_capslock", TOKEN_ENABLE_CAPSLOCK },
    { "enable_dvorak", TOKEN_ENABLE_DVORAK },
    { "preedit_style", TOKEN_PREEDIT_STYLE },
    { "preedit_style_fg", TOKEN_PREEDIT_STYLE_FG },
    { "preedit_style_bg", TOKEN_PREEDIT_STYLE_BG }
};

static void
set_preedit_style (const char *style)
{
    if (style == NULL) {
	im_hangul_preedit_attr = im_hangul_preedit_foreground;
    } else if (g_ascii_strcasecmp(style, "underline") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_underline;
    } else if (g_ascii_strcasecmp(style, "reverse") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_reverse;
    } else if (g_ascii_strcasecmp(style, "shade") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_shade;
    } else if (g_ascii_strcasecmp(style, "foreground") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_foreground;
    } else if (g_ascii_strcasecmp(style, "background") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_background;
    } else if (g_ascii_strcasecmp(style, "color") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_color;
    } else if (g_ascii_strcasecmp(style, "normal") == 0) {
	im_hangul_preedit_attr = im_hangul_preedit_normal;
    } else {
	im_hangul_preedit_attr = im_hangul_preedit_foreground;
    }
}

void im_hangul_config_parser(void)
{
    int i;
    int fd;
    FILE *file;
    GScanner *scanner;
    const gchar *env_conf_file;
    gchar *conf_file = NULL; 
    guint type;
    GTokenValue value;
    char *str;

    env_conf_file = g_getenv("IM_HANGUL_CONF_FILE");
    if (env_conf_file == NULL) {
	const gchar *homedir = g_get_home_dir();
	if (homedir == NULL)
	    return;

	conf_file = g_build_filename(homedir, ".imhangul.conf", NULL);
    } else {
	conf_file = g_strdup(env_conf_file);
    }

    file = fopen(conf_file, "r");
    g_free(conf_file);
    if (file == NULL)
	return;

    fd = fileno(file);
    scanner = g_scanner_new(&im_hangul_scanner_config);
    g_scanner_input_file(scanner, fd);

    for (i = 0; i < G_N_ELEMENTS (symbols); i++) {
	g_scanner_scope_add_symbol(scanner, 0,
			   symbols[i].name, GINT_TO_POINTER(symbols[i].token));
    }

    do {
	type = g_scanner_get_next_token(scanner);
	if (type == TOKEN_ENABLE_PREEDIT) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == TOKEN_TRUE) {
		    pref_use_preedit_string = TRUE;
		} else {
		    pref_use_preedit_string = FALSE;
		}
	    }
	} else if (type == TOKEN_ENABLE_STATUS_WINDOW) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == TOKEN_TRUE) {
		    pref_use_status_window = TRUE;
		} else {
		    pref_use_status_window = FALSE;
		}
	    }
	} else if (type == TOKEN_ENABLE_CAPSLOCK) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == TOKEN_TRUE) {
		    pref_use_capslock = TRUE;
		} else {
		    pref_use_capslock = FALSE;
		}
	    }
	} else if (type == TOKEN_ENABLE_DVORAK) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == TOKEN_TRUE) {
		    pref_use_dvorak = TRUE;
		} else {
		    pref_use_dvorak = FALSE;
		}
	    }
	} else if (type == TOKEN_PREEDIT_STYLE) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == G_TOKEN_IDENTIFIER) {
		    value = g_scanner_cur_value(scanner);
		    str = value.v_identifier;
		    set_preedit_style(str);
		}
	    }
	} else if (type == TOKEN_PREEDIT_STYLE_FG) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == G_TOKEN_STRING) {
		    value = g_scanner_cur_value(scanner);
		    str = value.v_identifier;
		    gdk_color_parse(str, &pref_fg);
		}
	    }
	} else if (type == TOKEN_PREEDIT_STYLE_BG) {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
		if (type == G_TOKEN_STRING) {
		    value = g_scanner_cur_value(scanner);
		    str = value.v_identifier;
		    gdk_color_parse(str, &pref_bg);
		}
	    }
	} else {
	    type = g_scanner_get_next_token(scanner);
	    if (type == G_TOKEN_EQUAL_SIGN) {
		type = g_scanner_get_next_token(scanner);
	    }
	}
    } while (!g_scanner_eof(scanner));

    g_scanner_destroy(scanner);

    fclose(file);
}

void
gtk_im_context_hangul_register_type (GTypeModule *type_module)
{
  static const GTypeInfo im_context_hangul_info = {
    sizeof(GtkIMContextHangulClass),
    (GBaseInitFunc) NULL,
    (GBaseFinalizeFunc) NULL,
    (GClassInitFunc) im_hangul_class_init,
    NULL,
    NULL,
    sizeof(GtkIMContextHangul),
    0,
    (GInstanceInitFunc) im_hangul_ic_init,
  };

  gtk_type_im_context_hangul =
      g_type_module_register_type (type_module,
				   GTK_TYPE_IM_CONTEXT,
				   "GtkIMContextHangul",
				   &im_context_hangul_info, 0);
}

static void 
im_hangul_class_init (GtkIMContextHangulClass *klass)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS(klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  parent_class = g_type_class_peek_parent (klass);

  im_context_class->set_client_window = im_hangul_ic_set_client_window;
  im_context_class->filter_keypress = im_hangul_ic_slave_filter_keypress;
  im_context_class->reset = im_hangul_ic_reset;
  im_context_class->focus_in = im_hangul_ic_focus_in;
  im_context_class->focus_out = im_hangul_ic_focus_out;
  im_context_class->get_preedit_string = im_hangul_get_preedit_string;
  im_context_class->set_use_preedit = im_hangul_ic_set_use_preedit;
  im_context_class->set_cursor_location = im_hangul_ic_cursor_location;

  gobject_class->finalize = im_hangul_ic_finalize;
}

static void
im_hangul_ic_clear_buf (GtkIMContextHangul *hcontext)
{
  hcontext->index = -1;
  hcontext->stack[0] = 0;
  hcontext->stack[1] = 0;
  hcontext->stack[2] = 0;
  hcontext->stack[3] = 0;
  hcontext->stack[4] = 0;
  hcontext->stack[5] = 0;
  hcontext->stack[6] = 0;
  hcontext->stack[7] = 0;
  hcontext->stack[8] = 0;
  hcontext->stack[9] = 0;
  hcontext->stack[10] = 0;
  hcontext->stack[11] = 0;

  hcontext->lindex = 0;
  hcontext->choseong[0] = 0;
  hcontext->choseong[1] = 0;
  hcontext->choseong[2] = 0;
  hcontext->choseong[3] = 0;

  hcontext->vindex = 0;
  hcontext->jungseong[0] = 0;
  hcontext->jungseong[1] = 0;
  hcontext->jungseong[2] = 0;
  hcontext->jungseong[3] = 0;

  hcontext->tindex = 0;
  hcontext->jongseong[0] = 0;
  hcontext->jongseong[1] = 0;
  hcontext->jongseong[2] = 0;
  hcontext->jongseong[3] = 0;
}

static void 
im_hangul_ic_init (GtkIMContextHangul *hcontext)
{
  im_hangul_ic_clear_buf (hcontext);

  hcontext->slave = gtk_im_context_simple_new();
  g_signal_connect(G_OBJECT(hcontext->slave), "commit",
		   G_CALLBACK(im_hangul_ic_commit_by_slave), hcontext);

  hcontext->client_window = NULL;
  hcontext->toplevel = NULL;
  hcontext->candidate = NULL;
  hcontext->cursor.x = 0;
  hcontext->cursor.y = 0;
  hcontext->cursor.width = -1;
  hcontext->cursor.height = -1;
  hcontext->surrounding_delete_length = 0;

  hcontext->composer = NULL;		/* initial value: composer == null */
  hcontext->keyboard_table = NULL;
  hcontext->compose_table_size = G_N_ELEMENTS(compose_table_default);
  hcontext->compose_table = compose_table_default;

  /* options */
  hcontext->always_use_jamo = FALSE;
  hcontext->use_preedit = TRUE;
}

static void
im_hangul_ic_finalize (GObject *object)
{
  GtkIMContextHangul *hic = GTK_IM_CONTEXT_HANGUL(object);

  if (hic->toplevel != NULL)
    toplevel_remove_context(hic->toplevel, hic);

  gtk_im_context_reset(hic->slave);
  g_signal_handlers_disconnect_by_func(hic->slave,
				       im_hangul_ic_commit_by_slave,
				       object);
  g_object_unref(G_OBJECT(hic->slave));
  hic->slave = NULL;

  G_OBJECT_CLASS(parent_class)->finalize (object);
  if ((GObject*)current_focused_ic == object)
    current_focused_ic = NULL;
}

static void
im_hangul_ic_set_client_window (GtkIMContext *context,
			     GdkWindow *client_window)
{
  GtkIMContextHangul *hcontext;

  g_return_if_fail (context != NULL);
  g_return_if_fail (GTK_IS_IM_CONTEXT_HANGUL (context));

  hcontext = GTK_IM_CONTEXT_HANGUL(context);

  if (hcontext->client_window == client_window)
    return;

  if (hcontext->toplevel != NULL)
    toplevel_remove_context(hcontext->toplevel, hcontext);

  if (client_window == NULL) {
    hcontext->client_window = NULL;
    hcontext->toplevel = NULL;
    return;
  }

  hcontext->client_window = client_window;
  hcontext->toplevel = toplevel_get (client_window);
  toplevel_append_context(hcontext->toplevel, hcontext);
}

GtkIMContext *
gtk_im_context_hangul_new (void)
{
  return GTK_IM_CONTEXT (g_object_new (GTK_TYPE_IM_CONTEXT_HANGUL, NULL));
}

void
gtk_im_context_hangul_set_composer (GtkIMContextHangul   *hcontext,
				    IMHangulComposerType  type)
{
  g_return_if_fail (hcontext);

  switch (type)
    {
    case IM_HANGUL_COMPOSER_2:
      hcontext->composer = im_hangul_ic_composer_2;
      break;
    case IM_HANGUL_COMPOSER_3:
      hcontext->composer = im_hangul_ic_composer_3;
      break;
    default:
      hcontext->composer = im_hangul_ic_composer_2;
      break;
    }
}

void
gtk_im_context_hangul_set_keyboard_table (GtkIMContextHangul *hcontext,
			                  const gunichar     *keyboard_table)
{
  g_return_if_fail (hcontext);

  hcontext->keyboard_table = keyboard_table;
}

void
gtk_im_context_hangul_set_compose_table (GtkIMContextHangul        *hcontext,
			                 const IMHangulCombination *compose_table,
			                 int                        compose_table_size)
{
  g_return_if_fail (hcontext);
  g_return_if_fail (compose_table);

  hcontext->compose_table = compose_table;
  hcontext->compose_table_size = compose_table_size;
}

void
gtk_im_context_hangul_set_use_jamo (GtkIMContextHangul *hcontext,
    				    gboolean		use_jamo)
{
  g_return_if_fail (hcontext);

  if (use_jamo)
    {
      hcontext->always_use_jamo = TRUE;
      output_mode |= OUTPUT_MODE_JAMO;
    }
  else
    {
      hcontext->always_use_jamo = FALSE;
      output_mode &= ~OUTPUT_MODE_JAMO;
    }
}

static void
im_hangul_set_input_mode_info_for_screen (GdkScreen *screen, int state)
{
  if (screen != NULL) {
    GdkWindow *root_window = gdk_screen_get_root_window(screen);
    long data = state;
    gdk_property_change (root_window,
			 gdk_atom_intern ("_HANGUL_INPUT_MODE", FALSE),
			 gdk_atom_intern ("INTEGER", FALSE),
			 32, GDK_PROP_MODE_REPLACE,
			 (const guchar *)&data, 1);
  }
}

static void
im_hangul_set_input_mode_info (GdkWindow *window, int state)
{
  if (window != NULL) {
    GdkScreen *screen = gdk_drawable_get_screen(window);
    im_hangul_set_input_mode_info_for_screen (screen, state);
  }
}

static void
im_hangul_set_input_mode(GtkIMContextHangul *hcontext, int mode)
{
  switch (mode) {
    case INPUT_MODE_DIRECT:
      im_hangul_set_input_mode_info (hcontext->client_window,
				     INPUT_MODE_INFO_ENGLISH);
      im_hangul_ic_hide_status_window(hcontext);
      g_signal_emit_by_name (hcontext, "preedit_end");
      break;
    case INPUT_MODE_HANGUL:
      im_hangul_set_input_mode_info (hcontext->client_window,
				     INPUT_MODE_INFO_HANGUL);
      im_hangul_ic_show_status_window(hcontext);
      g_signal_emit_by_name (hcontext, "preedit_start");
      break;
  }
  im_hangul_ic_set_toplevel_input_mode(hcontext, mode);
}

static void
im_hangul_ic_push (GtkIMContextHangul *hcontext, gunichar ch)
{
  hcontext->stack[++hcontext->index] = ch;
}

static gunichar
im_hangul_ic_peek (GtkIMContextHangul *hcontext)
{
  if (hcontext->index < 0)
    return 0;
  return hcontext->stack[hcontext->index];
}

static gunichar
im_hangul_ic_pop (GtkIMContextHangul *hcontext)
{
  if (hcontext->index < 0)
    return 0;
  return hcontext->stack[hcontext->index--];
}

/* choseong to compatibility jamo */
static gunichar
im_hangul_choseong_to_cjamo (gunichar ch)
{
  static gunichar table[] = {
    0x3131,	/* 0x1100 */
    0x3132,	/* 0x1101 */
    0x3134,	/* 0x1102 */
    0x3137,	/* 0x1103 */
    0x3138,	/* 0x1104 */
    0x3139,	/* 0x1105 */
    0x3141,	/* 0x1106 */
    0x3142,	/* 0x1107 */
    0x3143,	/* 0x1108 */
    0x3145,	/* 0x1109 */
    0x3146,	/* 0x110a */
    0x3147,	/* 0x110b */
    0x3148,	/* 0x110c */
    0x3149,	/* 0x110d */
    0x314a,	/* 0x110e */
    0x314b,	/* 0x110f */
    0x314c,	/* 0x1110 */
    0x314d,	/* 0x1111 */
    0x314e	/* 0x1112 */
  };

  if (ch < 0x1100 || ch > 0x1112)
    return 0;
  return table[ch - 0x1100];
}

/* jungseong to compatibility jamo */
static gunichar
im_hangul_jungseong_to_cjamo (gunichar ch)
{
  static gunichar table[] = {
    0x314f,	/* 0x1161 */
    0x3150,	/* 0x1162 */
    0x3151,	/* 0x1163 */
    0x3152,	/* 0x1164 */
    0x3153,	/* 0x1165 */
    0x3154,	/* 0x1166 */
    0x3155,	/* 0x1167 */
    0x3156,	/* 0x1168 */
    0x3157,	/* 0x1169 */
    0x3158,	/* 0x116a */
    0x3159,	/* 0x116b */
    0x315a,	/* 0x116c */
    0x315b,	/* 0x116d */
    0x315c,	/* 0x116e */
    0x315d,	/* 0x116f */
    0x315e,	/* 0x1170 */
    0x315f,	/* 0x1171 */
    0x3160,	/* 0x1172 */
    0x3161,	/* 0x1173 */
    0x3162,	/* 0x1174 */
    0x3163	/* 0x1175 */
  };

  if (ch < 0x1161 || ch > 0x1175)
    return 0;
  return table[ch - 0x1161];
}

/* jongseong to compatibility jamo */
static gunichar
im_hangul_jongseong_to_cjamo (gunichar ch)
{
  static gunichar table[] = {
    0x3131,	/* 0x11a8 */
    0x3132,	/* 0x11a9 */
    0x3133,	/* 0x11aa */
    0x3134,	/* 0x11ab */
    0x3135,	/* 0x11ac */
    0x3136,	/* 0x11ad */
    0x3137,	/* 0x11ae */
    0x3139,	/* 0x11af */
    0x313a,	/* 0x11b0 */
    0x313b,	/* 0x11b1 */
    0x313c,	/* 0x11b2 */
    0x313d,	/* 0x11b3 */
    0x313e,	/* 0x11b4 */
    0x313f,	/* 0x11b5 */
    0x3140,	/* 0x11b6 */
    0x3141,	/* 0x11b7 */
    0x3142,	/* 0x11b8 */
    0x3144,	/* 0x11b9 */
    0x3145,	/* 0x11ba */
    0x3146,	/* 0x11bb */
    0x3147,	/* 0x11bc */
    0x3148,	/* 0x11bd */
    0x314a,	/* 0x11be */
    0x314b,	/* 0x11bf */
    0x314c,	/* 0x11c0 */
    0x314d,	/* 0x11c1 */
    0x314e	/* 0x11c2 */
  };

  if (ch < 0x11a8 || ch > 0x11c2)
    return 0;
  return table[ch - 0x11a8];
}

#define HCF	0x115f
#define HJF	0x1160

static gunichar
im_hangul_jamo_to_syllable (gunichar choseong,
			    gunichar jungseong,
			    gunichar jongseong)
{
  static const gunichar hangul_base = 0xac00;
  static const gunichar choseong_base = 0x1100;
  static const gunichar jungseong_base = 0x1161;
  static const gunichar jongseong_base = 0x11a7;
/*  static const int nchoseong = 19; */
  static int njungseong = 21;
  static int njongseong = 28;
/*  static const int njungjong = 21 * 28; */
  gunichar ch;

  /* we use 0x11a7 like a Jongseong filler */
  if (jongseong == 0)
    jongseong = 0x11a7;		/* Jongseong filler */

  if (!(choseong  >= 0x1100 && choseong  <= 0x1112))
    return 0;
  if (!(jungseong >= 0x1161 && jungseong <= 0x1175))
    return 0;
  if (!(jongseong >= 0x11a7 && jongseong <= 0x11c2))
    return 0;

  choseong  -= choseong_base;
  jungseong -= jungseong_base;
  jongseong -= jongseong_base;
 
  ch = ((choseong * njungseong) + jungseong) * njongseong + jongseong
	 + hangul_base;
  return ch;
}

static int
im_hangul_make_preedit_string (GtkIMContextHangul *hcontext, gchar *buf)
{
  int i;
  int n = 0;
  gunichar ch;

  if (im_hangul_ic_is_empty (hcontext)) {
    buf[0] = '\0';
    return 0;
  }

  if (output_mode & OUTPUT_MODE_JAMO_EXT)
    {
      /* we use conjoining jamo, U+1100 - U+11FF */
      if (hcontext->choseong[0] == 0)
	n += g_unichar_to_utf8 (HCF, buf + n);
      else
	{
	  for (i = 0; i <= hcontext->lindex; i++)
	    n += g_unichar_to_utf8 (hcontext->choseong[i], buf + n);
	}

      if (hcontext->jungseong[0] == 0)
	n += g_unichar_to_utf8 (HJF, buf + n);
      else
	{
	  for (i = 0; i <= hcontext->vindex; i++)
	    n += g_unichar_to_utf8 (hcontext->jungseong[i], buf + n);
	}

      if (hcontext->jongseong[0] != 0)
	{
	  for (i = 0; i <= hcontext->tindex; i++)
	    n += g_unichar_to_utf8 (hcontext->jongseong[i], buf + n);
	}
      buf[n] = '\0';
    }
  else if (output_mode & OUTPUT_MODE_JAMO)
    {
      /* we use conjoining jamo, U+1100 - U+11FF */
      if (hcontext->choseong[0] == 0)
	n = g_unichar_to_utf8 (HCF, buf);
      else
	n = g_unichar_to_utf8 (hcontext->choseong[0], buf);
   
      if (hcontext->jungseong[0] == 0)
	n += g_unichar_to_utf8 (HJF, buf + n);
      else
	n += g_unichar_to_utf8 (hcontext->jungseong[0], buf + n);
   
      if (hcontext->jongseong[0] != 0) {
	n += g_unichar_to_utf8 (hcontext->jongseong[0], buf + n);
      }
      buf[n] = '\0';
    }
  else
    {
      /* this code is very stupid but pango has some bug in Xft
       * therefore we have to do this */
      if (hcontext->choseong[0])
	{
	  if (hcontext->jungseong[0])
	    {
	      /* have cho, jung, jong or no jong */
	      ch = im_hangul_jamo_to_syllable (hcontext->choseong[0],
					       hcontext->jungseong[0],
					       hcontext->jongseong[0]);
	      n = g_unichar_to_utf8 (ch, buf);
	      buf[n] = '\0';
	      return n;
	    }
	  else
	    {
	      if (hcontext->jongseong[0])
		{
		  /* have cho, jong */
		  ch = im_hangul_choseong_to_cjamo (hcontext->choseong[0]);
		  n = g_unichar_to_utf8 (ch, buf);
		  ch = im_hangul_jongseong_to_cjamo (hcontext->jongseong[0]);
		  n += g_unichar_to_utf8 (ch, buf + n);
		  buf[n] = '\0';
		  return n;
		}
	      else
		{
		  /* have cho */
		  ch = im_hangul_choseong_to_cjamo (hcontext->choseong[0]);
		  n = g_unichar_to_utf8 (ch, buf);
		  buf[n] = '\0';
		  return n;
		}
	    }
	}
      else
	{
	  if (hcontext->jungseong[0])
	    {
	      if (hcontext->jongseong[0]) {
		/* have jung, jong */
		ch = im_hangul_jungseong_to_cjamo (hcontext->jungseong[0]);
		n = g_unichar_to_utf8 (ch, buf);
		ch = im_hangul_jongseong_to_cjamo (hcontext->jongseong[0]);
		n += g_unichar_to_utf8 (ch, buf + n);
		buf[n] = '\0';
		return n;
	      } else {
		/* have jung */
		ch = im_hangul_jungseong_to_cjamo (hcontext->jungseong[0]);
		n = g_unichar_to_utf8 (ch, buf);
		buf[n] = '\0';
		return n;
	      }
	    }
	  else
	    {
	      if (hcontext->jongseong[0])
		{
		  /* have jong */
		  ch = im_hangul_jongseong_to_cjamo (hcontext->jongseong[0]);
		  n = g_unichar_to_utf8 (ch, buf);
		  buf[n] = '\0';
		  return n;
		}
	      else
		{
		  /* have nothing */
		  ;
		}
	    }
	} 
    }

  return n;
}

static void
im_hangul_preedit_underline (GtkIMContextHangul *hic,
			     PangoAttrList **attrs, gint start, gint end)
{
    PangoAttribute *attr;

    *attrs = pango_attr_list_new ();
    attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
    attr->start_index = start;
    attr->end_index = end;
    pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_reverse (GtkIMContextHangul *hic,
			   PangoAttrList **attrs, gint start, gint end)
{
    static GdkColor default_base = { 0, 0xffff, 0xffff, 0xffff };
    static GdkColor default_text = { 0, 0, 0, 0 };

    PangoAttribute *attr;
    GtkWidget *widget = NULL;
    GdkColor *fg = &default_base;
    GdkColor *bg = &default_text;

    gdk_window_get_user_data(hic->client_window, (gpointer)&widget);
    if (widget != NULL) {
	GtkStyle *style = gtk_widget_get_style(widget);
	fg = &style->base[GTK_STATE_NORMAL];
	bg = &style->text[GTK_STATE_NORMAL];
    }

    *attrs = pango_attr_list_new ();
    attr = pango_attr_foreground_new (fg->red, fg->green, fg->blue);
    attr->start_index = start;
    attr->end_index = end;
    pango_attr_list_insert (*attrs, attr);

    attr = pango_attr_background_new (bg->red, bg->green, bg->blue);
    attr->start_index = start;
    attr->end_index = end;
    pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_shade (GtkIMContextHangul *hic,
			   PangoAttrList **attrs, gint start, gint end)
{
    static const GdkColor default_text = { 0, 0, 0, 0 };
    static const GdkColor default_base = { 0, 0xffff * 90 / 100,
					      0xffff * 90 / 100,
					      0xffff * 90 / 100 };

    PangoAttribute *attr;
    GtkWidget *widget = NULL;
    GdkColor fg = default_text;
    GdkColor bg = default_base;

    gdk_window_get_user_data(hic->client_window, (gpointer)&widget);
    if (widget != NULL) {
	GtkStyle *style = gtk_widget_get_style(widget);
	if (style != NULL) {
	    fg.red   = style->text[GTK_STATE_NORMAL].red;
	    fg.green = style->text[GTK_STATE_NORMAL].green;
	    fg.blue  = style->text[GTK_STATE_NORMAL].blue;
	    bg.red   = (style->base[GTK_STATE_NORMAL].red   * 90 +
			style->text[GTK_STATE_NORMAL].red   * 10) / 100;
	    bg.green = (style->base[GTK_STATE_NORMAL].green * 90 +
			style->text[GTK_STATE_NORMAL].green * 10) / 100;
	    bg.blue  = (style->base[GTK_STATE_NORMAL].blue  * 90 +
			style->text[GTK_STATE_NORMAL].blue  * 10) / 100;
	}
    }

    *attrs = pango_attr_list_new ();
    attr = pango_attr_foreground_new (fg.red, fg.green, fg.blue);
    attr->start_index = start;
    attr->end_index = end;
    pango_attr_list_insert (*attrs, attr);

    attr = pango_attr_background_new (bg.red, bg.green, bg.blue);
    attr->start_index = start;
    attr->end_index = end;
    pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_foreground (GtkIMContextHangul *hic,
			      PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new ();
  attr = pango_attr_foreground_new (pref_fg.red, pref_fg.green, pref_fg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_background (GtkIMContextHangul *hic,
			      PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new ();
  attr = pango_attr_background_new (pref_bg.red, pref_bg.green, pref_bg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_color (GtkIMContextHangul *hic,
		         PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new ();
  attr = pango_attr_foreground_new (pref_fg.red, pref_fg.green, pref_fg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);

  attr = pango_attr_background_new (pref_bg.red, pref_bg.green, pref_bg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_normal (GtkIMContextHangul *hic,
			  PangoAttrList **attrs, gint start, gint end)
{
  /* we do nothing */
  *attrs = pango_attr_list_new ();
}

static void
im_hangul_get_preedit_string (GtkIMContext *context, gchar **str,
			      PangoAttrList **attrs,
			      gint *cursor_pos)
{
  gchar buf[40];
  int len = 0;
  GtkIMContextHangul *hcontext;
  
  g_return_if_fail (context != NULL);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);

  buf[0] = '\0';
  len = im_hangul_make_preedit_string (hcontext, buf);

  if (attrs)
    {
      im_hangul_preedit_attr (hcontext, attrs, 0, len);
    }

  if (cursor_pos)
    {
      *cursor_pos = g_utf8_strlen(buf, -1);
    }

  if (str)
    {
      *str = g_strdup (buf);
    }
}

static void
im_hangul_ic_focus_in (GtkIMContext *context)
{
  int input_mode;
  GtkIMContextHangul *hcontext;

  g_return_if_fail (context != NULL);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);
  input_mode = im_hangul_ic_get_toplevel_input_mode(hcontext);
  im_hangul_set_input_mode(hcontext, input_mode);

  current_focused_ic = context;
}

static inline void
im_hangul_ic_emit_preedit_changed (GtkIMContextHangul *hcontext)
{
  if (hcontext->use_preedit)
    g_signal_emit_by_name (hcontext, "preedit_changed");
}

static void
im_hangul_ic_focus_out (GtkIMContext *context)
{
  GtkIMContextHangul *hcontext;

  g_return_if_fail (context != NULL);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);
  if (hcontext->candidate == NULL)
    {
      if (im_hangul_ic_commit (hcontext))
	im_hangul_ic_emit_preedit_changed (hcontext);
    }

  im_hangul_ic_hide_status_window (hcontext);
  im_hangul_set_input_mode_info (hcontext->client_window, INPUT_MODE_INFO_NONE);
  if (current_focused_ic == context)
    current_focused_ic = NULL;
}

static void
im_hangul_ic_set_use_preedit (GtkIMContext *context, gboolean use_preedit)
{
  GtkIMContextHangul *hcontext;

  g_return_if_fail (context != NULL);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);
  hcontext->use_preedit = use_preedit;
}

static void
im_hangul_ic_cursor_location (GtkIMContext *context, GdkRectangle *area)
{
  GtkIMContextHangul *hcontext;

  g_return_if_fail (context != NULL);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);
  hcontext->cursor = *area;
}

static inline gboolean
im_hangul_is_modifier (guint state)
{
  return ((state & GDK_CONTROL_MASK) || (state & GDK_MOD1_MASK));
}

static inline gboolean
im_hangul_is_choseong (gunichar ch)
{
  return (ch >= 0x1100 && ch <= 0x1159);
}

static inline gboolean
im_hangul_is_jungseong (gunichar ch)
{
  return (ch >= 0x1161 && ch <= 0x11A2);
}

static inline gboolean
im_hangul_is_jongseong (gunichar ch)
{
  return (ch >= 0x11A7 && ch <= 0x11F9);
}

static inline gboolean
im_hangul_ic_is_empty (GtkIMContextHangul *hcontext)
{
  return (hcontext->choseong[0]  == 0 &&
	  hcontext->jungseong[0] == 0 &&
	  hcontext->jongseong[0] == 0 );
}

static inline gboolean
im_hangul_is_trigger (GdkEventKey *key)
{
  return ( key->keyval == GDK_Hangul || 
	   key->keyval == GDK_Alt_R ||
	  (key->keyval == GDK_space && (key->state & GDK_SHIFT_MASK)));
}

static inline gboolean
im_hangul_is_candidate (GdkEventKey *key)
{
  return (key->keyval == GDK_Hangul_Hanja ||
	  key->keyval == GDK_F9 ||
          key->keyval == GDK_Control_R);
}

static inline gboolean
im_hangul_is_backspace (GdkEventKey *key)
{
  return (key->keyval == GDK_BackSpace);
}

static void
im_hangul_ic_reset (GtkIMContext *context)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);

  if (im_hangul_ic_commit (hcontext))
    im_hangul_ic_emit_preedit_changed (hcontext);
}

static gboolean
im_hangul_ic_process_nonhangul (GtkIMContextHangul *hcontext,
			     GdkEventKey *key)
{
  /* Now, we do not process other keys. We just return this key event to
   * the default input module, simple. so do not use the routine below:
  if (!im_hangul_is_modifier (key->state))
    {
      gunichar ch = gdk_keyval_to_unicode (key->keyval);

      if (ch != 0)
	{
	  gchar buf[10];
	  gint n = g_unichar_to_utf8 (ch, buf);

	  buf[n] = '\0';
	  g_signal_emit_by_name (hcontext, "commit", buf);
	  return TRUE;
	} 
    }
  */
  return FALSE;
}

static gboolean
im_hangul_handle_direct_mode (GtkIMContextHangul *hcontext,
			      GdkEventKey *key)
{
  if (im_hangul_is_trigger (key))
    {
      if (im_hangul_ic_commit (hcontext))
	im_hangul_ic_emit_preedit_changed (hcontext);
      im_hangul_set_input_mode(hcontext, INPUT_MODE_HANGUL);
      return TRUE;
    }
  return im_hangul_ic_process_nonhangul (hcontext, key);
}

static void
im_hangul_ic_commit_by_slave (GtkIMContext *context, gchar *str, gpointer data)
{
  g_signal_emit_by_name (GTK_IM_CONTEXT_HANGUL(data), "commit", str);
}

static gboolean
im_hangul_ic_commit (GtkIMContextHangul *hcontext)
{
  int i;
  int n = 0;
  gchar buf[40];
 
  buf[0] = '\0';

  if (im_hangul_ic_is_empty (hcontext))
    return FALSE;

  if (output_mode & OUTPUT_MODE_JAMO_EXT)
    {
      /* we use conjoining jamo, U+1100 - U+11FF */
      if (hcontext->choseong[0] == 0)
	n += g_unichar_to_utf8 (HCF, buf + n);
      else
	{
	  for (i = 0; i <= hcontext->lindex; i++)
	    n += g_unichar_to_utf8 (hcontext->choseong[i], buf + n);
	}

      if (hcontext->jungseong[0] == 0)
	n += g_unichar_to_utf8 (HJF, buf + n);
      else
	{
	  for (i = 0; i <= hcontext->vindex; i++)
	    n += g_unichar_to_utf8 (hcontext->jungseong[i], buf + n);
	}

      if (hcontext->jongseong[0] != 0)
	{
	  for (i = 0; i <= hcontext->tindex; i++)
	    n += g_unichar_to_utf8 (hcontext->jongseong[i], buf + n);
	}
      buf[n] = '\0';
    }
  else if (output_mode & OUTPUT_MODE_JAMO)
    {
      /* we use conjoining jamo, U+1100 - U+11FF */
      if (hcontext->choseong[0] == 0)
	n = g_unichar_to_utf8 (HCF, buf);
      else
	n = g_unichar_to_utf8 (hcontext->choseong[0], buf);
   
      if (hcontext->jungseong[0] == 0)
	n += g_unichar_to_utf8 (HJF, buf + n);
      else
	n += g_unichar_to_utf8 (hcontext->jungseong[0], buf + n);
   
      if (hcontext->jongseong[0] != 0)
	n += g_unichar_to_utf8 (hcontext->jongseong[0], buf + n);

      buf[n] = '\0';
    }
  else
    {
      /* use hangul syllables (U+AC00 - U+D7AF)
       * and compatibility jamo (U+3130 - U+318F) */
      gunichar ch;
      ch = im_hangul_jamo_to_syllable (hcontext->choseong[0],
				       hcontext->jungseong[0],
				       hcontext->jongseong[0]);
   
      if (ch)
	{
	  n = g_unichar_to_utf8 (ch, buf);
	  buf[n] = '\0';
	}
      else
	{
	  if (hcontext->choseong[0])
	    {
	      ch = im_hangul_choseong_to_cjamo (hcontext->choseong[0]);
	      n = g_unichar_to_utf8 (ch, buf);
	      buf[n] = '\0';
	    }
	  if (hcontext->jungseong[0])
	    {
	      ch = im_hangul_jungseong_to_cjamo (hcontext->jungseong[0]);
	      n += g_unichar_to_utf8 (ch, buf + n);
	      buf[n] = '\0';
	    }
	  if (hcontext->jongseong[0])
	    {
	      ch = im_hangul_jongseong_to_cjamo (hcontext->jongseong[0]);
	      n += g_unichar_to_utf8 (ch, buf + n);
	      buf[n] = '\0';
	    }
	}
    }

  im_hangul_ic_clear_buf (hcontext);

  im_hangul_ic_emit_preedit_changed (hcontext);
  g_signal_emit_by_name (hcontext, "commit", buf);
  return TRUE;
}

/* this is a very dangerous function:
 * safe only when GDKKEYSYMS's value is enumarated  */
static guint
im_hangul_dvorak_to_qwerty (guint code)
{
  /* maybe safe if we use switch statement */
  static guint table[] = {
    GDK_exclam,			/* GDK_exclam */
    GDK_Q,			/* GDK_quotedbl */
    GDK_numbersign,		/* GDK_numbersign */
    GDK_dollar,			/* GDK_dollar */
    GDK_percent,		/* GDK_percent */
    GDK_ampersand,		/* GDK_ampersand */
    GDK_q,			/* GDK_apostrophe */
    GDK_parenleft,		/* GDK_parenleft */
    GDK_parenright,		/* GDK_parenright */
    GDK_asterisk,		/* GDK_asterisk */
    GDK_braceright,		/* GDK_plus */
    GDK_w,			/* GDK_comma */
    GDK_apostrophe,		/* GDK_minus */
    GDK_e,			/* GDK_period */
    GDK_bracketleft,		/* GDK_slash */
    GDK_0,			/* GDK_0 */
    GDK_1,			/* GDK_1 */
    GDK_2,			/* GDK_2 */
    GDK_3,			/* GDK_3 */
    GDK_4,			/* GDK_4 */
    GDK_5,			/* GDK_5 */
    GDK_6,			/* GDK_6 */
    GDK_7,			/* GDK_7 */
    GDK_8,			/* GDK_8 */
    GDK_9,			/* GDK_9 */
    GDK_Z,			/* GDK_colon */
    GDK_z,			/* GDK_semicolon */
    GDK_W,			/* GDK_less */
    GDK_bracketright,		/* GDK_qual */
    GDK_E,			/* GDK_greater */
    GDK_braceleft,		/* GDK_question */
    GDK_at,			/* GDK_at */
    GDK_A,			/* GDK_A */
    GDK_N,			/* GDK_B */
    GDK_I,			/* GDK_C */
    GDK_H,			/* GDK_D */
    GDK_D,			/* GDK_E */
    GDK_Y,			/* GDK_F */
    GDK_U,			/* GDK_G */
    GDK_J,			/* GDK_H */
    GDK_G,			/* GDK_I */
    GDK_C,			/* GDK_J */
    GDK_V,			/* GDK_K */
    GDK_P,			/* GDK_L */
    GDK_M,			/* GDK_M */
    GDK_L,			/* GDK_N */
    GDK_S,			/* GDK_O */
    GDK_R,			/* GDK_P */
    GDK_X,			/* GDK_Q */
    GDK_O,			/* GDK_R */
    GDK_colon,			/* GDK_S */
    GDK_K,			/* GDK_T */
    GDK_F,			/* GDK_U */
    GDK_greater,		/* GDK_V */
    GDK_less,			/* GDK_W */
    GDK_B,			/* GDK_X */
    GDK_T,			/* GDK_Y */
    GDK_question,		/* GDK_Z */
    GDK_minus,			/* GDK_bracketleft */
    GDK_backslash,		/* GDK_backslash */
    GDK_equal,			/* GDK_bracketright */
    GDK_asciicircum,		/* GDK_asciicircum */
    GDK_quotedbl,		/* GDK_underscore */
    GDK_grave,			/* GDK_grave */
    GDK_a,			/* GDK_a */
    GDK_n,			/* GDK_b */
    GDK_i,			/* GDK_c */
    GDK_h,			/* GDK_d */
    GDK_d,			/* GDK_e */
    GDK_y,			/* GDK_f */
    GDK_u,			/* GDK_g */
    GDK_j,			/* GDK_h */
    GDK_g,			/* GDK_i */
    GDK_c,			/* GDK_j */
    GDK_v,			/* GDK_k */
    GDK_p,			/* GDK_l */
    GDK_m,			/* GDK_m */
    GDK_l,			/* GDK_n */
    GDK_s,			/* GDK_o */
    GDK_r,			/* GDK_p */
    GDK_x,			/* GDK_q */
    GDK_o,			/* GDK_r */
    GDK_semicolon,		/* GDK_s */
    GDK_k,			/* GDK_t */
    GDK_f,			/* GDK_u */
    GDK_period,			/* GDK_v */
    GDK_comma,			/* GDK_w */
    GDK_b,			/* GDK_x */
    GDK_t,			/* GDK_y */
    GDK_slash,			/* GDK_z */
    GDK_underscore,		/* GDK_braceleft */
    GDK_bar,			/* GDK_bar */
    GDK_plus,			/* GDK_braceright */
    GDK_asciitilde,		/* GDK_asciitilde */
  };

  if (code < GDK_exclam || code > GDK_asciitilde)
    return code;
  return table[code - GDK_exclam];
}


static gunichar
im_hangul_compose (GtkIMContextHangul *hcontext,
    		   gunichar            first,
		   gunichar            last)
{
  int min, max, mid;
  guint32 key;
 
  /* make key */
  key = first << 16 | last;

  /* binary search in table */
  min = 0;
  max = hcontext->compose_table_size - 1;

  while (max >= min)
    {
      mid = (min + max) / 2;
      if (hcontext->compose_table[mid].key < key)
	min = mid + 1;
      else if (hcontext->compose_table[mid].key > key)
	max = mid - 1;
      else
	return hcontext->compose_table[mid].code;
    }

  return 0;
}

static gunichar
im_hangul_mapping (GtkIMContextHangul *hcontext,
		   guint	       keyval,
		   guint	       state)
{
  if (hcontext->keyboard_table == NULL)
    return 0;

  /* treat for dvorak */
  if (pref_use_dvorak)
    keyval = im_hangul_dvorak_to_qwerty (keyval);

  /* hangul jamo keysym */
  if (keyval >= 0x01001100 && keyval <= 0x010011ff)
    return keyval & 0x0000ffff;

  if (keyval >= GDK_exclam && keyval <= GDK_asciitilde)
    {
      /* treat capslock, as capslock is not on */
      if (state & GDK_LOCK_MASK)
	{
	  if (state & GDK_SHIFT_MASK)
	    {
	      if (keyval >= GDK_a && keyval <= GDK_z)
		keyval -= (GDK_a - GDK_A);
	    }
	  else
	    {
	      if (keyval >= GDK_A && keyval <= GDK_Z)
		keyval += (GDK_a - GDK_A);
	    }
	}
      return hcontext->keyboard_table[keyval - GDK_exclam];
    }

  return 0;
}

static void
im_hangul_candidate_commit(GtkIMContextHangul *hcontext,
			   gunichar ch)
{
  im_hangul_ic_clear_buf (hcontext);
  im_hangul_ic_emit_preedit_changed (hcontext);
  if (hcontext->surrounding_delete_length > 0)
    {
      gtk_im_context_delete_surrounding (GTK_IM_CONTEXT(hcontext),
				 0, hcontext->surrounding_delete_length);
      hcontext->surrounding_delete_length = 0;
    }
  im_hangul_ic_commit_unicode (hcontext, ch);
  candidate_delete (hcontext->candidate);
  hcontext->candidate = NULL;
}

static gboolean
im_hangul_cadidate_filter_keypress (GtkIMContextHangul *hcontext,
				    GdkEventKey *key)
{
  gunichar ch = 0;

  switch (key->keyval)
    {
      case GDK_Return:
      case GDK_KP_Enter:
	ch = candidate_get_current(hcontext->candidate);
	break;
      case GDK_Left:
      case GDK_h:
      case GDK_Page_Up:
	candidate_prev_page(hcontext->candidate);
	break;
      case GDK_Right:
      case GDK_l:
      case GDK_Page_Down:
	candidate_next_page(hcontext->candidate);
	break;
      case GDK_Up:
      case GDK_k:
      case GDK_BackSpace:
      case GDK_KP_Subtract:
	candidate_prev(hcontext->candidate);
	break;
      case GDK_Down:
      case GDK_j:
      case GDK_space:
      case GDK_KP_Add:
      case GDK_KP_Tab:
	candidate_next(hcontext->candidate);
	break;
      case GDK_Escape:
	candidate_delete(hcontext->candidate);
	hcontext->candidate = NULL;
	break;
      case GDK_0:
	ch = candidate_get_nth(hcontext->candidate, 9);
	break;
      case GDK_1:
      case GDK_2:
      case GDK_3:
      case GDK_4:
      case GDK_5:
      case GDK_6:
      case GDK_7:
      case GDK_8:
      case GDK_9:
	ch = candidate_get_nth(hcontext->candidate, key->keyval - GDK_1);
	break;
      default:
	break;
    }

  if (ch != 0)
    im_hangul_candidate_commit(hcontext, ch);

  return TRUE;
}

static gboolean
im_hangul_ic_slave_filter_keypress (GtkIMContext *context, GdkEventKey *key)
{
  GtkIMContextHangul *hcontext;

  g_return_val_if_fail (context != NULL, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);
  return gtk_im_context_filter_keypress(hcontext->slave, key);
}

/* use hangul composer */
static gboolean
im_hangul_ic_filter_keypress (GtkIMContext *context, GdkEventKey *key)
{
  GtkIMContextHangul *hcontext;

  g_return_val_if_fail (context != NULL, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  hcontext = GTK_IM_CONTEXT_HANGUL(context);

  /* ignore key release */
  if (key->type == GDK_KEY_RELEASE)
    return FALSE;

  /* we silently ignore shift keys */
  if (key->keyval == GDK_Shift_L || key->keyval == GDK_Shift_R)
    return FALSE;

  /* candidate window mode */
  if (hcontext->candidate != NULL)
    return im_hangul_cadidate_filter_keypress (hcontext, key);

  /* on Ctrl-Hangul we turn on/off manual_mode */
  if (pref_use_manual_mode &&
      key->keyval == GDK_Hangul && (key->state & GDK_CONTROL_MASK))
    output_mode ^= OUTPUT_MODE_JAMO_EXT;

  /* on capslock, we use Hangul Jamo */
  if (!hcontext->always_use_jamo)
    {
      if (pref_use_capslock && key->state & GDK_LOCK_MASK)
	output_mode |= OUTPUT_MODE_JAMO;
      else
	output_mode &= ~OUTPUT_MODE_JAMO;
    }

  /* handle direct mode */
  if (im_hangul_ic_get_toplevel_input_mode(hcontext) == INPUT_MODE_DIRECT)
    return im_hangul_handle_direct_mode (hcontext, key);

  /* handle Escape key: automaticaly change to direct mode */
  if (key->keyval == GDK_Escape)
    {
      if (im_hangul_ic_commit (hcontext))
	im_hangul_ic_emit_preedit_changed (hcontext);
      im_hangul_set_input_mode(hcontext, INPUT_MODE_DIRECT);
      return FALSE;
    }

  /* modifiler key */
  if (im_hangul_is_modifier (key->state))
    {
      if (im_hangul_ic_commit (hcontext))
	im_hangul_ic_emit_preedit_changed (hcontext);
      return FALSE;
    }

  /* hanja key */
  if (im_hangul_is_candidate(key))
    {
      popup_candidate_window (hcontext);
      return TRUE;
    }

  /* trigger key: mode change to direct mode */
  if (im_hangul_is_trigger (key))
    {
      if (im_hangul_ic_commit (hcontext))
	im_hangul_ic_emit_preedit_changed (hcontext);
      im_hangul_set_input_mode(hcontext, INPUT_MODE_DIRECT);
      return TRUE;
    }

  if (hcontext->composer)
    return hcontext->composer (hcontext, key);
  else
    {
      g_warning ("imhangul: null composer\n");
      return FALSE;
    }
}

/* status window */
static gboolean
status_window_expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_rectangle (widget->window,
		      widget->style->fg_gc[GTK_STATE_NORMAL],
		      FALSE,
		      0, 0,
		      widget->allocation.width-1, widget->allocation.height-1);

  return FALSE;
}

static gboolean
status_window_configure	(GtkWidget *widget,
			 GdkEventConfigure *event,
			 Toplevel *toplevel)
{
  GdkRectangle rect;
  GtkRequisition requisition;
  gint y;

  if (toplevel == NULL || toplevel->status == NULL)
    return FALSE;

  gdk_window_get_frame_extents (widget->window, &rect);
  gtk_widget_size_request (toplevel->status, &requisition);

  if (rect.y + rect.height + requisition.height < gdk_screen_height ())
    y = rect.y + rect.height;
  else
    y = gdk_screen_height () - requisition.height;

  gtk_window_move (GTK_WINDOW(toplevel->status), rect.x, y);
  return FALSE;
}

static GtkWidget*
status_window_new(GtkWidget *parent)
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *label;

  if (parent == NULL)
    return NULL;

  window = gtk_window_new (GTK_WINDOW_POPUP);

  gtk_container_set_border_width (GTK_CONTAINER(window), 1);
  /* gtk_window_set_decorated (GTK_WINDOW(window), FALSE); */
  gtk_widget_set_name (window, "imhangul_status");
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_widget_set_app_paintable (window, TRUE);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME(frame), GTK_SHADOW_OUT);
  gtk_widget_show (frame);
  gtk_container_add (GTK_CONTAINER(window), frame);

  /* hangul status window label */
  label = gtk_label_new (_("hangul")); 
  gtk_container_add (GTK_CONTAINER(frame), label);
  gtk_widget_show (label);

  g_signal_connect (G_OBJECT(window), "expose-event",
		   G_CALLBACK(status_window_expose_event), NULL);

  return window;
}

static void
im_hangul_ic_show_status_window (GtkIMContextHangul *hcontext)
{
  g_return_if_fail (hcontext != NULL);

  if (pref_use_status_window && hcontext->toplevel != NULL) {
    if (hcontext->toplevel->status == NULL) {
      hcontext->toplevel->status =
	status_window_new(hcontext->toplevel->widget);
	status_window_configure (hcontext->toplevel->widget,
				 NULL,
				 hcontext->toplevel);
    }
    gtk_widget_show (hcontext->toplevel->status);
  }
}

static void
im_hangul_ic_hide_status_window (GtkIMContextHangul *hcontext)
{
  g_return_if_fail (hcontext != NULL);

  if (hcontext->toplevel != NULL && hcontext->toplevel->status != NULL) {
    gtk_widget_hide (hcontext->toplevel->status);
  }
}

static GtkWidget *
get_toplevel_widget (GdkWindow *window)
{
  GtkWidget *gtk_toplevel;
  gpointer ptr;

  if (window == NULL)
    return NULL;

  gdk_window_get_user_data (window, &ptr);
  memcpy(&gtk_toplevel, &ptr, sizeof(gtk_toplevel));
  if (gtk_toplevel != NULL)
    gtk_toplevel = gtk_widget_get_toplevel(GTK_WIDGET(gtk_toplevel));

  return gtk_toplevel;
}

static void
toplevel_destroy(Toplevel *toplevel)
{
  if (toplevel != NULL) {
    toplevel_delete(toplevel);
    toplevels = g_slist_remove(toplevels, toplevel);
  }
}

static Toplevel *
toplevel_new(GtkWidget *toplevel_widget)
{
  Toplevel *toplevel = NULL;

  toplevel = g_new(Toplevel, 1);
  toplevel->input_mode = INPUT_MODE_DIRECT;
  toplevel->widget = toplevel_widget;
  toplevel->status = NULL;
  toplevel->contexts = NULL;
  toplevel->destroy_handler_id = 
	    g_signal_connect_swapped (G_OBJECT(toplevel->widget), "destroy",
			     G_CALLBACK(toplevel_destroy), toplevel);
  toplevel->configure_handler_id = 
	    g_signal_connect (G_OBJECT(toplevel->widget), "configure-event",
			     G_CALLBACK(status_window_configure),
			     toplevel);

  g_object_set_data(G_OBJECT(toplevel_widget),
		     "gtk-imhangul-toplevel-info", toplevel);
  return toplevel;
}

static Toplevel *
toplevel_get(GdkWindow *window)
{
  Toplevel *toplevel = NULL;
  GtkWidget *toplevel_widget;

  toplevel_widget = get_toplevel_widget (window);
  if (toplevel_widget == NULL) {
    return NULL;
  }

  toplevel = g_object_get_data(G_OBJECT(toplevel_widget),
			       "gtk-imhangul-toplevel-info");
  if (toplevel == NULL) {
    toplevel = toplevel_new(toplevel_widget);
    toplevels = g_slist_prepend(toplevels, toplevel);
  }

  return toplevel;
}

static void
toplevel_remove_context(Toplevel *toplevel, GtkIMContextHangul *context)
{
  if (toplevel == NULL || context == NULL)
    return;

  toplevel->contexts = g_slist_remove(toplevel->contexts, context);
}

static void
toplevel_append_context(Toplevel *toplevel, GtkIMContextHangul *context)
{
  if (toplevel == NULL || context == NULL)
    return;

  toplevel->contexts = g_slist_prepend(toplevel->contexts, context);
}

static void
toplevel_delete(Toplevel *toplevel)
{
  if (toplevel != NULL) {
    if (toplevel->status != NULL) {
      gtk_widget_destroy(toplevel->status);
    }
    if (toplevel->contexts != NULL) {
      GSList *item = toplevel->contexts;
      while (item != NULL) {
	GtkIMContextHangul *context = (GtkIMContextHangul *)(item->data);
	context->toplevel = NULL;
	item = g_slist_next(item);
      }
      g_slist_free(toplevel->contexts);
    }
    g_signal_handler_disconnect (toplevel->widget,
				 toplevel->configure_handler_id);
    g_signal_handler_disconnect (toplevel->widget,
				 toplevel->destroy_handler_id);
    g_object_set_data (G_OBJECT(toplevel->widget),
		       "gtk-imhangul-toplevel-info", NULL);
    g_free(toplevel);
  }
}

static int
im_hangul_ic_get_toplevel_input_mode(GtkIMContextHangul *hcontext)
{
  if (hcontext->toplevel == NULL)
    return INPUT_MODE_DIRECT;
  return hcontext->toplevel->input_mode;
}

static void
im_hangul_ic_set_toplevel_input_mode(GtkIMContextHangul *hcontext, int mode)
{
  if (hcontext->toplevel != NULL)
    hcontext->toplevel->input_mode = mode;
}

/*
 * candidate selection window
 */
static gint
get_index_of_candidate_table (gunichar ch)
{
  int first, last, mid;

  /* binary search */
  first = 0;
  last = G_N_ELEMENTS (candidate_table) - 1;
  while (first <= last)
    {
      mid = (first + last) / 2;

      if (ch == candidate_table[mid][0].ch)
	return mid;

      if (ch < candidate_table[mid][0].ch)
	last = mid - 1;
      else
	first = mid + 1;
    }
  return -1;
}

static gboolean
get_candidate_table (GtkIMContextHangul *hcontext,
		     gchar *label_buf,
		     gsize buf_size,
		     const CandidateItem **table)
{
  gunichar ch = 0;

  if (im_hangul_ic_is_empty (hcontext)) {
    gchar *text = NULL;
    gint cursor_index = 0;
    gtk_im_context_get_surrounding (GTK_IM_CONTEXT(hcontext),
				    &text, &cursor_index);
    if (text != NULL) {
      ch = g_utf8_get_char_validated (text + cursor_index, 3);
      g_free(text);
      hcontext->surrounding_delete_length = 1;
    }
  } else if (hcontext->choseong[0] != 0 &&
	     hcontext->jungseong[0] == 0 &&
	     hcontext->jongseong[0] == 0) {
    ch = im_hangul_choseong_to_cjamo(hcontext->choseong[0]);
  } else {
    ch = im_hangul_jamo_to_syllable (hcontext->choseong[0],
				     hcontext->jungseong[0],
				     hcontext->jongseong[0]);
  }

  if (ch > 0) {
    int index = get_index_of_candidate_table (ch);
    if (index != -1) {
      int n;
      n = g_unichar_to_utf8(ch, label_buf);
      label_buf[n] = '\0';
      *table = candidate_table[index] + 1;
      return TRUE;
    }
  }

  return FALSE;
}

static void
popup_candidate_window (GtkIMContextHangul *hcontext)
{
  gchar buf[12];
  const CandidateItem *table;
  gboolean ret;

  if (hcontext->candidate != NULL)
    {
      candidate_delete(hcontext->candidate);
      hcontext->candidate = NULL;
    }

  ret = get_candidate_table (hcontext, buf, sizeof(buf), &table);
  if (ret)
    {
      hcontext->candidate = candidate_new (buf,
					   9,
					   table,
					   hcontext->client_window,
					   &hcontext->cursor,
					   hcontext);
    }
}

static gint
im_hangul_key_snooper(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  if (current_focused_ic != NULL) {
    /* Some keys like return, tab, ':' is usually used for auto completion or
     * commiting some changes. Some application programmers make the program
     * catch the key before the im module getting the key and check that it is 
     * return or tab or so. Then the program get the string from the text entry
     * or textview, so there is no chance for im module to commit the current
     * string. So in this case, we catch it first and process the filter
     * function of the input context. Then mostly imhangul will work fine,
     * I think :) */
    return im_hangul_ic_filter_keypress(current_focused_ic, event);
  }

  return FALSE;
}

void
im_hangul_init(void)
{
  im_hangul_config_parser();

  /* install gtk key snooper
   * this is work around code for the problem:
   *   http://bugzilla.gnome.org/show_bug.cgi?id=62948
   * I finally decided to install key snooper and catch the keys before the
   * widget getting it. */
  snooper_handler_id = gtk_key_snooper_install(im_hangul_key_snooper, NULL);
}

void
im_hangul_finalize (void)
{
  GSList *item;

  /* remove gtk key snooper */
  if (snooper_handler_id > 0) {
    gtk_key_snooper_remove(snooper_handler_id);
    snooper_handler_id = 0;
  }

  /* remove toplevel info */
  for (item = toplevels; item != NULL; item = g_slist_next(item)) {
    toplevel_delete((Toplevel*)item->data);
  }
  g_slist_free(toplevels);
  toplevels = NULL;
}

/* composer functions (automata) */
static gunichar
im_hangul_choseong_to_jongseong (gunichar ch)
{
  static const gunichar table[] = {
    0x11a8,	/* choseong kiyeok	  ->	jongseong kiyeok      */
    0x11a9,	/* choseong ssangkiyeok	  ->	jongseong ssangkiyeok */
    0x11ab,	/* choseong nieun	  ->	jongseong nieun       */
    0x11ae,	/* choseong tikeut	  ->	jongseong tikeut      */
    0x0,	/* choseong ssangtikeut	  ->	jongseong tikeut      */
    0x11af,	/* choseong rieul	  ->	jongseong rieul       */
    0x11b7,	/* choseong mieum	  ->	jongseong mieum       */
    0x11b8,	/* choseong pieup	  ->	jongseong pieup       */
    0x0,	/* choseong ssangpieup	  ->	jongseong pieup       */
    0x11ba,	/* choseong sios	  ->	jongseong sios	      */
    0x11bb,	/* choseong ssangsios	  ->	jongseong ssangsios   */
    0x11bc,	/* choseong ieung	  ->	jongseong ieung       */
    0x11bd,	/* choseong cieuc	  ->	jongseong cieuc       */
    0x0,	/* choseong ssangcieuc	  ->	jongseong cieuc       */
    0x11be,	/* choseong chieuch	  ->	jongseong chieuch     */
    0x11bf,	/* choseong khieukh	  ->	jongseong khieukh     */
    0x11c0,	/* choseong thieuth	  ->	jongseong thieuth     */
    0x11c1,	/* choseong phieuph	  ->	jongseong phieuph     */
    0x11c2	/* choseong hieuh	  ->	jongseong hieuh       */
  };

  if (ch >= 0x11a8 && ch <= 0x11c2)
    return ch;
  if (ch < 0x1100 || ch > 0x1112)
    return 0;
  return table[ch - 0x1100];
}

static gunichar
im_hangul_jongseong_to_choseong (gunichar ch)
{
  static const gunichar table[] = {
    0x1100,	/* jongseong kiyeok	  ->	choseong kiyeok       */
    0x1101,	/* jongseong ssangkiyeok  ->	choseong ssangkiyeok  */
    0x1109,	/* jongseong kiyeok-sios  ->	choseong sios	      */
    0x1102,	/* jongseong nieun	  ->	choseong nieun	      */
    0x110c,	/* jongseong nieun-cieuc  ->	choseong cieuc	      */
    0x1112,	/* jongseong nieun-hieuh  ->	choseong hieuh	      */
    0x1103,	/* jongseong tikeut	  ->	choseong tikeut       */
    0x1105,	/* jongseong rieul	  ->	choseong rieul	      */
    0x1100,	/* jongseong rieul-kiyeok ->	choseong kiyeok       */
    0x1106,	/* jongseong rieul-mieum  ->	choseong mieum	      */
    0x1107,	/* jongseong rieul-pieup  ->	choseong pieup	      */
    0x1109,	/* jongseong rieul-sios   ->	choseong sios	      */
    0x1110,	/* jongseong rieul-thieuth -	choseong thieuth      */
    0x1111,	/* jongseong rieul-phieuph -	choseong phieuph      */
    0x1112,	/* jongseong rieul-hieuh  ->	choseong hieuh	      */
    0x1106,	/* jongseong mieum	  ->	choseong mieum	      */
    0x1107,	/* jongseong pieup	  ->	choseong pieup	      */
    0x1109,	/* jongseong pieup-sios   ->	choseong sios	      */
    0x1109,	/* jongseong sios	  ->	choseong sios	      */
    0x110a,	/* jongseong ssangsios	  ->	choseong ssangsios    */
    0x110b,	/* jongseong ieung	  ->	choseong ieung	      */
    0x110c,	/* jongseong cieuc	  ->	choseong cieuc	      */
    0x110e,	/* jongseong chieuch	  ->	choseong chieuch      */
    0x110f,	/* jongseong khieukh	  ->	choseong khieukh      */
    0x1110,	/* jongseong thieuth	  ->	choseong thieuth      */
    0x1111,	/* jongseong phieuph	  ->	choseong phieuph      */
    0x1112	/* jongseong hieuh	  ->	choseong hieuh	      */
  };

  if (ch < 0x11a8 || ch > 0x11c2)
    return 0;
  return table[ch - 0x11a8];
}

static void
im_hangul_jongseong_dicompose (gunichar ch,
			       gunichar* jong,
			       gunichar* cho)
{
  static const gunichar table[][2] = {
    { 0,      0x1100 }, /* jong kiyeok	      = cho  kiyeok		  */
    { 0x11a8, 0x1100 }, /* jong ssangkiyeok   = jong kiyeok + cho kiyeok  */
    { 0x11a8, 0x1109 }, /* jong kiyeok-sios   = jong kiyeok + cho sios	  */
    { 0,      0x1102 }, /* jong nieun	      = cho  nieun		  */
    { 0x11ab, 0x110c }, /* jong nieun-cieuc   = jong nieun  + cho cieuc   */
    { 0x11ab, 0x1112 }, /* jong nieun-hieuh   = jong nieun  + cho hieuh   */
    { 0,      0x1103 }, /* jong tikeut	      = cho  tikeut		  */
    { 0,      0x1105 }, /* jong rieul	      = cho  rieul		  */
    { 0x11af, 0x1100 }, /* jong rieul-kiyeok  = jong rieul  + cho kiyeok  */
    { 0x11af, 0x1106 }, /* jong rieul-mieum   = jong rieul  + cho mieum   */
    { 0x11af, 0x1107 }, /* jong rieul-pieup   = jong rieul  + cho pieup   */
    { 0x11af, 0x1109 }, /* jong rieul-sios    = jong rieul  + cho sios	  */
    { 0x11af, 0x1110 }, /* jong rieul-thieuth = jong rieul  + cho thieuth */
    { 0x11af, 0x1111 }, /* jong rieul-phieuph = jong rieul  + cho phieuph */
    { 0x11af, 0x1112 }, /* jong rieul-hieuh   = jong rieul  + cho hieuh   */
    { 0,      0x1106 }, /* jong mieum	      = cho  mieum		  */
    { 0,      0x1107 }, /* jong pieup	      = cho  pieup		  */
    { 0x11b8, 0x1109 }, /* jong pieup-sios    = jong pieup  + cho sios	  */
    { 0,      0x1109 }, /* jong sios	      = cho  sios		  */
    { 0x11ba, 0x1109 }, /* jong ssangsios     = jong sios   + cho sios	  */
    { 0,      0x110b }, /* jong ieung	      = cho  ieung		  */
    { 0,      0x110c }, /* jong cieuc	      = cho  cieuc		  */
    { 0,      0x110e }, /* jong chieuch       = cho  chieuch		  */
    { 0,      0x110f }, /* jong khieukh       = cho  khieukh		  */
    { 0,      0x1110 }, /* jong thieuth       = cho  thieuth		  */
    { 0,      0x1111 }, /* jong phieuph       = cho  phieuph		  */
    { 0,      0x1112 }	/* jong hieuh	      = cho  hieuh		  */
  };

  *jong = table[ch - 0x11a8][0];
  *cho	= table[ch - 0x11a8][1];
}

static gboolean
im_hangul_ic_composer_2 (GtkIMContextHangul *hcontext,
		      GdkEventKey *key)
{
  gunichar ch;
  gunichar comp_ch;
  gunichar jong_ch;

  ch = im_hangul_mapping (hcontext, key->keyval, key->state);

  if (hcontext->jongseong[0])
    {
      if (im_hangul_is_choseong (ch))
	{
	  jong_ch = im_hangul_choseong_to_jongseong (ch);
	  comp_ch = im_hangul_compose (hcontext, 
	      			       hcontext->jongseong[0], jong_ch);
	  if (im_hangul_is_jongseong (comp_ch))
	    {
	      hcontext->jongseong[0] = comp_ch;
	      im_hangul_ic_push (hcontext, comp_ch);
	    }
	  else
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	    }
	  goto done;
	}
      if (im_hangul_is_jungseong (ch))
	{
	  gunichar pop, peek;
	  pop = im_hangul_ic_pop (hcontext);
	  peek = im_hangul_ic_peek (hcontext);
	  if (im_hangul_is_jungseong (peek))
	    {
	      hcontext->jongseong[0] = 0;
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = im_hangul_jongseong_to_choseong (pop);
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, hcontext->choseong[0]);
	      im_hangul_ic_push (hcontext, ch);
	    }
	  else
	    {
	      gunichar choseong, jongseong; 
	      im_hangul_jongseong_dicompose (hcontext->jongseong[0],
					     &jongseong, &choseong);
	      hcontext->jongseong[0] = jongseong;
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = choseong;
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, choseong);
	      im_hangul_ic_push (hcontext, ch);
	    }
	  goto done;
	}
    }
  else if (hcontext->jungseong[0])
    {
      if (im_hangul_is_choseong (ch))
	{
	  if (hcontext->choseong[0])
	    {
	      jong_ch = im_hangul_choseong_to_jongseong (ch);
	      if (im_hangul_is_jongseong (jong_ch))
		{
		  hcontext->jongseong[0] = jong_ch;
		  im_hangul_ic_push (hcontext, jong_ch);
		}
	      else
		{
		  im_hangul_ic_commit (hcontext);
		  hcontext->choseong[0] = ch;
		  im_hangul_ic_push (hcontext, ch);
		}
	    }
	  else
	    {
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	    }
	  goto done;
	}
      if (im_hangul_is_jungseong (ch))
	{
	  comp_ch = im_hangul_compose (hcontext,
	      			       hcontext->jungseong[0], ch);
	  if (im_hangul_is_jungseong (comp_ch))
	    {
	      hcontext->jungseong[0] = comp_ch;
	      im_hangul_ic_push (hcontext, comp_ch);
	    }
	  else
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	    }
	  goto done;
	}
    }
  else if (hcontext->choseong[0])
    {
      if (im_hangul_is_choseong (ch))
	{
	  comp_ch = im_hangul_compose (hcontext,
	      			       hcontext->choseong[0], ch);
	  if (im_hangul_is_choseong (comp_ch))
	    {
	      hcontext->choseong[0] = comp_ch;
	      im_hangul_ic_push (hcontext, comp_ch);
	    }
	  else
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	    }
	  goto done;
	}
      if (im_hangul_is_jungseong (ch))
	{
	  hcontext->jungseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
    }
  else
    {
      if (im_hangul_is_choseong (ch))
	{
	  hcontext->choseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
      if (im_hangul_is_jungseong (ch))
	{
	  hcontext->jungseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
    }

  /* treat backspace */
  if (im_hangul_is_backspace (key))
    {
      ch = im_hangul_ic_pop (hcontext);
      if (ch == 0)
	return FALSE;

      if (im_hangul_is_choseong (ch))
	{
	  ch = im_hangul_ic_peek (hcontext);
	  hcontext->choseong[0] = im_hangul_is_choseong (ch) ? ch : 0;
	  goto done;
	}
      if (im_hangul_is_jungseong (ch))
	{
	  ch = im_hangul_ic_peek (hcontext);
	  hcontext->jungseong[0] = im_hangul_is_jungseong (ch) ? ch : 0;
	  goto done;
	}
      if (im_hangul_is_jongseong (ch))
	{
	  ch = im_hangul_ic_peek (hcontext);
	  hcontext->jongseong[0] = im_hangul_is_jongseong (ch) ? ch : 0;
	  goto done;
	}
      return FALSE;
    }

  if (im_hangul_ic_commit (hcontext))
    im_hangul_ic_emit_preedit_changed (hcontext);

  return im_hangul_ic_process_nonhangul (hcontext, key); /* english */

done:
  im_hangul_ic_emit_preedit_changed (hcontext);
  return TRUE;
}

static gboolean
im_hangul_add_choseong (GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->lindex >= 3)
    return FALSE;
  hcontext->lindex++;
  hcontext->choseong[hcontext->lindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_add_jungseong (GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->vindex >= 3)
    return FALSE;
  hcontext->vindex++;
  hcontext->jungseong[hcontext->vindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_add_jongseong (GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->tindex >= 3)
    return FALSE;
  hcontext->tindex++;
  hcontext->jongseong[hcontext->tindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_sub_choseong (GtkIMContextHangul *hcontext)
{
  hcontext->choseong[hcontext->lindex] = 0;
  if (hcontext->lindex <= 0)
    return FALSE;
  hcontext->lindex--;
  return TRUE;
}

static gboolean
im_hangul_sub_jungseong (GtkIMContextHangul *hcontext)
{
  hcontext->jungseong[hcontext->vindex] = 0;
  if (hcontext->vindex <= 0)
    return FALSE;
  hcontext->vindex--;
  return TRUE;
}

static gboolean
im_hangul_sub_jongseong (GtkIMContextHangul *hcontext)
{
  hcontext->jongseong[hcontext->tindex] = 0;
  if (hcontext->tindex <= 0)
    return FALSE;
  hcontext->tindex--;
  return TRUE;
}

static void
im_hangul_ic_commit_unicode (GtkIMContextHangul *hcontext, gunichar ch)
{
  int n;
  gchar buf[6];

  n = g_unichar_to_utf8 (ch, buf);
  buf[n] = '\0';

  im_hangul_ic_clear_buf (hcontext);

  g_signal_emit_by_name (hcontext, "commit", buf);
}

static gboolean
im_hangul_ic_composer_3 (GtkIMContextHangul *hcontext,
		      GdkEventKey *key)
{
  gunichar ch;

  ch = im_hangul_mapping (hcontext, key->keyval, key->state);

  if (output_mode & OUTPUT_MODE_JAMO_EXT)
    {
      if (hcontext->jongseong[0])
	{
	  if (im_hangul_is_choseong (ch))
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      if (!im_hangul_add_jongseong (hcontext, ch))
		{
		  im_hangul_ic_commit (hcontext);
		  hcontext->jongseong[0] = ch;
		}
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	}
      else if (hcontext->jungseong[0])
	{
	  if (im_hangul_is_choseong (ch))
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      if (!im_hangul_add_jungseong (hcontext, ch))
		{
		  im_hangul_ic_commit (hcontext);
		  hcontext->jungseong[0] = ch;
		}
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      hcontext->jongseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	}
      else if (hcontext->choseong[0])
	{
	  if (im_hangul_is_choseong (ch))
	    {
	      if (!im_hangul_add_choseong (hcontext, ch))
		{
		  im_hangul_ic_commit (hcontext);
		  hcontext->choseong[0] = ch;
		}
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      im_hangul_ic_commit (hcontext);
	      hcontext->jongseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	}
      else
	{
	  if (im_hangul_is_choseong (ch))
	    {
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      hcontext->jongseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	}

      /* treat backspace */
      if (im_hangul_is_backspace (key))
	{
	  ch = im_hangul_ic_pop (hcontext);
	  if (ch == 0)
	    return FALSE;
     
	  if (im_hangul_is_choseong (ch))
	    {
	      im_hangul_sub_choseong (hcontext);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      im_hangul_sub_jungseong (hcontext);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      im_hangul_sub_jongseong (hcontext);
	      goto done;
	    }
	  return FALSE;
	}
    }
  else
    {
      /* choseong */
      if (im_hangul_is_choseong (ch))
	{
	  if (hcontext->choseong[0] == 0)
	    {
	      hcontext->choseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_choseong (im_hangul_ic_peek (hcontext)))
	    {
	      gunichar choseong = im_hangul_compose (hcontext,
		  				     hcontext->choseong[0], ch);
	      if (choseong)
		{
		  hcontext->choseong[0] = choseong;
		  im_hangul_ic_push (hcontext, choseong);
		  goto done;
		}
	    }
	  im_hangul_ic_commit (hcontext);
	  hcontext->choseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
      /* junseong */
      if (im_hangul_is_jungseong (ch))
	{
	  if (hcontext->jungseong[0] == 0)
	    {
	      hcontext->jungseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jungseong (im_hangul_ic_peek (hcontext)))
	    {
	      gunichar jungseong = im_hangul_compose (hcontext,
		  				      hcontext->jungseong[0],
						      ch);
	      if (jungseong)
		{
		  hcontext->jungseong[0] = jungseong;
		  im_hangul_ic_push (hcontext, jungseong);
		  goto done;
		}
	    }
	  im_hangul_ic_commit (hcontext);
	  hcontext->jungseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
      /* jongseong */
      if (im_hangul_is_jongseong (ch))
	{
	  if (hcontext->jongseong[0] == 0)
	    {
	      hcontext->jongseong[0] = ch;
	      im_hangul_ic_push (hcontext, ch);
	      goto done;
	    }
	  if (im_hangul_is_jongseong (im_hangul_ic_peek (hcontext)))
	    {
	      gunichar jongseong = im_hangul_compose (hcontext,
		  				      hcontext->jongseong[0],
						      ch);
	      if (jongseong)
		{
		  hcontext->jongseong[0] = jongseong;
		  im_hangul_ic_push (hcontext, jongseong);
		  goto done;
		}
	    }
	  im_hangul_ic_commit (hcontext);
	  hcontext->jongseong[0] = ch;
	  im_hangul_ic_push (hcontext, ch);
	  goto done;
	}
      /* treat backspace */
      if (im_hangul_is_backspace (key))
	{
	  ch = im_hangul_ic_pop (hcontext);
	  if (ch == 0)
	    return FALSE;

	  if (im_hangul_is_choseong (ch))
	    {
	      ch = im_hangul_ic_peek (hcontext);
	      hcontext->choseong[0] = im_hangul_is_choseong (ch) ? ch : 0;
	      goto done;
	    }
	  if (im_hangul_is_jungseong (ch))
	    {
	      ch = im_hangul_ic_peek (hcontext);
	      hcontext->jungseong[0] = im_hangul_is_jungseong (ch) ? ch : 0;
	      goto done;
	    }
	  if (im_hangul_is_jongseong (ch))
	    {
	      ch = im_hangul_ic_peek (hcontext);
	      hcontext->jongseong[0] = im_hangul_is_jongseong (ch) ? ch : 0;
	      goto done;
	    }
	  return FALSE;
	}
    }

  /* number and puctuation */
  if (ch > 0)
    {
      im_hangul_ic_commit (hcontext);
      im_hangul_ic_commit_unicode (hcontext, ch);
      goto done;
    }

  if (im_hangul_ic_commit (hcontext))
      im_hangul_ic_emit_preedit_changed (hcontext);

  return im_hangul_ic_process_nonhangul (hcontext, key);

done:
  im_hangul_ic_emit_preedit_changed (hcontext);
  return TRUE;
}

/* candidate window */
enum {
  COLUMN_INDEX,
  COLUMN_CHARACTER,
  COLUMN_COMMENT,
  NO_OF_COLUMNS
};

static void
candidate_on_row_activated(GtkWidget *widget,
			   GtkTreePath *path,
			   GtkTreeViewColumn *column,
			   Candidate *candidate)
{
  if (path != NULL)
    {
      int *indices;
      GtkIMContextHangul *hcontext = candidate->hangul_context;

      indices = gtk_tree_path_get_indices(path);
      candidate->current = candidate->first + indices[0];
      im_hangul_candidate_commit(hcontext,
				 candidate->data[candidate->current].ch);
    }
}

static void
candidate_on_cursor_changed(GtkWidget *widget,
			    Candidate *candidate)
{
  GtkTreePath *path;

  gtk_tree_view_get_cursor(GTK_TREE_VIEW(widget), &path, NULL);
  if (path != NULL)
    {
      int *indices;
      indices = gtk_tree_path_get_indices(path);
      candidate->current = candidate->first + indices[0];
      gtk_tree_path_free(path);
    }
}

static gboolean
candidate_on_scroll(GtkWidget *widget,
		    GdkEventScroll *event,
		    gpointer data)
{
  Candidate *candidate;

  if (data == NULL)
    return FALSE;

  candidate = (Candidate*)data;
  switch (event->direction) {
    case GDK_SCROLL_UP:
      candidate_prev_page(candidate);
      break;
    case GDK_SCROLL_DOWN:
      candidate_next_page(candidate);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

static gboolean
candidate_on_key_press(GtkWidget *widget,
		       GdkEventKey *event,
		       gpointer data)
{
  Candidate *candidate;
  gunichar ch = 0;

  if (data == NULL)
    return FALSE;

  candidate = (Candidate*)data;
  switch (event->keyval) {
    case GDK_Return:
    case GDK_KP_Enter:
      ch = candidate_get_current(candidate);
      break;
    case GDK_Left:
    case GDK_h:
    case GDK_Page_Up:
      candidate_prev_page(candidate);
      break;
    case GDK_Right:
    case GDK_l:
    case GDK_Page_Down:
      candidate_next_page(candidate);
      break;
    case GDK_Up:
    case GDK_k:
    case GDK_BackSpace:
    case GDK_KP_Subtract:
      candidate_prev(candidate);
      break;
    case GDK_Down:
    case GDK_j:
    case GDK_space:
    case GDK_KP_Add:
    case GDK_KP_Tab:
      candidate_next(candidate);
      break;
    case GDK_Escape:
      candidate->hangul_context->candidate = NULL;
      candidate_delete(candidate);
      break;
    case GDK_0:
      ch = candidate_get_nth(candidate, 9);
      break;
    case GDK_1:
    case GDK_2:
    case GDK_3:
    case GDK_4:
    case GDK_5:
    case GDK_6:
    case GDK_7:
    case GDK_8:
    case GDK_9:
      ch = candidate_get_nth(candidate, event->keyval - GDK_1);
      break;
    default:
      return FALSE;
  }

  if (ch != 0)
    im_hangul_candidate_commit(candidate->hangul_context, ch);
  return TRUE;
}

static void
candidate_on_expose (GtkWidget *widget,
		     GdkEventExpose *event,
		     gpointer data)
{
  GtkStyle *style;
  GtkAllocation alloc;

  style = gtk_widget_get_style(widget);
  alloc = GTK_WIDGET(widget)->allocation;
  gdk_draw_rectangle(widget->window, style->black_gc,
		     FALSE,
		     0, 0, alloc.width - 1, alloc.height - 1);
}

static void
candidate_update_cursor(Candidate *candidate)
{
  GtkTreePath *path;

  if (candidate->treeview == NULL)
    return;

  path = gtk_tree_path_new_from_indices(candidate->current - candidate->first,                                          -1);
  gtk_tree_view_set_cursor(GTK_TREE_VIEW(candidate->treeview),
			   path, NULL, FALSE);
  gtk_tree_path_free(path);
}

static void
candidate_set_window_position (Candidate *candidate)
{
    gint width = 0, height = 0;
    gint absx = 0, absy = 0;
    gint root_w, root_h, cand_w, cand_h;
    GtkRequisition requisition;

    if (candidate->parent == NULL)
      return;

    gdk_window_get_origin (GDK_WINDOW(candidate->parent), &absx, &absy);
    gdk_drawable_get_size (GDK_DRAWABLE(candidate->parent), &width, &height);

    root_w = gdk_screen_width();
    root_h = gdk_screen_height();

    gtk_widget_size_request(GTK_WIDGET(candidate->window), &requisition);
    cand_w = requisition.width;
    cand_h = requisition.height;

    absx += candidate->cursor.x;
    absy += (candidate->cursor.height < 0)? 
	    height : candidate->cursor.y + candidate->cursor.height;

    if (absy + cand_h > root_h)
      absy = root_h - cand_h;
    if (absx + cand_w > root_w)
      absx = root_w - cand_w;
    gtk_window_move(GTK_WINDOW(candidate->window), absx, absy);
}

static void
candidate_update_list(Candidate *candidate)
{
  int i, len;
  gchar buf[16];
  GtkTreeIter iter;

  gtk_list_store_clear(candidate->store);
  for (i = 0;
       i < candidate->n_per_page && candidate->first + i < candidate->n;
       i++)
    {
      len = g_unichar_to_utf8(candidate->data[candidate->first + i].ch,
			      buf);
      buf[len] = '\0';
      gtk_list_store_append(candidate->store, &iter);
      gtk_list_store_set(candidate->store, &iter,
	  COLUMN_INDEX, (i + 1) % 10,
	  COLUMN_CHARACTER, buf,
	  COLUMN_COMMENT, candidate->data[candidate->first + i].comment,
	  -1);
    }
  candidate_set_window_position (candidate);
}

static void
candidate_create_window(Candidate *candidate)
{
  GtkWidget *frame;
  GtkWidget *treeview;
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  candidate->window = gtk_window_new(GTK_WINDOW_POPUP);

  candidate_update_list(candidate);

  frame = gtk_frame_new(candidate->label);
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_NONE);
  gtk_container_add(GTK_CONTAINER(candidate->window), frame);

  treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(candidate->store));
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);
  gtk_widget_set_name(GTK_WIDGET(treeview), "imhangul_candidate");
  gtk_container_add(GTK_CONTAINER(frame), treeview);
  candidate->treeview = treeview;
  g_object_unref(candidate->store);

  /* number column */
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("No",
						    renderer,
						    "text", COLUMN_INDEX,
						    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  /* character column */
  renderer = gtk_cell_renderer_text_new();
  g_object_set(renderer, "scale", 2.0, NULL);
  column = gtk_tree_view_column_new_with_attributes("Character",
						    renderer,
						    "text", COLUMN_CHARACTER,
						    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  /* comment column */
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Comment",
						    renderer,
						    "text", COLUMN_COMMENT,
						    NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

  candidate_update_cursor(candidate);

  g_signal_connect(G_OBJECT(treeview), "row-activated",
		   G_CALLBACK(candidate_on_row_activated), candidate);
  g_signal_connect(G_OBJECT(treeview), "cursor-changed",
		   G_CALLBACK(candidate_on_cursor_changed), candidate);
  g_signal_connect(G_OBJECT(candidate->window), "scroll-event",
		   G_CALLBACK(candidate_on_scroll), candidate);
  g_signal_connect(G_OBJECT(candidate->window), "key-press-event",
		   G_CALLBACK(candidate_on_key_press), candidate);
  g_signal_connect_after(G_OBJECT(candidate->window), "expose-event",
			 G_CALLBACK(candidate_on_expose), candidate);
  g_signal_connect_swapped(G_OBJECT(candidate->window), "realize",
			   G_CALLBACK(candidate_set_window_position),
			   candidate);

  gtk_widget_show_all(candidate->window);
  gtk_grab_add(candidate->window);
}

static Candidate*
candidate_new(char *label_str,
	      int n_per_page,
	      const CandidateItem *data,
	      GdkWindow *parent,
	      GdkRectangle *area,
	      GtkIMContextHangul *hcontext)
{
  int n;
  Candidate *candidate;

  candidate = (Candidate*)g_malloc(sizeof(Candidate));
  candidate->first = 0;
  candidate->current = 0;
  candidate->n_per_page = n_per_page;
  candidate->n = 0;
  candidate->data = NULL;
  candidate->parent = parent;
  candidate->cursor = *area;
  candidate->label = g_strdup(label_str);
  candidate->store = NULL;
  candidate->treeview = NULL;
  candidate->hangul_context = hcontext;

  for (n = 0; data[n].ch != 0; n++)
    continue;

  candidate->data = data;
  candidate->n = n;
  if (n_per_page == 0)
    candidate->n_per_page = candidate->n;

  candidate->store = gtk_list_store_new(NO_OF_COLUMNS,
				    G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);
  candidate_create_window(candidate);

  return candidate;
}

static void
candidate_prev(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  if (candidate->current > 0)
    candidate->current--;

  if (candidate->current < candidate->first)
    {
      candidate->first -= candidate->n_per_page;
      candidate_update_list(candidate);
    }
  candidate_update_cursor(candidate);
}

static void
candidate_next(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  if (candidate->current < candidate->n - 1)
    candidate->current++;

  if (candidate->current >= candidate->first + candidate->n_per_page)
    {
      candidate->first += candidate->n_per_page;
      candidate_update_list(candidate);
    }
  candidate_update_cursor(candidate);
}

static void
candidate_prev_page(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  if (candidate->first - candidate->n_per_page >= 0)
    {
      candidate->current -= candidate->n_per_page;
      if (candidate->current < 0)
	candidate->current = 0;
      candidate->first -= candidate->n_per_page;
      candidate_update_list(candidate);
    }
  candidate_update_cursor(candidate);
}

static void
candidate_next_page(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  if (candidate->first + candidate->n_per_page < candidate->n)
    {
      candidate->current += candidate->n_per_page;
      if (candidate->current > candidate->n - 1)
	candidate->current = candidate->n - 1;
      candidate->first += candidate->n_per_page;
      candidate_update_list(candidate);
    }
  candidate_update_cursor(candidate);
}

static gunichar
candidate_get_current(Candidate *candidate)
{
  if (candidate == NULL)
    return 0;

  return candidate->data[candidate->current].ch;
}

static gunichar
candidate_get_nth(Candidate *candidate, int index)
{
  if (candidate == NULL)
    return 0;

  index += candidate->first;
  if (index < 0 || index >= candidate->n)
    return 0;

  return candidate->data[index].ch;
}

void
candidate_delete(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  gtk_grab_remove(candidate->window);
  gtk_widget_destroy(candidate->window);
  g_free(candidate->label);
  g_free(candidate);
}
/* vim: set cindent sw=4 sts=4 ts=8 : */

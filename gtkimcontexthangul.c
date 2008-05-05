/* ImHangul - Gtk+ 2.0 Input Method Module for Hangul
 * Copyright (C) 2002-2008 Choe Hwanjin
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

#include <hangul.h>

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
  gchar *key;
  GtkIMContextHangul *hangul_context;
  GtkWidget *window;
  GdkWindow *parent;
  GdkRectangle cursor;
  GtkListStore *store;
  GtkWidget *treeview;
  HanjaList *list;
  int first;
  int n;
  int n_per_page;
  int current;
};

struct _CandidateItem {
    gunichar ch;
    gchar *comment;
};

static size_t ucschar_strlen(const ucschar* s);

static Candidate*  candidate_new             (char *key,
					      int n_per_page,
					      HanjaList *list,
					      GdkWindow *parent,
					      GdkRectangle *area,
					      GtkIMContextHangul *hcontext);
static void        candidate_prev            (Candidate *candidate);
static void        candidate_next            (Candidate *candidate);
static void        candidate_prev_page       (Candidate *candidate);
static void        candidate_next_page       (Candidate *candidate);
static const Hanja* candidate_get_current    (Candidate *candidate);
static const Hanja* candidate_get_nth        (Candidate *candidate, int index);
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
static inline gboolean im_hangul_is_trigger   (GdkEventKey *key);
static inline gboolean im_hangul_is_backspace (GdkEventKey *key);
static inline void     im_hangul_ic_emit_preedit_changed (GtkIMContextHangul *hcontext);

/* commit functions */
static void     im_hangul_ic_commit_by_slave (GtkIMContext *context,
					      gchar *str, gpointer data);

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

static char*    im_hangul_get_candidate_string(GtkIMContextHangul *ic);

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

GType gtk_type_im_context_hangul = 0;

/* static variables for hangul immodule */
static GObjectClass *parent_class;

static GSList          *toplevels = NULL;

static guint		snooper_handler_id = 0;
static GtkIMContext    *current_focused_ic = NULL;

static HanjaTable*      hanja_table = NULL;

/* preferences */
static gboolean		pref_use_capslock = FALSE;
static gboolean		pref_use_status_window = FALSE;
static gboolean		pref_use_dvorak = FALSE;
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
im_hangul_ic_init (GtkIMContextHangul *hcontext)
{
  hcontext->slave = gtk_im_context_simple_new();
  g_signal_connect(G_OBJECT(hcontext->slave), "commit",
		   G_CALLBACK(im_hangul_ic_commit_by_slave), hcontext);

  hcontext->client_window = NULL;
  hcontext->toplevel = NULL;
  hcontext->cursor.x = 0;
  hcontext->cursor.y = 0;
  hcontext->cursor.width = -1;
  hcontext->cursor.height = -1;

  hcontext->hic = hangul_ic_new("2");

  hcontext->candidate = NULL;
  hcontext->candidate_string = NULL;

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

  hangul_ic_delete(hic->hic);

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
gtk_im_context_hangul_select_keyboard(GtkIMContextHangul *hcontext,
			              const char *keyboard)
{
    g_return_if_fail (hcontext);

    hangul_ic_select_keyboard(hcontext->hic, keyboard);
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
    int len;
    const ucschar* preedit;
    GtkIMContextHangul *ic;

    g_return_if_fail (context != NULL);

    ic = GTK_IM_CONTEXT_HANGUL(context);
    preedit = hangul_ic_get_preedit_string(ic->hic);
    len = ucschar_strlen(preedit);

    if (attrs)
	im_hangul_preedit_attr(ic, attrs, 0, len);

    if (cursor_pos)
	*cursor_pos = len;

    if (str)
	*str = g_ucs4_to_utf8(preedit, len, NULL, NULL, NULL);
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

  im_hangul_ic_reset(context);

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
    const ucschar* flush;
    GtkIMContextHangul *hic = GTK_IM_CONTEXT_HANGUL (context);

    flush = hangul_ic_flush(hic->hic);
    if (flush[0] != 0) {
	char* str = g_ucs4_to_utf8(flush, -1, NULL, NULL, NULL);
	im_hangul_ic_emit_preedit_changed(hic);
	g_signal_emit_by_name(hic, "commit", str);
	g_free(str);
    }
}

static gboolean
im_hangul_handle_direct_mode (GtkIMContextHangul *hcontext,
			      GdkEventKey *key)
{
    if (im_hangul_is_trigger (key)) {
	im_hangul_ic_reset(GTK_IM_CONTEXT(hcontext));
	im_hangul_set_input_mode(hcontext, INPUT_MODE_HANGUL);
	return TRUE;
    }
    return FALSE;
}

static void
im_hangul_ic_commit_by_slave (GtkIMContext *context, gchar *str, gpointer data)
{
  g_signal_emit_by_name (GTK_IM_CONTEXT_HANGUL(data), "commit", str);
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
im_hangul_get_keyval(GtkIMContextHangul *hcontext,
		     guint	       keyval,
		     guint	       state)
{
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
      return keyval;
    }

  return 0;
}

static void
im_hangul_candidate_commit(GtkIMContextHangul *ic,
			   const char* match_key,
			   const Hanja* hanja)
{
    const char* key;
    const char* value;

    key = hanja_get_key(hanja);
    value = hanja_get_value(hanja);
    if (value != NULL) {
	gunichar* str = (gunichar*)ic->candidate_string->data;
	int len = ic->candidate_string->len;
	int len_to_delete = g_utf8_strlen(key, -1);
	int match_keylen = g_utf8_strlen(match_key, -1);

	if (!hangul_ic_is_empty(ic->hic)) {
	    const ucschar* preedit;
	    int preedit_len;
	    preedit = hangul_ic_get_preedit_string(ic->hic);
	    preedit_len = ucschar_strlen(preedit);

	    len_to_delete--;
	    match_keylen--;
	    len -= preedit_len;
	    hangul_ic_reset(ic->hic);
	    im_hangul_ic_emit_preedit_changed(ic);
	}

	if (len_to_delete > 0) {
	    while (match_keylen > len_to_delete) {
		int n = hangul_syllable_len(str, len);
		str += n;
		len -= n;
		match_keylen--;
	    }

	    gtk_im_context_delete_surrounding(GTK_IM_CONTEXT(ic), -len, len);
	}

	g_signal_emit_by_name(ic, "commit", value);
	candidate_delete(ic->candidate);
	ic->candidate = NULL;
	if (ic->candidate_string != NULL &&
	    ic->candidate_string->len > 0) {
	    g_array_remove_range(ic->candidate_string,
				 0, ic->candidate_string->len);
	}
    }
}

static gboolean
im_hangul_cadidate_filter_keypress (GtkIMContextHangul *hcontext,
				    GdkEventKey *key)
{
  const Hanja* hanja = NULL;

  switch (key->keyval)
    {
      case GDK_Return:
      case GDK_KP_Enter:
	hanja = candidate_get_current(hcontext->candidate);
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
	hanja = candidate_get_nth(hcontext->candidate, 9);
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
	hanja = candidate_get_nth(hcontext->candidate, key->keyval - GDK_1);
	break;
      default:
	break;
    }

  if (hanja != NULL)
      im_hangul_candidate_commit(hcontext, hcontext->candidate->key, hanja);

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
  int keyval;
  bool res;
  const ucschar* commit;
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

  /* on capslock, we use Hangul Jamo */
  if (!hcontext->always_use_jamo) {
      if (pref_use_capslock && key->state & GDK_LOCK_MASK)
	  hangul_ic_set_output_mode(hcontext->hic, HANGUL_OUTPUT_JAMO);
      else
	  hangul_ic_set_output_mode(hcontext->hic, HANGUL_OUTPUT_SYLLABLE);
  }

  /* handle direct mode */
  if (im_hangul_ic_get_toplevel_input_mode(hcontext) == INPUT_MODE_DIRECT)
    return im_hangul_handle_direct_mode (hcontext, key);

  /* handle Escape key: automaticaly change to direct mode */
  if (key->keyval == GDK_Escape)
    {
      im_hangul_ic_reset(context);
      im_hangul_set_input_mode(hcontext, INPUT_MODE_DIRECT);
      return FALSE;
    }

  /* modifiler key */
  if (im_hangul_is_modifier (key->state))
    {
      im_hangul_ic_reset(context);
      return FALSE;
    }

  /* hanja key */
  if (im_hangul_is_candidate(key))
    {
      popup_candidate_window (hcontext);
      return TRUE;
    }

  /* trigger key: mode change to direct mode */
  if (im_hangul_is_trigger(key)) {
      im_hangul_ic_reset(context);
      im_hangul_set_input_mode(hcontext, INPUT_MODE_DIRECT);
      return TRUE;
  }

  /* backspace */
  if (im_hangul_is_backspace(key)) {
      res = hangul_ic_backspace(hcontext->hic);
      if (res)
	  im_hangul_ic_emit_preedit_changed(hcontext);
      return res;
  }

  /* process */
  keyval = im_hangul_get_keyval(hcontext, key->keyval, key->state);
  res = hangul_ic_process(hcontext->hic, keyval);

  commit = hangul_ic_get_commit_string(hcontext->hic);
  if (commit[0] != 0) {
      char* str = g_ucs4_to_utf8(commit, -1, NULL, NULL, NULL);
      g_signal_emit_by_name (hcontext, "commit", str);
      g_free(str);
  }

  im_hangul_ic_emit_preedit_changed(hcontext);

  return res;
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
static char*
im_hangul_get_candidate_string(GtkIMContextHangul *ic)
{
    int n;
    gboolean res;
    gchar* text = NULL;
    gint cursor_index = 0;
    gunichar buf[20] = { 0, };
    char* str = NULL;

    n = G_N_ELEMENTS(buf);
    if (!hangul_ic_is_empty(ic->hic)) {
	const ucschar* preedit;
	int i, preedit_len;

	preedit = hangul_ic_get_preedit_string(ic->hic);
	preedit_len = ucschar_strlen(preedit);

	n -= preedit_len;
	for (i = 0; i < preedit_len; i++) {
	    buf[n + i] = preedit[i];
	}
    }

    res = gtk_im_context_get_surrounding(GTK_IM_CONTEXT(ic),
					 &text, &cursor_index);
    if (res && text != NULL) {
	gchar* p;

	p = g_utf8_find_prev_char(text, text + cursor_index);
	while (n > 0 && p != NULL) {
	    if (*p == ' ')
		break;

	    buf[n - 1] = g_utf8_get_char(p);

	    p = g_utf8_find_prev_char(text, p);
	    n--;
	}

	g_free(text);
    }

    if (n < G_N_ELEMENTS(buf)) {
	int len = G_N_ELEMENTS(buf) - n;
	if (ic->candidate_string == NULL)
	    ic->candidate_string = g_array_sized_new(FALSE, FALSE,
						     sizeof(gunichar), len);
	g_array_insert_vals(ic->candidate_string, 0, buf + n, len);

	len = hangul_jamos_to_syllables(buf, G_N_ELEMENTS(buf), buf + n, len);
	str = g_ucs4_to_utf8(buf, len, NULL, NULL, NULL);
    }

    return str;
}

static void
popup_candidate_window (GtkIMContextHangul *hcontext)
{
  char* key;
  HanjaList* list;

  if (hcontext->candidate != NULL)
    {
      candidate_delete(hcontext->candidate);
      hcontext->candidate = NULL;
    }

  if (hanja_table == NULL)
      hanja_table = hanja_table_load(NULL);

  key = im_hangul_get_candidate_string(hcontext);
  list = hanja_table_match_suffix(hanja_table, key);
  if (list != NULL) {
      hcontext->candidate = candidate_new (key,
					   9,
					   list,
					   hcontext->client_window,
					   &hcontext->cursor,
					   hcontext);
  }
  g_free(key);
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

/* candidate window */
enum {
  COLUMN_INDEX,
  COLUMN_KEY,
  COLUMN_VALUE,
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
      const Hanja* hanja;
      GtkIMContextHangul *hcontext = candidate->hangul_context;

      indices = gtk_tree_path_get_indices(path);
      candidate->current = candidate->first + indices[0];
      hanja = candidate_get_current(candidate);
      im_hangul_candidate_commit(hcontext, candidate->key, hanja);
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
  const Hanja* hanja = NULL;

  if (data == NULL)
    return FALSE;

  candidate = (Candidate*)data;
  switch (event->keyval) {
    case GDK_Return:
    case GDK_KP_Enter:
      hanja = candidate_get_current(candidate);
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
      hanja = candidate_get_nth(candidate, 9);
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
      hanja = candidate_get_nth(candidate, event->keyval - GDK_1);
      break;
    default:
      return FALSE;
  }

  if (hanja != NULL)
    im_hangul_candidate_commit(candidate->hangul_context,
			       candidate->key, hanja);
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
  int i;
  GtkTreeIter iter;

  gtk_list_store_clear(candidate->store);
  for (i = 0;
       i < candidate->n_per_page && candidate->first + i < candidate->n;
       i++)
    {
      const char* value;
      const char* comment;
      const Hanja* hanja;
      
      hanja = hanja_list_get_nth(candidate->list, candidate->first + i);
      value = hanja_get_value(hanja);
      comment = hanja_get_comment(hanja);

      gtk_list_store_append(candidate->store, &iter);
      gtk_list_store_set(candidate->store, &iter,
	      COLUMN_INDEX, (i + 1) % 10,
	      COLUMN_VALUE, value,
	      COLUMN_COMMENT, comment,
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

  frame = gtk_frame_new(candidate->key);
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
						    "text", COLUMN_VALUE,
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
candidate_new(char *key,
	      int n_per_page,
	      HanjaList *list,
	      GdkWindow *parent,
	      GdkRectangle *area,
	      GtkIMContextHangul *hcontext)
{
  Candidate *candidate;

  candidate = (Candidate*)g_malloc(sizeof(Candidate));
  candidate->key = g_strdup(key);
  candidate->first = 0;
  candidate->current = 0;
  candidate->n_per_page = n_per_page;
  candidate->list = list;
  candidate->n = hanja_list_get_size(list);
  candidate->parent = parent;
  candidate->cursor = *area;
  candidate->store = NULL;
  candidate->treeview = NULL;
  candidate->hangul_context = hcontext;

  if (n_per_page == 0)
    candidate->n_per_page = candidate->n;

  candidate->store = gtk_list_store_new(NO_OF_COLUMNS,
		    G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
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

static const Hanja*
candidate_get_current(Candidate *candidate)
{
  if (candidate == NULL)
    return 0;

  return hanja_list_get_nth(candidate->list, candidate->current);
}

static const Hanja*
candidate_get_nth(Candidate *candidate, int index_)
{
  if (candidate == NULL)
    return 0;

  index_ += candidate->first;
  if (index_ < 0 || index_ >= candidate->n)
    return 0;

  return hanja_list_get_nth(candidate->list, index_);
}

static void
candidate_delete(Candidate *candidate)
{
  if (candidate == NULL)
    return;

  gtk_grab_remove(candidate->window);
  gtk_widget_destroy(candidate->window);
  hanja_list_delete(candidate->list);
  g_free(candidate->key);
  g_free(candidate);
}

static size_t
ucschar_strlen(const ucschar* s)
{
    const ucschar* p = s;
    while (*p != 0)
	p++;
    return p - s;
}

/* vim: set cindent sw=4 sts=4 ts=8 : */

/* GTK - The GIMP Toolkit
 * Copyright (C) 2002 Choe Hwanjin
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
 *
 * Author: Choe Hwanjin <krisna@kldp.org>
 */

#ifndef __IM_HANGUL_H__
#define __IM_HANGUL_H__

#include <string.h>

#include <gdk/gdkkeysyms.h>

/*
#include "gtk/gtkmain.h"
#include "gtk/gtkbutton.h"
#include "gtk/gtkeventbox.h"
#include "gtk/gtklabel.h"
#include "gtk/gtkhbox.h"
#include "gtk/gtksignal.h"
#include "gtk/gtkwindow.h"
#include "gtk/gtkintl.h"
#include "gtk/gtkimcontext.h"
*/

#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtkeventbox.h>
#include <gtk/gtkframe.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkvbox.h>
#include <gtk/gtkhbox.h>
#include <gtk/gtktable.h>
#include <gtk/gtknotebook.h>
#include <gtk/gtkwindow.h>
#include <gtk/gtkimcontext.h>

#include "hanjatable.h"

static GType gtk_type_im_context_hangul = 0;

#define GTK_TYPE_IM_CONTEXT_HANGUL	gtk_type_im_context_hangul
#define GTK_IM_CONTEXT_HANGUL(obj)	(GTK_CHECK_CAST ((obj), GTK_TYPE_IM_CONTEXT_HANGUL, GtkIMContextHangul))

typedef struct _GtkIMContextHangul	GtkIMContextHangul;
typedef struct _GtkIMContextHangulClass	GtkIMContextHangulClass;
typedef gboolean (*GtkIMContextHangulAutomata) (GtkIMContextHangul *,
					        GdkEventKey *);

struct _GtkIMContextHangul
{
  GtkIMContext object;

  int state;
  int index;			/* stack index */
  gunichar stack[12];

  int lindex;			/* leading consonant index */
  int vindex;			/* vowel index */
  int tindex;			/* trailing consonant index */
  gunichar choseong[4];
  gunichar jungseong[4];
  gunichar jongseong[4];

  GtkIMContextHangulAutomata automata;
  GtkWidget *toplevel;
};

struct _GtkIMContextHangulClass
{
  GtkIMContextClass parent_class;
};

/* Status window: mostly copied from gtkimcontextxim.c */
struct _StatusWindow
{
  GtkWidget *window;
  GtkWidget *hangul_label;
  GtkWidget *toplevel;
  guint destroy_handler_id;
  guint configure_handler_id;
};

typedef struct _StatusWindow StatusWindow;

static void im_hangul_register_type (GTypeModule *type_module,
				     const gchar *type_name);

static GtkIMContext *im_hangul_new(void);
static void im_hangul_shutdown(void);
static void im_hangul_set_automata(GtkIMContextHangul *hcontext,
				   GtkIMContextHangulAutomata automata);

/* asistant function for hangul automata */
#define im_hangul_is_modifier(state)	((state & GDK_CONTROL_MASK) || \
					 (state & GDK_MOD1_MASK))
#define im_hangul_is_choseong(ch)	((ch) >= 0x1100 && (ch) <= 0x1159)
#define im_hangul_is_jungseong(ch)	((ch) >= 0x1161 && (ch) <= 0x11A2)
#define im_hangul_is_jongseong(ch)	((ch) >= 0x11A7 && (ch) <= 0x11F9)

#define im_hangul_is_empty(ctx)		((ctx)->choseong[0]  == 0 &&	\
					 (ctx)->jungseong[0] == 0 &&	\
					 (ctx)->jongseong[0] == 0 )

static gboolean	im_hangul_is_trigger			(GdkEventKey *key);
static gboolean	im_hangul_is_backspace			(GdkEventKey *key);

static void     im_hangul_reset(GtkIMContext *context);
static void	im_hangul_mode_hangul(GtkIMContextHangul *hcontext);
static void	im_hangul_mode_direct(GtkIMContextHangul *hcontext);

/* stack functions */
static gunichar	im_hangul_pop	(GtkIMContextHangul *hcontext);
static gunichar	im_hangul_peek	(GtkIMContextHangul *hcontext);
static void	im_hangul_push	(GtkIMContextHangul *hcontext, gunichar ch);

/* commit functions */
static void     im_hangul_clear_buf     (GtkIMContextHangul *hcontext);
static gboolean	im_hangul_commit	(GtkIMContextHangul *hcontext);
static void	im_hangul_commit_utf8	(GtkIMContextHangul *hcontext,
					 gchar *utf8);
static gboolean im_hangul_process_nonhangul(GtkIMContextHangul *hcontext,
					    GdkEventKey *key);
static void im_hangul_class_init	(GtkIMContextHangulClass *klass);
static void im_hangul_init		(GtkIMContextHangul *hcontext);
static void im_hangul_finalize		(GObject *obj);

static gboolean	im_hangul_filter_keypress(GtkIMContext *context,
				          GdkEventKey *key);

static void im_hangul_preedit_underline		(PangoAttrList **attrs,
    						 gint start, gint end);
static void im_hangul_preedit_foreground 	(PangoAttrList **attrs,
    						 gint start, gint end);
static void im_hangul_preedit_background	(PangoAttrList **attrs,
    						 gint start, gint end);
static void im_hangul_preedit_nothing 		(PangoAttrList **attrs,
    						 gint start, gint end);
static void im_hangul_get_preedit_string(GtkIMContext *ic,
					 gchar **str,
					 PangoAttrList **attrs,
					 gint *cursor_pos);
static void im_hangul_focus_in	(GtkIMContext *context);
static void im_hangul_focus_out	(GtkIMContext *context);

static GtkWidget* get_toplevel_window (GdkWindow *window);
static void im_hangul_set_client_window	(GtkIMContext *context,
				         GdkWindow *client_window);
static void status_window_show      (GtkIMContextHangul *hcontext);
static void status_window_hide      (GtkIMContextHangul *hcontext);
static void status_window_set_label (GtkIMContextHangul *hcontext);

static void popup_hanja_window	   (GtkIMContextHangul *hcontext);
static void popup_char_table_window      (GtkIMContextHangul *hcontext);

/*
 * global variables for hangul immodule
 */

static GObjectClass *parent_class;

static GtkIMContextHangul *current_context = NULL;
static GSList *status_windows = NULL;
static GtkWidget *hanja_window = NULL;
static GtkWidget *char_table_window = NULL;

enum {
  INPUT_MODE_DIRECT,
  INPUT_MODE_HANGUL,
  INPUT_MODE_HANJA
} IMHangulInputMode;

enum {
  INPUT_MODE_INFO_NONE,
  INPUT_MODE_INFO_ENGLISH,
  INPUT_MODE_INFO_HANGUL
} IMHangulInputModeInfo;

enum {
  STATE_DIRECT,
  STATE_HANGUL
};

#define OUTPUT_MODE_AUTOMATIC   0 
#define OUTPUT_MODE_MANUAL      1

static GdkAtom          input_mode_info_atom;
static GdkAtom          input_mode_info_type;
static gboolean         manual_mode = FALSE;
static gint		input_mode = INPUT_MODE_DIRECT;
/* static gint		output_mode = OUTPUT_MODE_AUTOMATIC; */

/* preferences */
static gboolean		pref_use_global_state = TRUE;
static gboolean		pref_use_caps_lock = FALSE;
static gboolean		pref_use_hangul_jamo = FALSE;
static gboolean		pref_use_status_window = TRUE;
static gboolean		pref_use_dvorak = FALSE;
static gchar* 		pref_hanja_font = NULL;
static gint		pref_preedit_style = 1;
static void             (*im_hangul_preedit_attr)(PangoAttrList **attrs,
                                                  gint start,
                                                  gint end) = NULL;
static GdkColor		pref_fg = { 0, 0, 0, 0 };
static GdkColor		pref_bg = { 0, 0xFFFF, 0xFFFF, 0xFFFF };

static guint16 im_hangul_ignore_table[] = {
  GDK_Control_L,
  GDK_Control_R,
  GDK_Caps_Lock,
  GDK_Shift_Lock,
  GDK_Meta_L,
  GDK_Meta_R,
  GDK_Alt_L,
  GDK_Alt_R,
  GDK_Super_L,
  GDK_Super_R,
  GDK_Hyper_L,
  GDK_Hyper_R,
  GDK_Mode_switch,
  0
};

static const gunichar *keyboard_table;

static void
im_hangul_register_type(GTypeModule *type_module, const gchar *type_name)
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
    (GtkObjectInitFunc) im_hangul_init,
  };

  gtk_type_im_context_hangul =
      g_type_module_register_type(type_module,
				  GTK_TYPE_IM_CONTEXT,
				  type_name,
				  &im_context_hangul_info, 0);
}

static gboolean
have_property (const gchar* str)
{
  GtkSettingsClass *klass = gtk_type_class (GTK_TYPE_SETTINGS);

  return (g_object_class_find_property (G_OBJECT_CLASS (klass), str) != NULL);
}

static void 
im_hangul_class_init (GtkIMContextHangulClass *klass)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS(klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

  parent_class = g_type_class_peek_parent (klass);

  im_context_class->set_client_window = im_hangul_set_client_window;
  im_context_class->filter_keypress = im_hangul_filter_keypress;
  im_context_class->reset = im_hangul_reset;
  im_context_class->focus_in = im_hangul_focus_in;
  im_context_class->focus_out = im_hangul_focus_out;
  im_context_class->get_preedit_string = im_hangul_get_preedit_string;
  gobject_class->finalize = im_hangul_finalize;

  /* install settings */
  /* check whether installed or not */
  if (!have_property("gtk-imhangul-status"))
    gtk_settings_install_property (g_param_spec_boolean ("gtk-imhangul-status",
  						         "Status Window",
						         "Whether to show status window or not",
						         FALSE,
						         G_PARAM_READWRITE));
  if (!have_property("gtk-imhangul-use-capslock"))
    gtk_settings_install_property (g_param_spec_boolean ("gtk-imhangul-use-capslock",
  						         "Use Caps Lock",
						         "Whether to use Caps Lock key for changing hangul output mode to Jamo or not",
						         FALSE,
						         G_PARAM_READWRITE));
  if (!have_property("gtk-imhangul-dvorak"))
    gtk_settings_install_property (g_param_spec_boolean ("gtk-imhangul-dvorak",
  						         "Dvorak Keyboard",
						         "Whether to use dvorak keyboard or not",
						         FALSE,
						         G_PARAM_READWRITE));
  if (!have_property("gtk-imhangul-style"))
    gtk_settings_install_property (g_param_spec_int ("gtk-imhangul-style",
  						     "Preedit Style",
						     "Preedit string style",
						     0,
						     4,
						     0,
						     G_PARAM_READWRITE));
  /* initiate input_mode_atom */
  input_mode_info_atom = gdk_atom_intern("_HANGUL_INPUT_MODE", TRUE);
  input_mode_info_type = gdk_atom_intern("INTEGER", TRUE);
}

static void
im_hangul_clear_buf (GtkIMContextHangul *hcontext)
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
im_hangul_init (GtkIMContextHangul *hcontext)
{
  im_hangul_clear_buf(hcontext);

  hcontext->state = STATE_DIRECT;	/* english mode */
  hcontext->automata = NULL;	        /* initial value: automata == null */
  hcontext->toplevel = NULL;

  g_object_get (gtk_settings_get_default(),
  		"gtk-imhangul-status", &pref_use_status_window,
  		"gtk-imhangul-use-capslock", &pref_use_caps_lock,
  		"gtk-imhangul-dvorak", &pref_use_dvorak,
  		"gtk-imhangul-style", &pref_preedit_style,
		NULL);

  switch (pref_preedit_style) {
    case 0:
      im_hangul_preedit_attr = im_hangul_preedit_underline;
      break;
    case 1:
      im_hangul_preedit_attr = im_hangul_preedit_foreground;
      break;
    case 2:
    case 3:
      im_hangul_preedit_attr = im_hangul_preedit_background;
      break;
    case 4:
      im_hangul_preedit_attr = im_hangul_preedit_nothing;
      break;
    default:
      im_hangul_preedit_attr = im_hangul_preedit_foreground;
  }
}

static void
im_hangul_finalize(GObject *obj)
{
  GtkIMContextHangul* hcontext = GTK_IM_CONTEXT_HANGUL(obj);

  if (hcontext == current_context)
    current_context = NULL;
  parent_class->finalize (obj);
}

static void
im_hangul_push(GtkIMContextHangul *hcontext, gunichar ch)
{
  hcontext->stack[++hcontext->index] = ch;
}

static gunichar
im_hangul_peek(GtkIMContextHangul *hcontext)
{
  if (hcontext->index < 0)
    return 0;
  return hcontext->stack[hcontext->index];
}

static gunichar
im_hangul_pop(GtkIMContextHangul *hcontext)
{
  if (hcontext->index < 0)
    return 0;
  return hcontext->stack[hcontext->index--];
}

static void
im_hangul_set_client_window(GtkIMContext *context,
			    GdkWindow *client_window)
{
  GtkStyle *style;
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);

  hcontext->toplevel = get_toplevel_window(client_window);

  if (hcontext->toplevel) {
    /* set fg, bg colors */
    style = hcontext->toplevel->style;
    switch (pref_preedit_style) {
      case 2:
        pref_fg.red 	= style->base[GTK_STATE_NORMAL].red;
        pref_fg.green 	= style->base[GTK_STATE_NORMAL].green;
        pref_fg.blue 	= style->base[GTK_STATE_NORMAL].blue;
        pref_bg.red 	= style->text[GTK_STATE_NORMAL].red;
        pref_bg.green 	= style->text[GTK_STATE_NORMAL].green;
        pref_bg.blue 	= style->text[GTK_STATE_NORMAL].blue;
	break;
      case 3:
        pref_fg.red 	= style->text[GTK_STATE_NORMAL].red;
        pref_fg.green 	= style->text[GTK_STATE_NORMAL].green;
        pref_fg.blue 	= style->text[GTK_STATE_NORMAL].blue;
        pref_bg.red 	= (style->base[GTK_STATE_NORMAL].red   * 90 + 
      			   style->text[GTK_STATE_NORMAL].red   * 10) / 100;
        pref_bg.green 	= (style->base[GTK_STATE_NORMAL].green * 90 + 
        	           style->text[GTK_STATE_NORMAL].green * 10) / 100;
        pref_bg.blue 	= (style->base[GTK_STATE_NORMAL].blue  * 90 + 
        	           style->text[GTK_STATE_NORMAL].blue  * 10) / 100;
	break;
      default:
        pref_fg.red 	= style->text[GTK_STATE_NORMAL].red;
        pref_fg.green 	= style->text[GTK_STATE_NORMAL].green;
        pref_fg.blue 	= style->text[GTK_STATE_NORMAL].blue;
        pref_bg.red 	= style->base[GTK_STATE_NORMAL].red;
        pref_bg.green 	= style->base[GTK_STATE_NORMAL].green;
        pref_bg.blue 	= style->base[GTK_STATE_NORMAL].blue;
    }
  }
}

static GtkIMContext *
im_hangul_new(void)
{
  return GTK_IM_CONTEXT (g_object_new (GTK_TYPE_IM_CONTEXT_HANGUL, NULL));
}

static void
im_hangul_set_automata(GtkIMContextHangul *hcontext,
		       GtkIMContextHangulAutomata automata)
{
  hcontext->automata = automata;
}

static void
im_hangul_set_input_mode_info(int state)
{
  static GdkWindow *current_root_window = NULL;

  if (current_root_window == NULL)
    current_root_window = GDK_ROOT_PARENT();

  gdk_property_change(current_root_window,
                      input_mode_info_atom,
                      input_mode_info_type,
                      32, GDK_PROP_MODE_REPLACE,
                      (const guchar *)&state, 1);
}

static void 
im_hangul_mode_hangul(GtkIMContextHangul *hcontext)
{
  input_mode = INPUT_MODE_HANGUL;
  hcontext->state = STATE_HANGUL;
  im_hangul_set_input_mode_info(INPUT_MODE_INFO_HANGUL);
  status_window_set_label(hcontext);
}

static void 
im_hangul_mode_direct(GtkIMContextHangul *hcontext)
{
  input_mode = INPUT_MODE_DIRECT;
  hcontext->state = STATE_DIRECT;
  im_hangul_set_input_mode_info(INPUT_MODE_INFO_ENGLISH);
  status_window_set_label(hcontext);
}

static gunichar
im_hangul_choseong_to_unicode(gunichar ch)
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

static gunichar
im_hangul_jungseong_to_unicode(gunichar ch)
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

static gunichar
im_hangul_jongseong_to_unicode(gunichar ch)
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
im_hangul_jamo_to_syllable(gunichar choseong,
			   gunichar jungseong,
			   gunichar jongseong)
{
  static gunichar hangul_base = 0xac00;
  static gunichar choseong_base = 0x1100;
  static gunichar jungseong_base = 0x1161;
  static gunichar jongseong_base = 0x11a7;
/*  static int nchoseong = 19; */
  static int njungseong = 21;
  static int njongseong = 28;
/*  static int njungjong = 21 * 28; */
  gunichar ch;

  /* we use 0x11a7 like a Jongseong filler */
  if (jongseong == 0)
    jongseong = 0x11a7;         /* Jongseong filler */

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
im_hangul_preedit_string(GtkIMContextHangul *hcontext, gchar *buf)
{
  int i;
  int n = 0;
  gunichar ch;

  if (im_hangul_is_empty(hcontext)) {
    buf[0] = '\0';
    return 0;
  }

  if (manual_mode) {
    /* we use conjoining jamo, U+1100 - U+11FF */
    if (hcontext->choseong[0] == 0)
      n += g_unichar_to_utf8(HCF, buf + n);
    else {
      for (i = 0; i <= hcontext->lindex; i++)
        n += g_unichar_to_utf8(hcontext->choseong[i], buf + n);
    }
    if (hcontext->jungseong[0] == 0)
      n += g_unichar_to_utf8(HJF, buf + n);
    else {
      for (i = 0; i <= hcontext->vindex; i++)
        n += g_unichar_to_utf8(hcontext->jungseong[i], buf + n);
    }
    if (hcontext->jongseong[0] != 0) {
      for (i = 0; i <= hcontext->tindex; i++)
        n += g_unichar_to_utf8(hcontext->jongseong[i], buf + n);
    }
    buf[n] = '\0';
  } else if (pref_use_hangul_jamo) {
    /* we use conjoining jamo, U+1100 - U+11FF */
    if (hcontext->choseong[0] == 0)
      n = g_unichar_to_utf8(HCF, buf);
    else
      n = g_unichar_to_utf8(hcontext->choseong[0], buf);
 
    if (hcontext->jungseong[0] == 0)
      n += g_unichar_to_utf8(HJF, buf + n);
    else
      n += g_unichar_to_utf8(hcontext->jungseong[0], buf + n);
 
    if (hcontext->jongseong[0] != 0) {
      n += g_unichar_to_utf8(hcontext->jongseong[0], buf + n);
    }
    buf[n] = '\0';
  } else {
    /* this code is very stupid but pango has some bug in Xft
     * therefore we have to do this */
    if (hcontext->choseong[0]) {
      if (hcontext->jungseong[0]) {
        /* have cho, jung, jong or no jong */
        ch = im_hangul_jamo_to_syllable(hcontext->choseong[0],
        			        hcontext->jungseong[0],
        			        hcontext->jongseong[0]);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      } else {
        if (hcontext->jongseong[0]) {
          /* have cho, jong */
          ch = im_hangul_choseong_to_unicode(hcontext->choseong[0]);
          n = g_unichar_to_utf8(ch, buf);
          ch = im_hangul_jongseong_to_unicode(hcontext->jongseong[0]);
          n += g_unichar_to_utf8(ch, buf + n);
          buf[n] = '\0';
          return n;
        } else {
          /* have cho */
          ch = im_hangul_choseong_to_unicode(hcontext->choseong[0]);
          n = g_unichar_to_utf8(ch, buf);
          buf[n] = '\0';
          return n;
        }
      }
    } else {
      if (hcontext->jungseong[0]) {
        if (hcontext->jongseong[0]) {
          /* have jung, jong */
          ch = im_hangul_jungseong_to_unicode(hcontext->jungseong[0]);
          n = g_unichar_to_utf8(ch, buf);
          ch = im_hangul_jongseong_to_unicode(hcontext->jongseong[0]);
          n += g_unichar_to_utf8(ch, buf + n);
          buf[n] = '\0';
          return n;
        } else {
          /* have jung */
          ch = im_hangul_jungseong_to_unicode(hcontext->jungseong[0]);
          n = g_unichar_to_utf8(ch, buf);
          buf[n] = '\0';
          return n;
        }
      } else {
        if (hcontext->jongseong[0]) {
          /* have jong */
          ch = im_hangul_jongseong_to_unicode(hcontext->jongseong[0]);
          n = g_unichar_to_utf8(ch, buf);
          buf[n] = '\0';
          return n;
        } else {
          /* have nothing */
          ;
        }
      }
    }
  }

  /*
  g_print("PreEdit: U+%04x U+%04x U+%04x (%d)\n",
  	  hcontext->choseong[0],
	  hcontext->jungseong[0],
	  hcontext->jongseong[0], n);
  */
  return n;
}

static void
im_hangul_preedit_underline (PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new();
  attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_foreground (PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new();
  attr = pango_attr_foreground_new(0xeeee, 0x0, 0x0);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_background (PangoAttrList **attrs, gint start, gint end)
{
  PangoAttribute *attr;

  *attrs = pango_attr_list_new();
  attr = pango_attr_foreground_new(pref_fg.red, pref_fg.green, pref_fg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);

  attr = pango_attr_background_new(pref_bg.red, pref_bg.green, pref_bg.blue);
  attr->start_index = start;
  attr->end_index = end;
  pango_attr_list_insert (*attrs, attr);
}

static void
im_hangul_preedit_nothing (PangoAttrList **attrs, gint start, gint end)
{
  /* we do nothing */
  *attrs = pango_attr_list_new();
}

static void
im_hangul_get_preedit_string(GtkIMContext *context, gchar **str,
			     PangoAttrList **attrs,
			     gint *cursor_pos)
{
  gchar buf[40];
  int len = 0;
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);

  buf[0] = '\0';
  len = im_hangul_preedit_string(hcontext, buf);

  if (attrs) {
    im_hangul_preedit_attr(attrs, 0, len);
  }

  if (cursor_pos) {
    *cursor_pos = len;
  }

  if (str) {
    *str = g_strdup(buf);
  }
}

static void
im_hangul_focus_in(GtkIMContext *context)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);

  if (input_mode == INPUT_MODE_DIRECT) {
    im_hangul_set_input_mode_info (INPUT_MODE_INFO_ENGLISH);
    if (pref_use_global_state)
      hcontext->state = STATE_DIRECT;
  } else {
    im_hangul_set_input_mode_info (INPUT_MODE_INFO_HANGUL);
    if (pref_use_global_state)
      hcontext->state = STATE_HANGUL;
  }

  current_context = hcontext;
  status_window_show(hcontext);
}

static void
im_hangul_focus_out(GtkIMContext *context)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);

  if (hanja_window == NULL) {
    if (im_hangul_commit(hcontext)) {
      g_signal_emit_by_name (hcontext, "preedit_changed");
      hcontext->state = STATE_HANGUL;
    }
  }

  status_window_hide(hcontext);
  im_hangul_set_input_mode_info (INPUT_MODE_INFO_NONE);
}

static gboolean
im_hangul_is_ignore_key(guint16 key)
{
  int i;
  for (i = 0; im_hangul_ignore_table[i]; i++) {
    if (key == im_hangul_ignore_table[i]) {
      return TRUE;
    }
  }
  return FALSE;
}

static gboolean
im_hangul_is_trigger(GdkEventKey *key)
{
  return ( key->keyval == GDK_Hangul || 
	  (key->keyval == GDK_space && (key->state & GDK_SHIFT_MASK)));
}

static gboolean
im_hangul_is_backspace(GdkEventKey *key)
{
  return (key->keyval == GDK_BackSpace);
}

static void
im_hangul_reset(GtkIMContext *context)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);

  if (im_hangul_commit(hcontext))
    g_signal_emit_by_name (hcontext, "preedit_changed");
}

static gboolean
im_hangul_process_nonhangul(GtkIMContextHangul *hcontext,
		            GdkEventKey *key)
{
  if (!im_hangul_is_modifier(key->state)) {
    gunichar ch = gdk_keyval_to_unicode(key->keyval);
    if (ch != 0) {
      gchar buf[10];
      gint n = g_unichar_to_utf8(ch, buf);
      buf[n] = '\0';
      /* g_print("Nonhangul: %s(U+%04x)\n", buf, ch); */
      g_signal_emit_by_name(hcontext, "commit", buf);
      return TRUE;
    } 
  }
  return FALSE;
}

static gboolean
im_hangul_handle_direct_mode(GtkIMContextHangul *hcontext,
		             GdkEventKey *key)
{
  if (im_hangul_is_trigger(key)) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    im_hangul_mode_hangul(hcontext);
    return TRUE;
  }
  return im_hangul_process_nonhangul(hcontext, key);
}

static gboolean
im_hangul_commit(GtkIMContextHangul *hcontext)
{
  int i;
  int n = 0;
  gchar buf[40];
 
  buf[0] = '\0';

  if (im_hangul_is_empty(hcontext))
    return FALSE;

  if (manual_mode) {
    /* we use conjoining jamo, U+1100 - U+11FF */
    if (hcontext->choseong[0] == 0)
      n += g_unichar_to_utf8(HCF, buf + n);
    else {
      for (i = 0; i <= hcontext->lindex; i++)
        n += g_unichar_to_utf8(hcontext->choseong[i], buf + n);
    }
    if (hcontext->jungseong[0] == 0)
      n += g_unichar_to_utf8(HJF, buf + n);
    else {
      for (i = 0; i <= hcontext->vindex; i++)
        n += g_unichar_to_utf8(hcontext->jungseong[i], buf + n);
    }
    if (hcontext->jongseong[0] != 0) {
      for (i = 0; i <= hcontext->tindex; i++)
        n += g_unichar_to_utf8(hcontext->jongseong[i], buf + n);
    }
    buf[n] = '\0';
  } else if (pref_use_hangul_jamo) {
    /* we use conjoining jamo, U+1100 - U+11FF */
    if (hcontext->choseong[0] == 0)
      n = g_unichar_to_utf8(HCF, buf);
    else
      n = g_unichar_to_utf8(hcontext->choseong[0], buf);
 
    if (hcontext->jungseong[0] == 0)
      n += g_unichar_to_utf8(HJF, buf + n);
    else
      n += g_unichar_to_utf8(hcontext->jungseong[0], buf + n);
 
    if (hcontext->jongseong[0] != 0)
      n += g_unichar_to_utf8(hcontext->jongseong[0], buf + n);

    buf[n] = '\0';
  } else {
    /* use hangul syllables (U+AC00 - U+D7AF)
     * and compatibility jamo (U+3130 - U+318F) */
    gunichar ch;
    ch = im_hangul_jamo_to_syllable(hcontext->choseong[0],
          			    hcontext->jungseong[0],
                                    hcontext->jongseong[0]);
 
    if (ch) {
      n = g_unichar_to_utf8(ch, buf);
      buf[n] = '\0';
      /* g_print("commit char: %s(U+%04x)\n", buf, ch); */
    } else {
      if (hcontext->choseong[0]) {
        ch = im_hangul_choseong_to_unicode(hcontext->choseong[0]);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        /* g_print("commit char: %s(U+%x)\n", buf, ch); */
      }
      if (hcontext->jungseong[0]) {
        ch = im_hangul_jungseong_to_unicode(hcontext->jungseong[0]);
        n += g_unichar_to_utf8(ch, buf + n);
        buf[n] = '\0';
        /* g_print("commit char: %s(U+%x)\n", buf, ch); */
      }
      if (hcontext->jongseong[0]) {
        ch = im_hangul_jongseong_to_unicode(hcontext->jongseong[0]);
        n += g_unichar_to_utf8(ch, buf + n);
        buf[n] = '\0';
        /* g_print("commit char: %s(U+%x)\n", buf, ch); */
      }
    }
  }

  im_hangul_clear_buf(hcontext);

  g_signal_emit_by_name(hcontext, "commit", buf);
  return TRUE;
}

static void
im_hangul_commit_utf8(GtkIMContextHangul *hcontext, gchar *utf8)
{
  gchar buf[9];

  g_return_if_fail(utf8);

  strncpy(buf, utf8, sizeof(buf));
  buf[8] = '\0';

  im_hangul_clear_buf(hcontext);

  g_signal_emit_by_name(hcontext, "commit", buf);
}

/* this is a very dangerous function:
 * safe only when GDKKEYSYMS's value is enumarated  */
static guint
im_hangul_dvorak_to_qwerty(guint code)
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

struct combination {
  guint32 key;
  gunichar code;
};

static struct combination compose_table_default[] = {
  { 0x11001100, 0x1101 }, /* choseong  kiyeok + kiyeok  = ssangkiyeok	*/
  { 0x11031103, 0x1104 }, /* choseong  tikeut + tikeut  = ssangtikeut	*/
  { 0x11071107, 0x1108 }, /* choseong  pieup  + pieup   = ssangpieup 	*/
  { 0x11091109, 0x110a }, /* choseong  sios   + sios    = ssangsios	*/
  { 0x110c110c, 0x110d }, /* choseong  cieuc  + cieuc   = ssangcieuc	*/
  { 0x11691161, 0x116a }, /* jungseong o      + a       = wa		*/
  { 0x11691162, 0x116b }, /* jungseong o      + ae      = wae		*/
  { 0x11691175, 0x116c }, /* jungseong o      + i       = oe		*/
  { 0x116e1165, 0x116f }, /* jungseong u      + eo      = weo		*/
  { 0x116e1166, 0x1170 }, /* jungseong u      + e       = we		*/
  { 0x116e1175, 0x1171 }, /* jungseong u      + i       = wi		*/
  { 0x11731175, 0x1174 }, /* jungseong eu     + i       = yi		*/
  { 0x11a811a8, 0x11a9 }, /* jongseong kiyeok + kiyeok  = ssangekiyeok 	*/
  { 0x11a811ba, 0x11aa }, /* jongseong kiyeok + sios    = kiyeok-sois	*/
  { 0x11ab11bd, 0x11ac }, /* jongseong nieun  + cieuc   = nieun-cieuc	*/
  { 0x11ab11c2, 0x11ad }, /* jongseong nieun  + hieuh   = nieun-hieuh	*/
  { 0x11af11a8, 0x11b0 }, /* jongseong rieul  + kiyeok  = rieul-kiyeok	*/
  { 0x11af11b7, 0x11b1 }, /* jongseong rieul  + mieum   = rieul-mieum 	*/
  { 0x11af11b8, 0x11b2 }, /* jongseong rieul  + pieup   = rieul-pieup	*/
  { 0x11af11ba, 0x11b3 }, /* jongseong rieul  + sios    = rieul-sios	*/
  { 0x11af11c0, 0x11b4 }, /* jongseong rieul  + thieuth = rieul-thieuth	*/
  { 0x11af11c1, 0x11b5 }, /* jongseong rieul  + phieuph = rieul-phieuph	*/
  { 0x11af11c2, 0x11b6 }, /* jongseong rieul  + hieuh   = rieul-hieuh	*/
  { 0x11b811ba, 0x11b9 }, /* jongseong pieup  + sios    = pieup-sios	*/
  { 0x11ba11ba, 0x11bb }, /* jongseong sios   + sios    = ssangsios	*/
};

static int compose_table_size = G_N_ELEMENTS(compose_table_default);
static struct combination *compose_table = compose_table_default;

static gunichar
im_hangul_compose(gunichar first, gunichar last)
{
  int min, max, mid;
  guint32 key;
 
  /* make key */
  key = first << 16 | last;

  /* binary search in table */
  min = 0;
  max = compose_table_size - 1;

  while (max >= min) {
    mid = (min + max) / 2;
    if (compose_table[mid].key < key)
      min = mid + 1;
    else if (compose_table[mid].key > key)
      max = mid - 1;
    else
      return compose_table[mid].code;
  }
  return 0;
}

static gunichar
im_hangul_mapping(guint keyval, guint state)
{
  if (keyboard_table == NULL)
    return 0;

  /* treat for dvorak */
  if (pref_use_dvorak)
    keyval = im_hangul_dvorak_to_qwerty(keyval);

  /* hangul jamo keysym */
  if (keyval >= 0x01001100 && keyval <= 0x010011ff)
    return keyval & 0x0000ffff;

  if (keyval >= GDK_exclam && keyval <= GDK_asciitilde) {
    /* treat capslock, as capslock is not on */
    if (state & GDK_LOCK_MASK) {
      if (state & GDK_SHIFT_MASK) {
        if (keyval >= GDK_a && keyval <= GDK_z)
          keyval -= (GDK_a - GDK_A);
      } else {
        if (keyval >= GDK_A && keyval <= GDK_Z)
          keyval += (GDK_a - GDK_A);
      }
    }
    return keyboard_table[keyval - GDK_exclam];
  }

  return 0;
}

/* use hangul automata */
static gboolean
im_hangul_filter_keypress(GtkIMContext *context, GdkEventKey *key)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);

  /* ignore key release */
  if (key->type == GDK_KEY_RELEASE)
    return FALSE;

  /* we silently ignore shift keys */
  if (key->keyval == GDK_Shift_L || key->keyval == GDK_Shift_R)
    return FALSE;

  /* on Ctrl-Hangul we turn on/off manual_mode */
  if (key->keyval == GDK_Hangul && (key->state & GDK_CONTROL_MASK))
    manual_mode = !manual_mode;

  /* on capslock, we use Hangul Jamo */
  if (pref_use_caps_lock && key->keyval == GDK_Caps_Lock)
    pref_use_hangul_jamo = !pref_use_hangul_jamo;

  /* some keys are ignored: Ctrl, Alt, Meta */
  /* we flush out all preedit text */
  if (im_hangul_is_ignore_key(key->keyval)) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    return FALSE;
  }

  /* handle direct mode */
  if (hcontext->state == STATE_DIRECT) {
    return im_hangul_handle_direct_mode(hcontext, key);
  }

  /* handle Escape key: automaticaly change to direct mode */
  if (key->keyval == GDK_Escape) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    im_hangul_mode_direct(hcontext);
    return FALSE;
  }

  /* modifiler key */
  if (im_hangul_is_modifier(key->state)) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    return FALSE;
  }

  /* hanja key */
  if (key->keyval == GDK_F9 || key->keyval == GDK_Hangul_Hanja) {
    popup_hanja_window(hcontext);
    return TRUE;
  }

  /* char table */
  if (key->keyval == GDK_F3 || (
      key->keyval == GDK_Hangul_Hanja && (key->state & GDK_CONTROL_MASK))) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    popup_char_table_window(hcontext);
    return TRUE;
  }

  /* trigger key: mode change to direct mode */
  if (im_hangul_is_trigger(key)) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name(hcontext, "preedit_changed");
    im_hangul_mode_direct(hcontext);
    return TRUE;
  }

  /* here we must hangul mode, so set STATE_HANGUL
   * static variable input_mode is not yet applied so we change it
   * below line must not removed */
  if (hcontext->state == STATE_DIRECT) {
    hcontext->state = STATE_HANGUL;
    g_print("This is really a error: our input mode is currupted\n");
  }

  if (hcontext->automata)
    return hcontext->automata(hcontext, key);
  else {
    g_print("imhangul: null automata\n");
    return FALSE;
  }
}


/*
 * status window
 */
static gboolean
status_window_expose_event(GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_rectangle (widget->window,
		      widget->style->fg_gc[GTK_STATE_NORMAL],
		      FALSE,
		      0, 0,
		      widget->allocation.width-1, widget->allocation.height-1);

  return FALSE;
}

static void
status_window_free(StatusWindow *status_window)
{
  status_windows = g_slist_remove(status_windows, status_window);
  
  g_signal_handler_disconnect(status_window->toplevel,
			      status_window->destroy_handler_id);
  g_signal_handler_disconnect(status_window->toplevel,
			      status_window->configure_handler_id);
  gtk_widget_destroy(status_window->window);
  g_object_set_data(G_OBJECT(status_window->toplevel),
  		    "gtk-imhangul-status-window", NULL);
  g_free(status_window);
}

static gboolean
status_window_configure	(GtkWidget *toplevel,
			 GdkEventConfigure *event,
			 GtkWidget *window)
{
  GdkRectangle rect;
  GtkRequisition requisition;
  gint y;

  gdk_window_get_frame_extents(toplevel->window, &rect);
  gtk_widget_size_request(window, &requisition);

  if (rect.y + rect.height + requisition.height < gdk_screen_height())
    y = rect.y + rect.height;
  else
    y = gdk_screen_height() - requisition.height;

  gtk_window_move(GTK_WINDOW(window), rect.x, y);
  return FALSE;
}

static gboolean
on_click_hangul (GtkWidget *widget,
		 GdkEventButton *event,
		 gpointer data)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(data);

  if (hcontext->state == STATE_DIRECT) {
    /* english mode change to hangul mode */
    im_hangul_mode_hangul(hcontext);
  } else {
    /* hangul mode change to english mode */
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    im_hangul_mode_direct(hcontext);
  }
  return TRUE;
}

static gboolean
on_click_hanja (GtkWidget *widget,
		 GdkEventButton *event,
		 gpointer data)
{
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(data);

  popup_hanja_window(hcontext);

  return TRUE;
}

static GtkWidget *
get_toplevel_window(GdkWindow *window)
{
  GtkWidget *toplevel;
  GdkWindow *parent;

  if (window == NULL)
    return NULL;

  while (TRUE) {
    parent = gdk_window_get_parent(window);
    if (parent == gdk_get_default_root_window())
      break;
    else
      window = parent;
  }

  gdk_window_get_user_data(window, (gpointer *)&toplevel);
  return toplevel;
}

static StatusWindow*
status_window_get(GtkIMContextHangul *hcontext)
{
  if (hcontext->toplevel == NULL)
    return NULL;

  return g_object_get_data (G_OBJECT(hcontext->toplevel),
  			    "gtk-imhangul-status-window");
}

static GtkWidget*
status_window_get_window(GtkIMContextHangul *hcontext, gboolean create)
{
  GtkWidget *toplevel;
  GtkWidget *window;
  GtkWidget *hbox;
  GtkWidget *ebox;
  GtkWidget *frame;
  GtkWidget *label;
  StatusWindow *status_window;

  if (!pref_use_status_window)
    return NULL;

  toplevel = hcontext->toplevel;
  if (toplevel == NULL)
    return NULL;

  status_window = status_window_get (hcontext);
  if (status_window)
    return status_window->window;
  else if (!create)
    return NULL;

  status_window = g_new(StatusWindow, 1);
  status_window->window = gtk_window_new(GTK_WINDOW_POPUP);
  status_window->toplevel = toplevel;

  status_windows = g_slist_prepend(status_windows, status_window);

  window = status_window->window;

  gtk_container_set_border_width(GTK_CONTAINER(window), 1);
  /* gtk_window_set_decorated(GTK_WINDOW(window), FALSE); */
  gtk_widget_set_name(window, "imhangul_status");
  gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
  gtk_widget_set_app_paintable(window, TRUE);

  frame = gtk_frame_new(NULL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);
  gtk_widget_show(frame);
  gtk_container_add(GTK_CONTAINER(window), frame);

  hbox = gtk_hbox_new(TRUE, 3);
  gtk_widget_show(hbox);
  gtk_container_add(GTK_CONTAINER(frame), hbox);

  /* yeongeo/hangul label */
  label = gtk_label_new(""); 
  status_window->hangul_label = label;
  gtk_widget_show(label);
  ebox = gtk_event_box_new();
  gtk_widget_show(ebox);
  gtk_container_add(GTK_CONTAINER(ebox), label);
  gtk_box_pack_start(GTK_BOX(hbox), ebox, TRUE, TRUE, 0);
  g_signal_connect(G_OBJECT(ebox), "button-press-event",
  		   G_CALLBACK(on_click_hangul), hcontext);

  /* hanja label */
  label = gtk_label_new("[\355\225\234\354\236\220]");
  gtk_widget_show(label);
  ebox = gtk_event_box_new();
  gtk_widget_show(ebox);
  gtk_container_add(GTK_CONTAINER(ebox), label);
  gtk_box_pack_start(GTK_BOX(hbox), ebox, TRUE, TRUE, 0);
  g_signal_connect(G_OBJECT(ebox), "button-press-event",
  		   G_CALLBACK(on_click_hanja), hcontext);

  status_window->destroy_handler_id =
			g_signal_connect_swapped(G_OBJECT(toplevel), "destroy",
					       G_CALLBACK(status_window_free),
					       status_window);
  status_window->configure_handler_id = 
			g_signal_connect(G_OBJECT(toplevel), "configure-event",
					 G_CALLBACK(status_window_configure),
					 window);
  status_window_configure(toplevel, NULL, window);

  g_signal_connect(G_OBJECT(window), "expose-event",
		   G_CALLBACK(status_window_expose_event), NULL);

  status_window_set_label(hcontext);
  g_object_set_data(G_OBJECT(toplevel),
  		    "gtk-imhangul-status-window", status_window);

  return window;
}

static void
status_window_show(GtkIMContextHangul *hcontext)
{
  GtkWidget *window = status_window_get_window (hcontext, TRUE);

  if (window) {
    status_window_set_label(hcontext);
    gtk_widget_show(window);
  }
}

static void
status_window_hide(GtkIMContextHangul *hcontext)
{
  GtkWidget *window = status_window_get_window (hcontext, FALSE);

  if (window) {
    gtk_widget_hide(window);
  }
}

static void
status_window_set_label(GtkIMContextHangul *hcontext)
{
  static const gchar yeongeo[] = { 
	'[', 0xec, 0x98, 0x81, 0xec, 0x96, 0xb4, ']', 0	/* utf8 string */
  };
  static const gchar hangul[] = { 
	'[', 0xed, 0x95, 0x9c, 0xea, 0xb8, 0x80, ']', 0 /* utf8 string */
  };

  GtkWidget *label;
  StatusWindow *status_window = status_window_get(hcontext);

  if (status_window == NULL)
    return;

  label = status_window->hangul_label;
  if (label) {
    if (hcontext->state == STATE_DIRECT)
      gtk_label_set_text(GTK_LABEL(label), yeongeo);
    else
      gtk_label_set_text(GTK_LABEL(label), hangul);
  }
}


/*
 * Hanja selection window
 */
static gint
get_index_of_hanjatable(gunichar ch)
{
  guint first, last, mid;

  /* binary search */
  first = 0;
  last = G_N_ELEMENTS(hanjatable) - 1;
  while (first <= last) {
    mid = (first + last) / 2;
    if (ch == hanjatable[mid][0])
      return mid;
    if (ch < hanjatable[mid][0]) {
      last = mid - 1;
    } else {
      first = mid + 1;
    }
  }
  return -1;
}

static gboolean
on_hanja_window_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  if (event->keyval == GDK_Escape) {
    gtk_widget_destroy(hanja_window);
    return TRUE;
  }
  return FALSE;
}

static void
on_hanja_window_destroy(GtkWidget *widget, gpointer data)
{
  gtk_grab_remove(widget);
  hanja_window = NULL;
  input_mode = INPUT_MODE_HANGUL;
}

static void
on_hanja_button_clicked(GtkButton *button, gpointer data)
{
  GtkIMContextHangul* hcontext = GTK_IM_CONTEXT_HANGUL(data);
  gchar *str = (gchar *)gtk_button_get_label(GTK_BUTTON(button));

  if (str) {
    im_hangul_commit_utf8(hcontext, str);
    hcontext->state = STATE_HANGUL;
    hcontext->index = -1;
    g_signal_emit_by_name (hcontext, "preedit_changed");
  }
  gtk_widget_destroy(hanja_window);
}

static GtkWidget *
create_hanja_window(GtkIMContextHangul *hcontext, gunichar ch)
{
  gunichar *p;
  gint x, y, n, index;
  GtkWidget *window, *table, *button, *label, *parent;
  PangoFontDescription *desc = NULL;
  PangoAttrList *attrs = NULL;
  PangoAttribute *attr = NULL;
  gchar buf[6];
  gulong id;

  index = get_index_of_hanjatable(ch);

  /* g_print("index: %d\n", index); */
  if (index < 0) /* there is no such hanja */
    return NULL;

  if (hanja_window != NULL)
    return NULL;

  hanja_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  window = hanja_window;
  table = gtk_table_new(1, 10, TRUE);

  if (pref_hanja_font)
    desc = pango_font_description_from_string(pref_hanja_font);

  x = 0;
  y = 0;
  p = hanjatable[index] + 1;
  while (*p != 0) {
    n = g_unichar_to_utf8(*p, buf);
    buf[n] = 0;

    button = gtk_button_new_with_label(buf);
    gtk_widget_set_name(button, "imhangul_hanja");
    label = GTK_BIN(button)->child;
    if (desc) {
      gtk_widget_modify_font(label, desc);
    } else {
      attrs = pango_attr_list_new();
      attr = pango_attr_scale_new(PANGO_SCALE_XX_LARGE);
      attr->start_index = 0;
      attr->end_index = n;
      pango_attr_list_insert(attrs, attr);
      gtk_label_set_attributes(GTK_LABEL(label), attrs);
    }

    gtk_table_attach(GTK_TABLE(table), button, x, x + 1, y, y + 1,
            (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
            (GtkAttachOptions) (GTK_EXPAND | GTK_SHRINK | GTK_FILL),
            0, 0);
    id = g_signal_connect(G_OBJECT(button), "clicked",
			  G_CALLBACK (on_hanja_button_clicked), hcontext);

    x++;
    if (x > 9) {
      y++;
      x = 0;
    }
    p++;
  }
  gtk_container_add(GTK_CONTAINER(window), table);

  g_signal_connect (G_OBJECT(window), "key-press-event",
		    G_CALLBACK (on_hanja_window_keypress), NULL);
  g_signal_connect (G_OBJECT(window), "destroy",
		    G_CALLBACK (on_hanja_window_destroy), NULL);

  parent = hcontext->toplevel;
  if (parent)
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));

  gtk_grab_add(window);
  gtk_widget_show_all(window);

  pango_font_description_free(desc);

  return window;
}

static void
popup_hanja_window(GtkIMContextHangul *hcontext)
{
  gunichar ch;

  if (hcontext->choseong[0] == 0 || hcontext->jungseong[0] == 0)
    return; 

  ch = im_hangul_jamo_to_syllable(hcontext->choseong[0],
				  hcontext->jungseong[0],
	                          hcontext->jongseong[0]);
  if (ch)
    create_hanja_window(hcontext, ch);
}

static gboolean
on_char_table_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  if (event->keyval == GDK_Escape) {
    gtk_widget_hide(widget);
    return TRUE;
  }
  return FALSE;
}

static void
on_char_table_clicked(GtkWidget *widget, gpointer data)
{
  gchar *str = (gchar *)gtk_button_get_label(GTK_BUTTON(widget));

  if (current_context != NULL && str != NULL) {
    im_hangul_commit_utf8(current_context, str);
  }
}

#include "chartable.h"

static GtkWidget*
create_char_window(GtkIMContextHangul *hcontext)
{
  GtkWidget *window;
  GtkWidget* notebook;
  GtkWidget* table;
  GtkWidget* label;
  GtkWidget* button;
  GtkWidget* parent;
  gchar buf[8];
  gint i, j, x, y, n;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  notebook = gtk_notebook_new();
  gtk_container_add(GTK_CONTAINER(window), notebook);
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
  gtk_widget_show(notebook);

  i = 0;
  while (char_table[i].name != NULL) {
    label = gtk_label_new(char_table[i].name);
    gtk_widget_show(label);

    x = 0;
    y = 0;
    table = gtk_table_new(1, 16, TRUE);
    gtk_widget_show(table);
    for (j = 0; char_table[i].list[j] != 0; j++) {
      n = g_unichar_to_utf8(char_table[i].list[j], buf);
      buf[n] = 0;
      button = gtk_button_new_with_label(buf);
      gtk_widget_show(button);
      gtk_table_attach(GTK_TABLE(table), button, x, x + 1, y, y + 1,
		       (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		       (GtkAttachOptions) (GTK_FILL),
		       0, 0);
      g_signal_connect(G_OBJECT(button), "clicked",
		       G_CALLBACK (on_char_table_clicked), NULL);
      x++;
      if (x >= 16) {
        x = 0;
	y++;
      }
    }

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table, label);
    i++;
  }

  g_signal_connect (G_OBJECT(window), "key-press-event",
		    G_CALLBACK (on_char_table_keypress), NULL);
  g_signal_connect (G_OBJECT(window), "delete_event",
		    G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  parent = hcontext->toplevel;
  if (parent)
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));

  return window;
}

static void
popup_char_table_window(GtkIMContextHangul *hcontext)
{
  if (char_table_window != NULL) {
    gtk_widget_show(char_table_window);
  } else {
    char_table_window = create_char_window(hcontext);
    gtk_widget_show(char_table_window);
  }
}

/*
 * im_hangul_shutdown:
 *
 * Destroys all the status windows that are kept by the hangul contexts.
 */
void
im_hangul_shutdown(void)
{
  /* remove status window */
  while (status_windows)
    status_window_free(status_windows->data);

  /* remove hanja selection dialog */
  if (hanja_window) {
    gtk_widget_destroy(hanja_window);
    hanja_window = NULL;
  }

  /* remove character selection dialog */
  if (char_table_window != NULL) {
    gtk_widget_destroy(char_table_window);
    char_table_window = NULL;
  }

  im_hangul_set_input_mode_info (INPUT_MODE_INFO_NONE);
  g_print("im-hangul module shutdown\n");
}

#endif /* __GTK_IM_CONTEXT_HANGUL_H__ */

/* vim: set cindent sw=2 : */

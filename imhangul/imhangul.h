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
#include "gtk/gtklabel.h"
#include "gtk/gtksignal.h"
#include "gtk/gtkwindow.h"
#include "gtk/gtkintl.h"
#include "gtk/gtkimcontext.h"
*/

#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>
#include <gtk/gtkbutton.h>
#include <gtk/gtklabel.h>
#include <gtk/gtktable.h>
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
  gunichar stack[10];
  int index;			/* stack index */
  gunichar choseong;
  gunichar jungseong;
  gunichar jongseong;
  GtkIMContextHangulAutomata automata;
  GdkWindow *client_window;
};

struct _GtkIMContextHangulClass
{
  GtkIMContextClass parent_class;
};

/* Status window: mostly copied from gtkimcontextxim.c */
struct _StatusWindow
{
  GtkWidget *window;
  GtkWidget *toplevel;
  guint destroy_handler_id;
  guint configure_handler_id;
};

typedef struct _StatusWindow StatusWindow;

static void im_hangul_register_type (GTypeModule *type_module,
				     const gchar *type_name);

static GtkIMContext *im_hangul_new(void);
static void im_hangul_shutdown(void);
static void im_hangul_set_automata(GtkIMContextHangul *context_hangul,
				   GtkIMContextHangulAutomata automata);

/* asistant function for hangul automata */
#define is_set(state, mask)		((state & mask) == mask)
#define im_hangul_is_modifier(state)	(is_set(state, GDK_CONTROL_MASK) || \
					 is_set(state, GDK_MOD1_MASK))
#define im_hangul_is_choseong(ch)	((ch) >= 0x1100 && (ch) <= 0x1112)
#define im_hangul_is_jungseong(ch)	((ch) >= 0x1161 && (ch) <= 0x1175)
#define im_hangul_is_jongseong(ch)	((ch) >= 0x11A7 && (ch) <= 0x11C2)
#define im_hangul_is_empty(ctx)		((ctx)->choseong  == 0 &&	\
					 (ctx)->jungseong == 0 &&	\
					 (ctx)->jongseong == 0 )

static gboolean	im_hangul_is_trigger			(GdkEventKey *key);
static gboolean	im_hangul_is_backspace			(GdkEventKey *key);
static gunichar	im_hangul_compchoseong_to_single	(gunichar ch);
static gunichar	im_hangul_compjungseong_to_single	(gunichar ch);
static gunichar	im_hangul_compjongseong_to_single	(gunichar ch);
static gunichar	im_hangul_pop	(GtkIMContextHangul *context_hangul);
static gunichar	im_hangul_peek	(GtkIMContextHangul *context_hangul);
static void	im_hangul_push	(GtkIMContextHangul *context_hangul, gunichar ch);
static void	im_hangul_mode_hangul(GtkIMContextHangul *context_hangul);
static void	im_hangul_mode_direct(GtkIMContextHangul *context_hangul);

/* commit function */
static void	im_hangul_commit	(GtkIMContextHangul *context_hangul);
static void	im_hangul_commit_unicode(GtkIMContextHangul *context_hangul,
					 gunichar ch);
static void	im_hangul_commit_utf8	(GtkIMContextHangul *context_hangul,
					 gchar *utf8);
static gboolean im_hangul_process_nonhangul(GtkIMContextHangul *context_hangul,
						 GdkEventKey *key);
static void im_hangul_class_init	(GtkIMContextHangulClass *class);
static void im_hangul_init		(GtkIMContextHangul *context_hangul);
static void im_hangul_finalize		(GObject *obj);

static gboolean	im_hangul_filter_keypress(GtkIMContext *context,
					      GdkEventKey *key);
static void im_hangul_get_preedit_string(GtkIMContext *ic,
					     gchar **str,
					     PangoAttrList **attrs,
					     gint *cursor_pos);
static void im_hangul_focus_in	(GtkIMContext *context);
static void im_hangul_focus_out	(GtkIMContext *context);


static void im_hangul_set_client_window	(GtkIMContext *context,
				         GdkWindow *client_window);
static void status_window_show     (GtkIMContextHangul *context_hangul);
static void status_window_hide     (GtkIMContextHangul *context_hangul);
static void status_window_set_mode (GtkIMContextHangul *context_hangul);

static void popup_hanja_window	   (GtkIMContextHangul *context_hangul);

/*
 * global variables for hangul immodule
 */
#define STATE_DIRECT -1 
#define STATE_HANGUL 0 
static GObjectClass *parent_class;

static StatusWindow *status_window = NULL;
static GtkWidget *hanja_window = NULL;

#define MODE_HANJA	-2
#define MODE_DIRECT	-1
#define MODE_HANGUL	 0
static gint		input_mode = MODE_DIRECT;

/* preferences */
static gboolean		pref_use_status_window = TRUE;
static gboolean		pref_use_dvorak = FALSE;
static gchar* 		pref_hanja_font = NULL;

static guint16 gtk_compose_ignore[] = {
  GDK_Shift_L,
  GDK_Shift_R,
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

static void 
im_hangul_class_init (GtkIMContextHangulClass *class)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS(class);
  GObjectClass *gobject_class = G_OBJECT_CLASS(class);

  parent_class = g_type_class_peek_parent (class);

  im_context_class->set_client_window = im_hangul_set_client_window;
  im_context_class->filter_keypress = im_hangul_filter_keypress;
  im_context_class->focus_in = im_hangul_focus_in;
  im_context_class->focus_out = im_hangul_focus_out;
  im_context_class->get_preedit_string = im_hangul_get_preedit_string;
  gobject_class->finalize = im_hangul_finalize;
}

static void 
im_hangul_init (GtkIMContextHangul *context_hangul)
{
  context_hangul->state = input_mode;	/* direct mode */
  context_hangul->stack[0] = 0;
  context_hangul->stack[1] = 0;
  context_hangul->stack[2] = 0;
  context_hangul->stack[3] = 0;
  context_hangul->stack[4] = 0;
  context_hangul->stack[5] = 0;
  context_hangul->stack[6] = 0;
  context_hangul->stack[7] = 0;
  context_hangul->stack[8] = 0;
  context_hangul->stack[9] = 0;
  context_hangul->index = -1;

  context_hangul->choseong = 0;
  context_hangul->jungseong = 0;
  context_hangul->jongseong = 0;
  context_hangul->automata = NULL;	/* initial value: automata == null */
  context_hangul->client_window = NULL;
}

static void
im_hangul_finalize(GObject *obj)
{
  parent_class->finalize (obj);
}

static void
im_hangul_set_client_window(GtkIMContext *context,
			    GdkWindow *client_window)
{
  GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);

  context_hangul->client_window = client_window;
}

static GtkIMContext *
im_hangul_new(void)
{
  return GTK_IM_CONTEXT (g_object_new (GTK_TYPE_IM_CONTEXT_HANGUL, NULL));
}

static void
im_hangul_push(GtkIMContextHangul *context_hangul, gunichar ch)
{
  context_hangul->stack[++context_hangul->index] = ch;
}

static gunichar
im_hangul_peek(GtkIMContextHangul *context_hangul)
{
  if (context_hangul->index < 0)
    return 0;
  return context_hangul->stack[context_hangul->index];
}

static gunichar
im_hangul_pop(GtkIMContextHangul *context_hangul)
{
  if (context_hangul->index < 0)
    return 0;
  return context_hangul->stack[context_hangul->index--];
}

static void
im_hangul_set_automata(GtkIMContextHangul *context_hangul,
		       GtkIMContextHangulAutomata automata)
{
  context_hangul->automata = automata;
}

static void 
im_hangul_mode_hangul(GtkIMContextHangul *context_hangul)
{
  input_mode = MODE_HANGUL;
  context_hangul->state = STATE_HANGUL;
  status_window_set_mode(context_hangul);
}

static void 
im_hangul_mode_direct(GtkIMContextHangul *context_hangul)
{
  input_mode = MODE_DIRECT;
  context_hangul->state = STATE_DIRECT;
  status_window_set_mode(context_hangul);
}

/* this funcs used for backspace */
static gunichar
im_hangul_compchoseong_to_single(gunichar ch)
{
  switch (ch) {
    case 0x1101:	/* hangul choseong ssangkiyeok */
      return 0x1100;	/* hangul choseong kiyeok */
    case 0x1104:	/* hangul choseong ssangtikeut */
      return 0x1103;	/* hangul choseong tikeut */
    case 0x1108:	/* hangul choseong ssangpieup */
      return 0x1107;	/* hangul choseong pieup */
    case 0x110a:	/* hangul choseong ssangsios */
      return 0x1109;	/* hangul choseong sios */
    case 0x110d:	/* hangul choseong ssangcieuc */
      return 0x110c;	/* hangul choseong cieuc */
  }
  return 0;
}

static gunichar
im_hangul_compjungseong_to_single(gunichar ch)
{
  switch (ch) {
    case 0x116a:	/* hangul jungseong wa */
    case 0x116b:	/* hangul jungseong wae */
    case 0x116c:	/* hangul jungseong oe */
      return 0x1169;	/* hangul jungseong o */
    case 0x116f:	/* hangul jungseong weo */
    case 0x1170:	/* hangul jungseong we */
    case 0x1171:	/* hangul jungseong wi */
      return 0x116e;	/* hangul jungseong u */
    case 0x1174:	/* hangul jungseong yi */
      return 0x1173;	/* hangul jungseong eu */
  }
  return 0;
}

static gunichar
im_hangul_compjongseong_to_single(gunichar ch)
{
  switch (ch) {
    case 0x11a9:	/* hangul jongseong ssangkiyeok */
    case 0x11aa:	/* hangul jongseong kiyeok-sios */
      return 0x11a8;	/* hangul jongseong kiyeok */
    case 0x11ac:	/* hangul jongseong nieun-cieuc */
    case 0x11ad:	/* hangul jongseong nieun-hieuh */
      return 0x11ab;	/* hangul jongseong nieun */
    case 0x11b0:	/* hangul jongseong rieul-kiyeok */
    case 0x11b1:	/* hangul jongseong rieul-mieum */
    case 0x11b2:	/* hangul jongseong rieul-pieup */
    case 0x11b3:	/* hangul jongseong rieul-sios */
    case 0x11b4:	/* hangul jongseong rieul-thieuth */
    case 0x11b5:	/* hangul jongseong rieul-phieuph */
    case 0x11b6:	/* hangul jongseong rieul-hieuh */
      return 0x11af;	/* hangul jongseong rieul */
    case 0x11b9:	/* hangul jongseong ssangsios */
      return 0x11b8;	/* hangul jongseong sios */
  }
  return 0;
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
  if (!im_hangul_is_choseong(ch))
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

  if (!im_hangul_is_jungseong(ch))
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
  if (!im_hangul_is_jongseong(ch))
    return 0;
  return table[ch - 0x11a8];
}

static gunichar
im_hangul_jamo_to_unicode(gunichar choseong,
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

  if (!im_hangul_is_choseong(choseong))
    return 0;
  if (!im_hangul_is_jungseong(jungseong))
    return 0;
  if (!im_hangul_is_jongseong(jongseong))
    return 0;

  choseong -= choseong_base;
  jungseong -= jungseong_base;
  jongseong -= jongseong_base;
 
  ch = ((choseong * njungseong) + jungseong) * njongseong + jongseong
	 + hangul_base;
  if (ch >= 0xAC00 && ch <= 0xD7AF)  /* check hangul code */
    return ch;
  else
    return 0;
}

#define HCF	0x115f
#define HJF	0x1160

static int
im_hangul_preedit_string(GtkIMContextHangul *context_hangul,
				     gchar *buf)
{
  int n = 0;
  gunichar ch;

  if (context_hangul->choseong == 0 && context_hangul->jungseong == 0 &&
      context_hangul->jongseong == 0) {
    buf[0] = '\0';
    return 0;
  }

  /* this code is very stupid but pango has some bug in Xft
   * therefore we have to do this */
  if (context_hangul->choseong) {
    if (context_hangul->jungseong) {
      if (context_hangul->jongseong) {
	/* have cho, jung, jong */
	ch = im_hangul_jamo_to_unicode(context_hangul->choseong,
				       context_hangul->jungseong,
				       context_hangul->jongseong);
	n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      } else {
	/* have cho, jung */
	ch = im_hangul_jamo_to_unicode(context_hangul->choseong,
				       context_hangul->jungseong,
				       0x11A7);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      }
    } else {
      if (context_hangul->jongseong) {
	/* have cho, jong */
        ch = im_hangul_choseong_to_unicode(context_hangul->choseong);
        n = g_unichar_to_utf8(ch, buf);
        ch = im_hangul_jongseong_to_unicode(context_hangul->jongseong);
        n += g_unichar_to_utf8(ch, buf + n);
        buf[n] = '\0';
        return n;
      } else {
	/* have cho */
        ch = im_hangul_choseong_to_unicode(context_hangul->choseong);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      }
    }
  } else {
    if (context_hangul->jungseong) {
      if (context_hangul->jongseong) {
	/* have jung, jong */
        ch = im_hangul_jungseong_to_unicode(context_hangul->jungseong);
        n = g_unichar_to_utf8(ch, buf);
        ch = im_hangul_jongseong_to_unicode(context_hangul->jongseong);
        n += g_unichar_to_utf8(ch, buf + n);
        buf[n] = '\0';
        return n;
      } else {
	/* have jung */
        ch = im_hangul_jungseong_to_unicode(context_hangul->jungseong);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      }
    } else {
      if (context_hangul->jongseong) {
	/* have jong */
        ch = im_hangul_jongseong_to_unicode(context_hangul->jongseong);
        n = g_unichar_to_utf8(ch, buf);
        buf[n] = '\0';
        return n;
      } else {
	/* have nothing */
	;
      }
    }
  }

  /* we use conjoining jamo, U+1100 - U+11FF */
  if (context_hangul->choseong == 0)
    n = g_unichar_to_utf8(HCF, buf);
  else
    n = g_unichar_to_utf8(context_hangul->choseong, buf);
  buf[n] = '\0';

  if (context_hangul->jungseong == 0)
    n += g_unichar_to_utf8(HJF, buf + n);
  else
    n += g_unichar_to_utf8(context_hangul->jungseong, buf + n);
  buf[n] = '\0';

  if (context_hangul->jongseong != 0) {
    n += g_unichar_to_utf8(context_hangul->jongseong, buf + n);
    buf[n] = '\0';
  }

/*
  g_print("PreEdit: U+%04x U+%04x U+%04x (%d)\n", context_hangul->choseong,
		 			    context_hangul->jungseong,
					    context_hangul->jongseong, n);
*/
  return n;
}

static void
im_hangul_get_preedit_string(GtkIMContext *context, gchar **str,
			     PangoAttrList **attrs,
			     gint *cursor_pos)
{
  gchar buf[24];
  int len = 0;
  GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);

  buf[0] = '\0';
  len = im_hangul_preedit_string(context_hangul, buf);

  if (attrs) {
    PangoAttribute *attr;
    *attrs = pango_attr_list_new();
    attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
    attr->start_index = 0;
    attr->end_index = len;
    pango_attr_list_insert (*attrs, attr);
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
  GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);

  status_window_show(context_hangul);
}

static void
im_hangul_focus_out(GtkIMContext *context)
{
  GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);

  if (hanja_window == NULL && 
      (context_hangul->choseong != 0 || context_hangul->jungseong != 0 ||
       context_hangul->jongseong != 0)) {
    im_hangul_commit(context_hangul);
    context_hangul->state = MODE_HANGUL;
    g_signal_emit_by_name (context_hangul, "preedit_changed");
  }
  status_window_hide(context_hangul);
}

static gboolean
im_hangul_is_ignore_key(guint16 key)
{
  int i;
  for (i = 0; gtk_compose_ignore[i]; i++) {
    if (key == gtk_compose_ignore[i]) {
      return TRUE;
    }
  }
  return FALSE;
}

static gboolean
im_hangul_is_trigger(GdkEventKey *key)
{
  return ( key->keyval == GDK_Hangul || 
	  (key->keyval == GDK_space && is_set(key->state, GDK_SHIFT_MASK)));
}

static gboolean
im_hangul_is_backspace(GdkEventKey *key)
{
  return (key->keyval == GDK_BackSpace);
}

static void
im_hangul_reset(GtkIMContextHangul *context_hangul)
{
  context_hangul->index = -1;
  context_hangul->stack[0] = 0;
  context_hangul->stack[1] = 0;
  context_hangul->stack[2] = 0;
  context_hangul->stack[3] = 0;
  context_hangul->stack[4] = 0;
  context_hangul->stack[5] = 0;
  context_hangul->stack[6] = 0;
  context_hangul->stack[7] = 0;
  context_hangul->stack[8] = 0;
  context_hangul->stack[9] = 0;
  context_hangul->choseong = 0;
  context_hangul->jungseong = 0;
  context_hangul->jongseong = 0;
  context_hangul->state = STATE_DIRECT;		/* english mode */
}

static gboolean
im_hangul_process_nonhangul(GtkIMContextHangul *context_hangul,
		            GdkEventKey *key)
{
  if (!im_hangul_is_modifier(key->state)) {
    gunichar ch = gdk_keyval_to_unicode(key->keyval);
    if (ch != 0) {
      gchar buf[10];
      gint n = g_unichar_to_utf8(ch, buf);
      buf[n] = '\0';
      /* g_print("Nonhangul: %s(U+%04x)\n", buf, ch); */
      g_signal_emit_by_name(context_hangul, "commit", buf);
      return TRUE;
    } 
  }
  return FALSE;
}

static gboolean
im_hangul_handle_direct_mode(GtkIMContextHangul *context_hangul,
		             GdkEventKey *key)
{
  if (im_hangul_is_trigger(key)) {
    im_hangul_commit(context_hangul);
    im_hangul_reset(context_hangul);
    im_hangul_mode_hangul(context_hangul);
    g_signal_emit_by_name (context_hangul, "preedit_changed");
    return TRUE;
  }
  return im_hangul_process_nonhangul(context_hangul, key);
}

static void
im_hangul_commit(GtkIMContextHangul *context_hangul)
{
  int n = 0;
  gchar buf[12];
  gunichar ch;
 
  ch = im_hangul_jamo_to_unicode(context_hangul->choseong,
				 context_hangul->jungseong,
	(context_hangul->jongseong)? context_hangul->jongseong : 0x11a7);

  buf[0] = '\0';
  if (ch) {
    n = g_unichar_to_utf8(ch, buf);
    buf[n] = '\0';
    /* g_print("commit char: %s(U+%04x)\n", buf, ch); */
  } else {
    if (context_hangul->choseong) {
      ch = im_hangul_choseong_to_unicode(context_hangul->choseong);
      n = g_unichar_to_utf8(ch, buf);
      buf[n] = '\0';
      /* g_print("commit char: %s(U+%x)\n", buf, ch); */
    }
    if (context_hangul->jungseong) {
      ch = im_hangul_jungseong_to_unicode(context_hangul->jungseong);
      n += g_unichar_to_utf8(ch, buf + n);
      buf[n] = '\0';
      /* g_print("commit char: %s(U+%x)\n", buf, ch); */
    }
    if (context_hangul->jongseong) {
      ch = im_hangul_jongseong_to_unicode(context_hangul->jongseong);
      n += g_unichar_to_utf8(ch, buf + n);
      buf[n] = '\0';
      /* g_print("commit char: %s(U+%x)\n", buf, ch); */
    }
  }

  context_hangul->choseong = 0;
  context_hangul->jungseong = 0;
  context_hangul->jongseong = 0;
  context_hangul->index = -1;

  g_signal_emit_by_name(context_hangul, "commit", buf);
}

static void
im_hangul_commit_unicode(GtkIMContextHangul *context_hangul, gunichar ch)
{
  int n;
  gchar buf[6];

  n = g_unichar_to_utf8(ch, buf);
  buf[n] = '\0';

  context_hangul->choseong = 0;
  context_hangul->jungseong = 0;
  context_hangul->jongseong = 0;
  context_hangul->index = -1;

  g_signal_emit_by_name(context_hangul, "commit", buf);
}

static void
im_hangul_commit_utf8(GtkIMContextHangul *context_hangul, gchar *utf8)
{
  gchar buf[9];

  g_return_if_fail(utf8);

  strncpy(buf, utf8, sizeof(buf));
  buf[8] = '\0';

  context_hangul->choseong = 0;
  context_hangul->jungseong = 0;
  context_hangul->jongseong = 0;
  context_hangul->index = -1;

  g_signal_emit_by_name(context_hangul, "commit", buf);
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

/* use hangul automata */
static gboolean
im_hangul_filter_keypress(GtkIMContext *context, GdkEventKey *key)
{
  GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);

  /* ignore key release */
  if (key->type == GDK_KEY_RELEASE) {
    return FALSE;
  }
  /* some keys are ignored */
  if (im_hangul_is_ignore_key(key->keyval)) {
    return FALSE;
  }

  /* handle direct mode */
  if (input_mode == MODE_DIRECT) {
    return im_hangul_handle_direct_mode(context_hangul, key);
  }

  /* handle Escape key: automaticaly change to direct mode */
  if (key->keyval == GDK_Escape) {
    if (context_hangul->choseong != 0 ||
	context_hangul->jungseong != 0 ||
	context_hangul->jongseong != 0) {
      im_hangul_commit(context_hangul);
      g_signal_emit_by_name (context_hangul, "preedit_changed");
    }
    im_hangul_mode_direct(context_hangul);
    return FALSE;
  }

  /* modifiler key */
  if (im_hangul_is_modifier(key->state)) {
    if (context_hangul->choseong != 0 ||
	context_hangul->jungseong != 0 ||
	context_hangul->jongseong != 0) {
      im_hangul_commit(context_hangul);
      g_signal_emit_by_name (context_hangul, "preedit_changed");
    }
    return FALSE;
  }

  /* hanja key */
  if (key->keyval == GDK_F9 || key->keyval == GDK_Hangul_Hanja) {
    popup_hanja_window(context_hangul);
    return TRUE;
  }

  if (context_hangul->automata)
    return context_hangul->automata(context_hangul, key);
  else
    return FALSE;
}


/*
 * status window
 */

static gboolean
status_window_expose_event(GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_rectangle (widget->window,
		      widget->style->bg_gc[GTK_STATE_NORMAL],
		      TRUE,
		      0, 0,
		      widget->allocation.width, widget->allocation.height);
  gdk_draw_rectangle (widget->window,
		      widget->style->text_gc[GTK_STATE_NORMAL],
		      FALSE,
		      0, 0,
		      widget->allocation.width - 1, widget->allocation.height - 1);

  return FALSE;
}

static void
status_window_style_set(GtkWidget *window,
			GtkStyle *previous_style,
			GtkWidget *label)
{
  gint i;

  for (i = 0; i < 5; i++)
    gtk_widget_modify_fg(label, i, &window->style->text[i]);
}

static void
status_window_free(StatusWindow *swindow)
{
  if (status_window) {
    g_signal_handler_disconnect(status_window->toplevel,
				status_window->destroy_handler_id);
    g_signal_handler_disconnect(status_window->toplevel,
				status_window->configure_handler_id);
    gtk_widget_destroy(status_window->window);
    g_free(status_window);
    status_window = NULL;
    /* g_print("Status window destroyed\n"); */
  }
}

static gboolean
status_window_configure	(GtkWidget *toplevel,
			 GdkEventConfigure *event,
			 StatusWindow *swindow)
{
  GdkRectangle rect;
  GtkRequisition requisition;
  gint y;

  gdk_window_get_frame_extents(toplevel->window, &rect);
  gtk_widget_size_request(status_window->window, &requisition);

  if (rect.y + rect.height + requisition.height < gdk_screen_height())
    y = rect.y + rect.height;
  else
    y = gdk_screen_height() - requisition.height;

  gtk_window_move(GTK_WINDOW(status_window->window), rect.x, y);
  return FALSE;
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

static GtkWidget *
status_window_get(GtkIMContextHangul *context_hangul, gboolean create)
{
  GtkWidget *toplevel;
  GtkWidget *window;
  GtkWidget *label;

  if (!pref_use_status_window)
    return NULL;

  toplevel = get_toplevel_window(context_hangul->client_window);
  if (toplevel == NULL)
    return NULL;

  if (status_window) {
    status_window_configure(toplevel, NULL, status_window);
    return status_window->window;
  } else if (!create)
    return NULL;

  status_window = g_new(StatusWindow, 1);
  status_window->window = gtk_window_new(GTK_WINDOW_POPUP);
  status_window->toplevel = toplevel;

  window = status_window->window;
  gtk_widget_set_name(window, "imhangul");
  gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
  gtk_widget_set_app_paintable(window, TRUE);

  label = gtk_label_new(""); 
  gtk_misc_set_padding(GTK_MISC(label), 2, 2);
  gtk_widget_show(label);
  gtk_container_add(GTK_CONTAINER(window), label);

  status_window->destroy_handler_id =
			g_signal_connect_swapped(toplevel, "destroy",
					       G_CALLBACK(status_window_free),
					       status_window);
  status_window->configure_handler_id = 
			g_signal_connect(toplevel, "configure_event",
					 G_CALLBACK(status_window_configure),
					 status_window);

  status_window_configure(toplevel, NULL, status_window);

/*
  g_signal_connect(window, "style_set",
		   G_CALLBACK(status_window_style_set), label);
*/
  g_signal_connect(window, "expose_event",
		   G_CALLBACK(status_window_expose_event), NULL);

  g_object_set_data(G_OBJECT(toplevel), "im-hangul-status-window",
		    status_window);

  gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(toplevel));
  status_window_set_mode(context_hangul);

  return window;
}

static void
status_window_show(GtkIMContextHangul *context_hangul)
{
  GtkWidget *window = status_window_get (context_hangul, TRUE);

  if (window) {
    status_window_set_mode(context_hangul);
    gtk_widget_show(window);
  }
}

static void
status_window_hide(GtkIMContextHangul *context_hangul)
{
  GtkWidget *window = status_window_get (context_hangul, FALSE);

  if (window) {
    gtk_widget_hide(window);
  }
}

static void
status_window_set_mode(GtkIMContextHangul *context_hangul)
{
  static const gchar english[] = { 
	'[', 0xec, 0x98, 0x81, 0xec, 0x96, 0xb4, ']', 0	/* utf8 string */
  };
  static const gchar hangul[] = { 
	'[', 0xed, 0x95, 0x9c, 0xea, 0xb8, 0x80, ']', 0 /* utf8 string */
  };

  GtkWidget *status_window = status_window_get(context_hangul, TRUE);
  if (status_window) {
    GtkWidget *label = GTK_BIN(status_window)->child;
    if (input_mode == MODE_DIRECT)
      gtk_label_set_text(GTK_LABEL(label), english);
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
  last = sizeof(hanjatable) / sizeof(gunichar*) - 1;
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
on_keypress(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
  if (event->keyval == GDK_Escape) {
    gtk_widget_destroy(hanja_window);
    return TRUE;
  }
  return FALSE;
}

static void
on_destroy(GtkWidget *widget, gpointer data)
{
  gtk_grab_remove(widget);
  hanja_window = NULL;
  input_mode = MODE_HANGUL;
}

static void
on_hanja_button_clicked(GtkButton *button, gpointer data)
{
  GtkIMContextHangul* context_hangul = GTK_IM_CONTEXT_HANGUL(data);
  gchar *str = (gchar *)gtk_button_get_label(GTK_BUTTON(button));

  if (str) {
    im_hangul_commit_utf8(context_hangul, str);
    context_hangul->state = MODE_HANGUL;
    context_hangul->index = -1;
    g_signal_emit_by_name (context_hangul, "preedit_changed");
  }
  gtk_widget_destroy(hanja_window);
}

static GtkWidget *
create_hanja_window(GtkIMContextHangul *context_hangul, gunichar ch)
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
  table = gtk_table_new(10, 1, TRUE);

  if (pref_hanja_font)
    desc = pango_font_description_from_string(pref_hanja_font);

  x = 0;
  y = 0;
  p = hanjatable[index] + 1;
  while (*p != 0) {
    n = g_unichar_to_utf8(*p, buf);
    buf[n] = 0;

    button = gtk_button_new_with_label(buf);
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
			  G_CALLBACK (on_hanja_button_clicked), context_hangul);

    x++;
    if (x > 9) {
      y++;
      x = 0;
    }
    p++;
  }
  gtk_container_add(GTK_CONTAINER(window), table);

  g_signal_connect (G_OBJECT(window), "key-press-event",
		    G_CALLBACK (on_keypress), NULL);
  g_signal_connect (G_OBJECT(window), "destroy",
		    G_CALLBACK (on_destroy), NULL);

  parent = get_toplevel_window(context_hangul->client_window);
  if (parent)
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));

  gtk_grab_add(window);
  gtk_widget_show_all(window);

  pango_font_description_free(desc);

  return window;
}

static void
popup_hanja_window(GtkIMContextHangul *context_hangul)
{
  gunichar ch;

  if (context_hangul->choseong == 0 || context_hangul->jungseong == 0)
    return; 

  ch = im_hangul_jamo_to_unicode(context_hangul->choseong,
				 context_hangul->jungseong,
	(context_hangul->jongseong)? context_hangul->jongseong : 0x11a7);
  if (ch)
    create_hanja_window(context_hangul, ch);
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
  status_window_free(status_window);
  status_window = NULL;

  /* remove hanja selection dialog */
  if (hanja_window) {
    gtk_widget_destroy(hanja_window);
    hanja_window = NULL;
  }

  g_print("im-hangul module shutdown\n");
}

#endif /* __GTK_IM_CONTEXT_HANGUL_H__ */

/* vim: nocindent
 */

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
 * this module is for hangul 3bul sik mode users
 */

#include <string.h>

#include <gdk/gdkkeysyms.h>

/*
#include "gtk/gtkintl.h"
#include "gtk/gtkimmodule.h"
*/
#include <gtk/gtkimmodule.h>

#include "imhangul.h"


static const GtkIMContextInfo hangul39_info = {
  "hangul39",
  "Hangul 3bul 390",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul39_info
};

#include "imhangul3.h"

/* Hangul keyboard map table:
 * 3set keyboard 390 (sebulsik 390) */
static const gunichar keyboard_table_390[] = {
  0x11bd,	/* GDK_exclam:		jongseong cieuc			*/
  0x0022,	/* GDK_quotedbl:	quotatioin mark			*/
  0x0023,	/* GDK_numbersign:	number sign	 		*/
  0x0024,	/* GDK_dollar:		dollar sign			*/
  0x0025,	/* GDK_percent:		percent sign			*/
  0x0026,	/* GDK_ampersand:	ampersand		 	*/
  0x1110,	/* GDK_apostrophe:	choseong thieuth		*/
  0x0028,	/* GDK_parenleft:	left parenthesis		*/
  0x0029,	/* GDK_parenright:	right parenthesis		*/
  0x002a,	/* GDK_asterisk:	asterisk			*/
  0x002b,	/* GDK_plus:		plus sign			*/
  0x002c,	/* GDK_comma:		comma				*/
  0x002d,	/* GDK_minus:		minus sign			*/
  0x002e,	/* GDK_period:		period				*/
  0x1169,	/* GDK_slash:		jungseong o			*/
  0x110f,	/* GDK_0:		choseong  khieukh		*/
  0x11c2,	/* GDK_1:		jongseong hieuh			*/
  0x11bb,	/* GDK_2:		jongseong ssangsios		*/
  0x11b8,	/* GDK_3:		jongseong pieup			*/
  0x116d,	/* GDK_4:		jungseong yo			*/
  0x1172,	/* GDK_5:		jungseong yu			*/
  0x1163,	/* GDK_6:		jungseong ya			*/
  0x1168,	/* GDK_7:		jungseong ye			*/
  0x1174,	/* GDK_8:		jungseong yi			*/
  0x116e,	/* GDK_9:		jungseong u			*/
  0x003a,	/* GDK_colon:		colon				*/
  0x1107,	/* GDK_semicolon:	choseong  pieup			*/
  0x0032,	/* GDK_less:		2				*/
  0x003d,	/* GDK_equal:		euals sign			*/
  0x0033,	/* GDK_greater:		3				*/
  0x003f,	/* GDK_question:	question mark	 		*/
  0x0040,	/* GDK_at:		commertial at			*/
  0x11ae,	/* GDK_A:		jongseong tikeut		*/
  0x0021,	/* GDK_B:		exclamation mark		*/
  0x11b1,	/* GDK_C:		jongseong rieul-mieum		*/
  0x11b0,	/* GDK_D:		jongseong rieul-kiyeok		*/
  0x11bf,	/* GDK_E:		jongseong khieukh		*/
  0x11a9,	/* GDK_F:		jongseong ssangkiyeok		*/
  0x002f,	/* GDK_G:		slash				*/
  0x0027,	/* GDK_H:		apostrophe			*/
  0x0038,	/* GDK_I:		8				*/
  0x0034,	/* GDK_J:		4				*/
  0x0035,	/* GDK_K:		5				*/
  0x0036,	/* GDK_L:		6				*/
  0x0031,	/* GDK_M:		1				*/
  0x0030,	/* GDK_N:		0				*/
  0x0039,	/* GDK_O:		9				*/
  0x003e,	/* GDK_P:		greater-than sign		*/
  0x11c1,	/* GDK_Q:		jongseong phieuph		*/
  0x1164,	/* GDK_R:		jungseong yae			*/
  0x11ad,	/* GDK_S:		jongseong nieun-hieuh		*/
  0x003b,	/* GDK_T:		semicolon			*/
  0x0037,	/* GDK_U:		7				*/
  0x11b6,	/* GDK_V:		jongseong rieul-hieuh		*/
  0x11c0,	/* GDK_W:		jongseong thikeuth		*/
  0x11b9,	/* GDK_X:		jongseong pieup-sios		*/
  0x003c,	/* GDK_Y:		less-than sign			*/
  0x11be,	/* GDK_Z:		jongseong chieuch		*/
  0x005b,	/* GDK_bracketleft:	left bracket			*/
  0x005c,	/* GDK_backslash:	backslash			*/
  0x005d,	/* GDK_bracketright:	right bracket			*/
  0x005e,	/* GDK_asciicircum:	circumflex accent		*/
  0x005f,	/* GDK_underscore:	underscore			*/
  0x0060,	/* GDK_quoteleft:	grave accent			*/
  0x11bc,	/* GDK_a:		jongseong ieung			*/
  0x116e,	/* GDK_b:		jungseong u			*/
  0x1166,	/* GDK_c:		jungseong e			*/
  0x1175,	/* GDK_d:		jungseong i			*/
  0x1167,	/* GDK_e:		jungseong yeo			*/
  0x1161,	/* GDK_f:		jungseong a			*/
  0x1173,	/* GDK_g:		jungseong eu			*/
  0x1102,	/* GDK_h:		choseong  nieun			*/
  0x1106,	/* GDK_i:		choseong  mieum			*/
  0x110b,	/* GDK_j:		choseong  ieung			*/
  0x1100,	/* GDK_k:		choseong  kiyeok		*/
  0x110c,	/* GDK_l:		choseong  cieuc			*/
  0x1112,	/* GDK_m:		choseong  hieuh			*/
  0x1109,	/* GDK_n:		choseong  sios			*/
  0x110e,	/* GDK_o:		choseong  chieuch		*/
  0x1111,	/* GDK_p:		choseong  phieuph		*/
  0x11ba,	/* GDK_q:		jongseong sios			*/
  0x1162,	/* GDK_r:		jungseong ae			*/
  0x11ab,	/* GDK_s:		jongseong nieun			*/
  0x1165, 	/* GDK_t:		jungseong eo			*/
  0x1103,	/* GDK_u:		choseong  tikeut		*/
  0x1169,	/* GDK_v:		jungseong o			*/
  0x11af,	/* GDK_w:		jongseong rieul			*/
  0x11a8,	/* GDK_x:		jongseong kiyeok		*/
  0x1105,	/* GDK_y:		choseong  rieul			*/
  0x11b7,	/* GDK_z:		jongseong mieum			*/
  0x007b,	/* GDK_braceleft:	left brace			*/
  0x007c,	/* GDK_bar:		vertical line(bar)		*/
  0x007d,	/* GDK_braceright:	right brace			*/
  0x007e,	/* GDK_asciitilde:	tilde				*/
};

void
im_module_init(GTypeModule *type_module)
{
  im_hangul_register_type(type_module, "GtkIMContextHangul39");
}

void
im_module_exit(void)
{
  im_hangul_shutdown();
}

void
im_module_list (const GtkIMContextInfo ***contexts,
		int *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS(info_list);
}

GtkIMContext *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, "hangul39") == 0) {
    GtkIMContext *context = im_hangul_new();
    if (context) {
      GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(context_hangul, im_hangul3_automata);

      keyboard_table = keyboard_table_390;

//      im_hangul3_choseong = im_hangul3_choseong_90;
//      im_hangul3_jungseong = im_hangul3_jungseong_90;
//      im_hangul3_jongseong = im_hangul3_jongseong_90;
//      im_hangul3_punct = im_hangul3_punct_90;
//      im_hangul3_comp_choseong = im_hangul3_comp_choseong_90;
//      im_hangul3_comp_jungseong = im_hangul3_comp_jungseong_90;
//      im_hangul3_comp_jongseong = im_hangul3_comp_jongseong_90;
      return context;
    }
  }
  return NULL;
}

/* vim: set nocindent : */

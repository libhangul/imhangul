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


static const GtkIMContextInfo hangul3f_info = {
  "hangul3f",
  "Hangul 3bul Final",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul3f_info
};

#include "imhangul3.h"

/* Hangul keyboard map table:
 * 3set keyboard final (sebulsik choejong) */
static const gunichar keyboard_table_3final[] = {
  0x11a9,	/* GDK_exclam:		jongseong ssangkiyeok		*/
  0x00b7,	/* GDK_quotedbl:	middle dot 			*/
  0x11bd,	/* GDK_numbersign:	jognseong cieuc 		*/
  0x11b5,	/* GDK_dollar:		jongseong rieul-phieuph		*/
  0x11b4,	/* GDK_percent:		jongseong rieul-thieuth		*/
  0x201c,	/* GDK_ampersand:	left double quotation mark 	*/
  0x1110,	/* GDK_apostrophe:	choseong  thieuth		*/
  0x0027,	/* GDK_parenleft:	apostrophe			*/
  0x007e,	/* GDK_parenright:	Tilde				*/
  0x201d,	/* GDK_asterisk:	right double quotation mark	*/
  0x002b,	/* GDK_plus:		plus sign			*/
  0x002c,	/* GDK_comma:		comma				*/
  0x0029,	/* GDK_minus:		right parenthesis		*/
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
  0x0034,	/* GDK_colon:		4				*/
  0x1107,	/* GDK_semicolon:	choseong  pieup			*/
  0x002c,	/* GDK_less:		comma				*/
  0x003e,	/* GDK_equal:		greater-than sign		*/
  0x002e,	/* GDK_greater:		period				*/
  0x0021,	/* GDK_question:	exclamation mark 		*/
  0x11b0,	/* GDK_at:		jongseong rieul-kiyeok		*/
  0x11ae,	/* GDK_A:		jongseong tikeut		*/
  0x003f,	/* GDK_B:		question mark			*/
  0x11bf,	/* GDK_C:		jongseong khieukh		*/
  0x11b2,	/* GDK_D:		jongseong rieul-pieup		*/
  0x11ac,	/* GDK_E:		jongseong nieun-cieuc		*/
  0x11b1,	/* GDK_F:		jongseong rieul-mieum		*/
  0x1164,	/* GDK_G:		jungseong yae			*/
  0x0030,	/* GDK_H:		0				*/
  0x0037,	/* GDK_I:		7				*/
  0x0031,	/* GDK_J:		1				*/
  0x0032,	/* GDK_K:		2				*/
  0x0033,	/* GDK_L:		3				*/
  0x0022,	/* GDK_M:		double quotation mark		*/
  0x002d,	/* GDK_N:		minus sign			*/
  0x0038,	/* GDK_O:		8				*/
  0x0039,	/* GDK_P:		9				*/
  0x11c1,	/* GDK_Q:		jongseong phieuph		*/
  0x11b6,	/* GDK_R:		jongseong rieul-hieuh		*/
  0x11ad,	/* GDK_S:		jongseong nieun-hieuh		*/
  0x11b3,	/* GDK_T:		jongseong rieul-sios		*/
  0x0036,	/* GDK_U:		6				*/
  0x11aa,	/* GDK_V:		jongseong kiyeok-sios		*/
  0x11c0,	/* GDK_W:		jongseong thikeuth		*/
  0x11b9,	/* GDK_X:		jongseong pieup-sios		*/
  0x0035,	/* GDK_Y:		5				*/
  0x11be,	/* GDK_Z:		jongseong chieuch		*/
  0x0028,	/* GDK_bracketleft:	left parenthesis		*/
  0x003a,	/* GDK_backslash:	colon				*/
  0x003c,	/* GDK_bracketright:	less-than sign			*/
  0x003d,	/* GDK_asciicircum:	equals sign			*/
  0x003b,	/* GDK_underscore:	semicolon			*/
  0x002a,	/* GDK_quoteleft:	asterisk			*/
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
  0x116b,	/* GDK_r:		jungseong ae			*/
  0x11ab,	/* GDK_s:		jongseong nieun			*/
  0x1165, 	/* GDK_t:		jungseong eo			*/
  0x1103,	/* GDK_u:		choseong  tikeut		*/
  0x1169,	/* GDK_v:		jungseong o			*/
  0x11af,	/* GDK_w:		jongseong rieul			*/
  0x11a8,	/* GDK_x:		jongseong kiyeok		*/
  0x1105,	/* GDK_y:		choseong  rieul			*/
  0x11b7,	/* GDK_z:		jongseong mieum			*/
  0x0025,	/* GDK_braceleft:	percent sign			*/
  0x005c,	/* GDK_bar:		backslash			*/
  0x002f,	/* GDK_braceright:	slash				*/
  0x203b,	/* GDK_asciitilde:	reference mark			*/
};

void
im_module_init(GTypeModule *type_module)
{
  im_hangul_register_type(type_module, "GtkIMContextHangul3f");
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
  if (strcmp (context_id, "hangul3f") == 0) {
    GtkIMContext *context = im_hangul_new();
    if (context) {
      GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(context_hangul, im_hangul3_automata);

      keyboard_table = keyboard_table_3final;

      return context;
    }
  }
  return NULL;
}

/* vim: set nocindent : */

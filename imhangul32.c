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
  "hangul32",
  "Hangul 3bul 2bul-shifted",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul3f_info
};

#include "imhangul3.h"

/* Hangul keyboard map table:
 * 3set keyboard 2bul shifted (sebulsik dubulsik badchim) */
static const gunichar keyboard_table_3final[] = {
  0x0021,	/* GDK_exclam:		exclamation mark		*/
  0x0022,	/* GDK_quotedbl:	quotation mark 			*/
  0x0023,	/* GDK_numbersign:	number sign	 		*/
  0x0024,	/* GDK_dollar:		dollar sign			*/
  0x0025,	/* GDK_percent:		percent sign			*/
  0x0026,	/* GDK_ampersand:	ampersand		 	*/
  0x0027,	/* GDK_apostrophe:	apostrophe			*/
  0x0028,	/* GDK_parenleft:	left parenthesis		*/
  0x0029,	/* GDK_parenright:	right parenthesis		*/
  0x002a,	/* GDK_asterisk:	asterisk			*/
  0x002b,	/* GDK_plus:		plus sign			*/
  0x002c,	/* GDK_comma:		comma				*/
  0x002d,	/* GDK_minus:		minus sign			*/
  0x002e,	/* GDK_period:		period				*/
  0x002f,	/* GDK_slash:		slash				*/
  0x0030,	/* GDK_0:		0				*/
  0x0031,	/* GDK_1:		1				*/
  0x0032,	/* GDK_2:		2				*/
  0x0033,	/* GDK_3:		3				*/
  0x0034,	/* GDK_4:		4				*/
  0x0035,	/* GDK_5:		5				*/
  0x0036,	/* GDK_6:		6				*/
  0x0037,	/* GDK_7:		7				*/
  0x0038,	/* GDK_8:		8				*/
  0x0039,	/* GDK_9:		9				*/
  0x003a,	/* GDK_colon:		colon				*/
  0x003b,	/* GDK_semicolon:	semicolon			*/
  0x003c,	/* GDK_less:		less-than sign			*/
  0x003d,	/* GDK_equal:		equals sign			*/
  0x003e,	/* GDK_greater:		greater-than sign		*/
  0x003f,	/* GDK_question:	question mark	 		*/
  0x0040,	/* GDK_at:		jongseong rieul-kiyeok		*/
  0x11b7,	/* GDK_A:		jongseong mieum			*/
  0x1172,	/* GDK_B:		jungseong yu			*/
  0x11be,	/* GDK_C:		jongseong chieuch		*/
  0x11bc,	/* GDK_D:		jongseong ieung			*/
  0x11ae,	/* GDK_E:		jongseong tikeut		*/
  0x11af,	/* GDK_F:		jongseong rieul			*/
  0x11c2,	/* GDK_G:		jongseong hieuh			*/
  0x1169,	/* GDK_H:		jungseong o			*/
  0x1163,	/* GDK_I:		jungseong ya			*/
  0x1165,	/* GDK_J:		jungseong eo			*/
  0x1161,	/* GDK_K:		jungseong a			*/
  0x1175,	/* GDK_L:		jungseong i			*/
  0x1173,	/* GDK_M:		jungseong eu			*/
  0x116e,	/* GDK_N:		jungseong u			*/
  0x1164,	/* GDK_O:		jungseong yae			*/
  0x1168,	/* GDK_P:		jungseong ye			*/
  0x11b8,	/* GDK_Q:		jongseong pieup			*/
  0x11a8,	/* GDK_R:		jongseong kiyeok		*/
  0x11ab,	/* GDK_S:		jongseong nieun			*/
  0x11ba,	/* GDK_T:		jongseong sios			*/
  0x1167,	/* GDK_U:		jungseong yeo			*/
  0x11c1,	/* GDK_V:		jongseong phieuph		*/
  0x11bd,	/* GDK_W:		jongseong cieuc			*/
  0x11c0,	/* GDK_X:		jongseong thieuth		*/
  0x116d,	/* GDK_Y:		jungseong yo			*/
  0x11bf,	/* GDK_Z:		jongseong khieukh		*/
  0x005b,	/* GDK_bracketleft:	left bracket			*/
  0x005c,	/* GDK_backslash:	backslash			*/
  0x005d,	/* GDK_bracketright:	right bracket			*/
  0x005e,	/* GDK_asciicircum:	circumflex accent		*/
  0x005f,	/* GDK_underscore:	underscore			*/
  0x0060,	/* GDK_quoteleft:	grave accent			*/
  0x1106,	/* GDK_a:		choseong  mieum			*/
  0x1172,	/* GDK_b:		jungseong yu			*/
  0x110e,	/* GDK_c:		choseong  chieuch		*/
  0x110b,	/* GDK_d:		choseong  ieung			*/
  0x1103,	/* GDK_e:		choseong  tikeut		*/
  0x1105,	/* GDK_f:		choseong  rieul			*/
  0x1112,	/* GDK_g:		choseong  hieuh			*/
  0x1169,	/* GDK_h:		jungseong o			*/
  0x1163,	/* GDK_i:		jungseong ya			*/
  0x1165,	/* GDK_j:		jungseong eo			*/
  0x1161,	/* GDK_k:		jungseong a			*/
  0x1175,	/* GDK_l:		jungseong i			*/
  0x1173,	/* GDK_m:		jungseong eu			*/
  0x116e,	/* GDK_n:		jungseong u			*/
  0x1162,	/* GDK_o:		jungseong ae			*/
  0x1166,	/* GDK_p:		jungseong e			*/
  0x1107,	/* GDK_q:		choseong  pieup			*/
  0x1100,	/* GDK_r:		choseong  kiyeok		*/
  0x1102,	/* GDK_s:		choseong  nieun			*/
  0x1109, 	/* GDK_t:		choseong  sios			*/
  0x1167,	/* GDK_u:		jungseong yeo			*/
  0x1111,	/* GDK_v:		choseong  phieuph		*/
  0x110c,	/* GDK_w:		choseong  cieuc			*/
  0x1110,	/* GDK_x:		choseong  thieuth		*/
  0x116d,	/* GDK_y:		jungseong yo			*/
  0x110f,	/* GDK_z:		choseong  khieukh		*/
  0x007b,	/* GDK_braceleft:	left brace			*/
  0x007c,	/* GDK_bar:		vertical bar			*/
  0x007d,	/* GDK_braceright:	right brace			*/
  0x007e,	/* GDK_asciitilde:	tilde				*/
};

void
im_module_init(GTypeModule *type_module)
{
  im_hangul_register_type(type_module, "GtkIMContextHangul32");
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
  if (strcmp (context_id, "hangul32") == 0) {
    GtkIMContext *context = im_hangul_new();
    if (context) {
      GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(hcontext, im_hangul3_automata);

      keyboard_table = keyboard_table_3final;

      return context;
    }
  }
  return NULL;
}

/* vim: set nocindent : */

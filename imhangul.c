/* ImHangul - Gtk+ 2.0 Input Method Module for Hangul
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
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <gdk/gdkkeysyms.h>

#include <gtk/gtkwidget.h>
#include <gtk/gtkimmodule.h>

#include "gettext.h"
#include "gtkimcontexthangul.h"

static GtkIMContext *im_hangul_new_2      (void);
static GtkIMContext *im_hangul_new_32     (void);
static GtkIMContext *im_hangul_new_390    (void);
static GtkIMContext *im_hangul_new_3final (void);
static GtkIMContext *im_hangul_new_3sun   (void);
static GtkIMContext *im_hangul_new_3yet   (void);

static const GtkIMContextInfo hangul2_info = {
  "hangul2",
  N_("Hangul 2bul"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  DEFAULT_KEYBOARD_2 
};

static const GtkIMContextInfo hangul32_info = {
  "hangul32",
  N_("Hangul 3bul 2bul-shifted"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  ""
};

static const GtkIMContextInfo hangul39_info = {
  "hangul39",
  N_("Hangul 3bul 390"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  DEFAULT_KEYBOARD_39 
};

static const GtkIMContextInfo hangul3f_info = {
  "hangul3f",
  N_("Hangul 3bul Final"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  DEFAULT_KEYBOARD_3F 
};

static const GtkIMContextInfo hangul3s_info = {
  "hangul3s",
  N_("Hangul 3bul No-Shift"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  ""
};

static const GtkIMContextInfo hangul3y_info = {
  "hangul3y",
  N_("Hangul 3bul Yetgeul"),
  GETTEXT_PACKAGE,
  IM_HANGUL_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul2_info,
  &hangul32_info,
  &hangul39_info,
  &hangul3f_info,
  &hangul3s_info,
  &hangul3y_info
};

void
im_module_init (GTypeModule *type_module)
{
  gtk_im_context_hangul_register_type (type_module);
}

void
im_module_exit (void)
{
  gtk_im_context_hangul_shutdown ();
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
  if (strcmp (context_id, "hangul2") == 0) {
    return im_hangul_new_2 ();
  } else if (strcmp (context_id, "hangul32") == 0) {
    return im_hangul_new_32 ();
  } else if (strcmp (context_id, "hangul39") == 0) {
    return im_hangul_new_390 ();
  } else if (strcmp (context_id, "hangul3f") == 0) {
    return im_hangul_new_3final ();
  } else if (strcmp (context_id, "hangul3s") == 0) {
    return im_hangul_new_3sun ();
  } else if (strcmp (context_id, "hangul3y") == 0) {
    return im_hangul_new_3yet ();
  }

  g_warning("imhangul:unknown context id: %s", context_id); 
  g_assert_not_reached ();

  return NULL;
}

/* Hangul keyboard map table:
 * 2set keyboard (dubulsik) */
static const gunichar keyboard_table_2[] = {
  0x0021,	/* GDK_exclam:		exclamation mark	*/
  0x0022,	/* GDK_quotedbl:	quotation mark 		*/
  0x0023,	/* GDK_numbersign:	number sign	 	*/
  0x0024,	/* GDK_dollar:		dollar sign		*/
  0x0025,	/* GDK_percent:		percent sign		*/
  0x0026,	/* GDK_ampersand:	ampersand		*/
  0x0027,	/* GDK_apostrophe:	apostrophe		*/
  0x0028,	/* GDK_parenleft:	left parenthesis	*/
  0x0029,	/* GDK_parenright:	right parenthesis	*/
  0x002a,	/* GDK_asterisk:	asterisk		*/
  0x002b,	/* GDK_plus:		plus sign		*/
  0x002c,	/* GDK_comma:		comma			*/
  0x002d,	/* GDK_minus:		minus sign		*/
  0x002e,	/* GDK_period:		period			*/
  0x002f,	/* GDK_slash:		slash			*/
  0x0030,	/* GDK_0:		0			*/
  0x0031,	/* GDK_1:		1			*/
  0x0032,	/* GDK_2:		2			*/
  0x0033,	/* GDK_3:		3			*/
  0x0034,	/* GDK_4:		4			*/
  0x0035,	/* GDK_5:		5			*/
  0x0036,	/* GDK_6:		6			*/
  0x0037,	/* GDK_7:		7			*/
  0x0038,	/* GDK_8:		8			*/
  0x0039,	/* GDK_9:		9			*/
  0x003a,	/* GDK_colon:		colon			*/
  0x003b,	/* GDK_semicolon:	semicolon		*/
  0x003c,	/* GDK_less:		less-than sign		*/
  0x003d,	/* GDK_equal:		equals sign		*/
  0x003e,	/* GDK_greater:		greater-than sign	*/
  0x003f,	/* GDK_question:	question mark	 	*/
  0x0040,	/* GDK_at:		commercial at		*/
  0x1106,	/* GDK_A:		choseong mieum		*/
  0x1172,	/* GDK_B:		jungseong yu		*/
  0x110e,	/* GDK_C:		choseong chieuch	*/
  0x110b,	/* GDK_D:		choseong ieung		*/
  0x1104,	/* GDK_E:		choseong ssangtikeut	*/
  0x1105,	/* GDK_F:		choseong rieul		*/
  0x1112,	/* GDK_G:		choseong hieuh		*/
  0x1169,	/* GDK_H:		jungseong o		*/
  0x1163,	/* GDK_I:		jungseong ya		*/
  0x1165,	/* GDK_J:		jungseong eo		*/
  0x1161,	/* GDK_K:		jungseong a		*/
  0x1175,	/* GDK_L:		jungseong i		*/
  0x1173,	/* GDK_M:		jungseong eu		*/
  0x116e,	/* GDK_N:		jungseong u		*/
  0x1164,	/* GDK_O:		jungseong yae		*/
  0x1168,	/* GDK_P:		jungseong ye		*/
  0x1108,	/* GDK_Q:		choseong ssangpieup	*/
  0x1101,	/* GDK_R:		choseong ssangkiyeok	*/
  0x1102,	/* GDK_S:		choseong nieun		*/
  0x110a,	/* GDK_T:		choseong ssangsios	*/
  0x1167,	/* GDK_U:		jungseong yeo		*/
  0x1111,	/* GDK_V:		choseong phieuph	*/
  0x110d,	/* GDK_W:		choseong ssangcieuc	*/
  0x1110,	/* GDK_X:		choseong thieuth	*/
  0x116d,	/* GDK_Y:		jungseong yo		*/
  0x110f,	/* GDK_Z:		choseong khieukh	*/
  0x005b,	/* GDK_bracketleft:	left bracket		*/
  0x005c,	/* GDK_backslash:	backslash		*/
  0x005d,	/* GDK_bracketright:	right bracket		*/
  0x005e,	/* GDK_asciicircum:	circumflex accent	*/
  0x005f,	/* GDK_underscore:	underscore		*/
  0x0060,	/* GDK_quoteleft:	grave accent		*/
  0x1106,	/* GDK_a:		choseong  mieum		*/
  0x1172,	/* GDK_b:		jungseong yu		*/
  0x110e,	/* GDK_c:		choseong  chieuch	*/
  0x110b,	/* GDK_d:		choseong  ieung		*/
  0x1103,	/* GDK_e:		choseong  tikeut	*/
  0x1105,	/* GDK_f:		choseong  rieul		*/
  0x1112,	/* GDK_g:		choseong  hieuh		*/
  0x1169,	/* GDK_h:		jungseong o		*/
  0x1163,	/* GDK_i:		jungseong ya		*/
  0x1165,	/* GDK_j:		jungseong eo		*/
  0x1161,	/* GDK_k:		jungseong a		*/
  0x1175,	/* GDK_l:		jungseong i		*/
  0x1173,	/* GDK_m:		jungseong eu		*/
  0x116e,	/* GDK_n:		jungseong u		*/
  0x1162,	/* GDK_o:		jungseong ae		*/
  0x1166,	/* GDK_p:		jungseong e		*/
  0x1107,	/* GDK_q:		choseong  pieup		*/
  0x1100,	/* GDK_r:		choseong  kiyeok	*/
  0x1102,	/* GDK_s:		choseong  nieun		*/
  0x1109, 	/* GDK_t:		choseong  sios		*/
  0x1167,	/* GDK_u:		jungseong yeo		*/
  0x1111,	/* GDK_v:		choseong  phieuph	*/
  0x110c,	/* GDK_w:		choseong  cieuc		*/
  0x1110,	/* GDK_x:		choseong  thieuth	*/
  0x116d,	/* GDK_y:		jungseong yo		*/
  0x110f,	/* GDK_z:		choseong  khieukh	*/
  0x007b,	/* GDK_braceleft:	left brace		*/
  0x007c,	/* GDK_bar:		vertical bar		*/
  0x007d,	/* GDK_braceright:	right brace		*/
  0x007e,	/* GDK_asciitilde:	tilde			*/
};

/* Hangul keyboard map table:
 * 3set keyboard 2bul shifted (sebulsik dubulsik badchim) */
static const gunichar keyboard_table_32[] = {
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
  0x0040,	/* GDK_at:		commercial at			*/
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
  0x1162,	/* GDK_r:		jungseong ae			*/
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

/* Hangul keyboard map table:
 * 3set keyboard no-shift (sebulsik sunarae) */
static const gunichar keyboard_table_3sun[] = {
  0x0021,	/* GDK_exclam:		exclamation mark		*/
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
  0x11bd,	/* GDK_minus:		jongseong cieuc			*/
  0x002e,	/* GDK_period:		period				*/
  0x11ae,	/* GDK_slash:		jongseong tikeut		*/
  0x1164,	/* GDK_0:		choseong  yae			*/
  0x11c2,	/* GDK_1:		jongseong hieuh			*/
  0x11bb,	/* GDK_2:		jongseong ssangsios		*/
  0x11b8,	/* GDK_3:		jongseong pieup			*/
  0x116d,	/* GDK_4:		jungseong yo			*/
  0x1172,	/* GDK_5:		jungseong yu			*/
  0x1163,	/* GDK_6:		jungseong ya			*/
  0x1168,	/* GDK_7:		jungseong ye			*/
  0x1174,	/* GDK_8:		jungseong yi			*/
  0x110f,	/* GDK_9:		choseong khieukh		*/
  0x003a,	/* GDK_colon:		colon				*/
  0x1107,	/* GDK_semicolon:	choseong  pieup			*/
  0x0032,	/* GDK_less:		2				*/
  0x11be,	/* GDK_equal:		jongseong chieuch		*/
  0x0033,	/* GDK_greater:		3				*/
  0x003f,	/* GDK_question:	question mark	 		*/
  0x0040,	/* GDK_at:		commertial at			*/
  0x11bc,	/* GDK_A:		jongseong ieung			*/
  0x0021,	/* GDK_B:		exclamation mark		*/
  0x005c,	/* GDK_C:		backslash			*/
  0x005d,	/* GDK_D:		right bracket			*/
  0x1167,	/* GDK_E:		jungseong yeo			*/
  0x1161,	/* GDK_F:		jungseong a			*/
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
  0x11ba,	/* GDK_Q:		jongseong sios			*/
  0x1162,	/* GDK_R:		jungseong ae			*/
  0x005b,	/* GDK_S:		left bracket			*/
  0x003b,	/* GDK_T:		semicolon			*/
  0x0037,	/* GDK_U:		7				*/
  0x1169,	/* GDK_V:		jungseong o			*/
  0x11af,	/* GDK_W:		jongseong rieul			*/
  0x003d,	/* GDK_X:		equals sign			*/
  0x003c,	/* GDK_Y:		less-than sign			*/
  0x002d,	/* GDK_Z:		minus sign			*/
  0x11c0,	/* GDK_bracketleft:	jongseong thieuth		*/
  0x11bf,	/* GDK_backslash:	jongseong khieukh		*/
  0x11c1,	/* GDK_bracketright:	jongseong phieuph		*/
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

/* Hangul keyboard map table:
 * 3set keyboard yetguel (sebulsik yetguel)
 * see http://asadal.cs.pusan.ac.kr/hangeul/kbd/oldkbd.jpg */
static const gunichar keyboard_table_3yet[] = {
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
  0x113c,	/* GDK_less:		choseong chitueumsios		*/
  0x003d,	/* GDK_equal:		euals sign			*/
  0x113e,	/* GDK_greater:		choseong ceongchieumsios	*/
  0x003f,	/* GDK_question:	question mark	 		*/
  0x11eb,	/* GDK_at:		jongseong pansios		*/
  0x11ae,	/* GDK_A:		jongseong tikeut		*/
  0x116e,	/* GDK_B:		jungseong u			*/
  0x11b1,	/* GDK_C:		jongseong rieul-mieum		*/
  0x11b0,	/* GDK_D:		jongseong rieul-kiyeok		*/
  0x11bf,	/* GDK_E:		jongseong khieukh		*/
  0x11a9,	/* GDK_F:		jongseong ssangkiyeok		*/
  0x119e,	/* GDK_G:		jungseong araea			*/
  0x1102,	/* GDK_H:		choseong nieun			*/
  0x1154,	/* GDK_I:		choseong chitueumchieuch	*/
  0x114c,	/* GDK_J:		choseong yesieung		*/
  0x114e,	/* GDK_K:		choseong chitueumcieuc		*/
  0x1150,	/* GDK_L:		choseong ceongchieumcieuc	*/
  0x1159,	/* GDK_M:		choseong yeorinhieuh		*/
  0x1140,	/* GDK_N:		choseong pansios		*/
  0x1155,	/* GDK_O:		choseong ceongchieumchieuch	*/
  0x1111,	/* GDK_P:		choseong phieuph		*/
  0x11c1,	/* GDK_Q:		jongseong phieuph		*/
  0x1164,	/* GDK_R:		jungseong yae			*/
  0x11ad,	/* GDK_S:		jongseong nieun-hieuh		*/
  0x1165,	/* GDK_T:		jungseong eo			*/
  0x302e,	/* GDK_U:		single dot tone mark		*/
  0x11b6,	/* GDK_V:		jongseong rieul-hieuh		*/
  0x11c0,	/* GDK_W:		jongseong thikeuth		*/
  0x11b9,	/* GDK_X:		jongseong pieup-sios		*/
  0x302f,	/* GDK_Y:		double dot tone mark		*/
  0x11be,	/* GDK_Z:		jongseong chieuch		*/
  0x005b,	/* GDK_bracketleft:	left bracket			*/
  0x005c,	/* GDK_backslash:	backslash			*/
  0x005d,	/* GDK_bracketright:	right bracket			*/
  0x005e,	/* GDK_asciicircum:	circumflex accent		*/
  0x005f,	/* GDK_underscore:	underscore			*/
  0x11f9,	/* GDK_quoteleft:	jongseong yeorinhieuh		*/
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
  0x11f0,	/* GDK_asciitilde:	jongseong yesieung		*/
};

static const IMHangulCombination compose_table_yet[] = {
  { 0x11001100, 0x1101 }, /* CHOSEONG SSANGKIYEOK           = KIYEOK + KIYEOK */
  { 0x11021100, 0x1113 }, /* CHOSEONG NIEUN-KIYEOK          = NIEUN + KIYEOK */
  { 0x11021102, 0x1114 }, /* CHOSEONG SSANGNIEUN            = NIEUN + NIEUN */
  { 0x11021103, 0x1115 }, /* CHOSEONG NIEUN-TIKEUT          = NIEUN + TIKEUT */
  { 0x11021107, 0x1116 }, /* CHOSEONG NIEUN-PIEUP           = NIEUN + PIEUP */
  { 0x11031100, 0x1117 }, /* CHOSEONG TIKEUT-KIYEOK         = TIKEUT + KIYEOK */
  { 0x11031103, 0x1104 }, /* CHOSEONG SSANGTIKEUT           = TIKEUT + TIKEUT */
  { 0x11051102, 0x1118 }, /* CHOSEONG RIEUL-NIEUN           = RIEUL + NIEUN */
  { 0x11051105, 0x1119 }, /* CHOSEONG SSANGRIEUL            = RIEUL + RIEUL */
  { 0x1105110b, 0x111b }, /* CHOSEONG KAPYEOUNRIEUL         = RIEUL + IEUNG */
  { 0x11051112, 0x111a }, /* CHOSEONG RIEUL-HIEUH           = RIEUL + HIEUH */
  { 0x11061107, 0x111c }, /* CHOSEONG MIEUM-PIEUP           = MIEUM + PIEUP */
  { 0x1106110b, 0x111d }, /* CHOSEONG KAPYEOUNMIEUM         = MIEUM + IEUNG */
  { 0x11071100, 0x111e }, /* CHOSEONG PIEUP-KIYEOK          = PIEUP + KIYEOK */
  { 0x11071102, 0x111f }, /* CHOSEONG PIEUP-NIEUN           = PIEUP + NIEUN */
  { 0x11071103, 0x1120 }, /* CHOSEONG PIEUP-TIKEUT          = PIEUP + TIKEUT */
  { 0x11071107, 0x1108 }, /* CHOSEONG SSANGPIEUP            = PIEUP + PIEUP */
  { 0x11071109, 0x1121 }, /* CHOSEONG PIEUP-SIOS            = PIEUP + SIOS */
  { 0x1107110b, 0x112b }, /* CHOSEONG KAPYEOUNPIEUP         = PIEUP + IEUNG */
  { 0x1107110c, 0x1127 }, /* CHOSEONG PIEUP-CIEUC           = PIEUP + CIEUC */
  { 0x1107110e, 0x1128 }, /* CHOSEONG PIEUP-CHIEUCH         = PIEUP + CHIEUCH */
  { 0x11071110, 0x1129 }, /* CHOSEONG PIEUP-THIEUTH         = PIEUP + THIEUTH */
  { 0x11071111, 0x112a }, /* CHOSEONG PIEUP-PHIEUPH         = PIEUP + PHIEUPH */
  { 0x1108110b, 0x112c }, /* CHOSEONG KAPYEOUNSSANGPIEUP    = SSANGPIEUP + IEUNG */
  { 0x11091100, 0x112d }, /* CHOSEONG SIOS-KIYEOK           = SIOS + KIYEOK */
  { 0x11091102, 0x112e }, /* CHOSEONG SIOS-NIEUN            = SIOS + NIEUN */
  { 0x11091103, 0x112f }, /* CHOSEONG SIOS-TIKEUT           = SIOS + TIKEUT */
  { 0x11091105, 0x1130 }, /* CHOSEONG SIOS-RIEUL            = SIOS + RIEUL */
  { 0x11091106, 0x1131 }, /* CHOSEONG SIOS-MIEUM            = SIOS + MIEUM */
  { 0x11091107, 0x1132 }, /* CHOSEONG SIOS-PIEUP            = SIOS + PIEUP */
  { 0x11091109, 0x110a }, /* CHOSEONG SSANGSIOS             = SIOS + SIOS */
  { 0x1109110b, 0x1135 }, /* CHOSEONG SIOS-IEUNG            = SIOS + IEUNG */
  { 0x1109110c, 0x1136 }, /* CHOSEONG SIOS-CIEUC            = SIOS + CIEUC */
  { 0x1109110e, 0x1137 }, /* CHOSEONG SIOS-CHIEUCH          = SIOS + CHIEUCH */
  { 0x1109110f, 0x1138 }, /* CHOSEONG SIOS-KHIEUKH          = SIOS + KHIEUKH */
  { 0x11091110, 0x1139 }, /* CHOSEONG SIOS-THIEUTH          = SIOS + THIEUTH */
  { 0x11091111, 0x113a }, /* CHOSEONG SIOS-PHIEUPH          = SIOS + PHIEUPH */
  { 0x11091112, 0x113b }, /* CHOSEONG SIOS-HIEUH            = SIOS + HIEUH */
  { 0x110a1109, 0x1134 }, /* CHOSEONG SIOS-SSANGSIOS        = SSANGSIOS + SIOS */
  { 0x110b1100, 0x1141 }, /* CHOSEONG IEUNG-KIYEOK          = IEUNG + KIYEOK */
  { 0x110b1103, 0x1142 }, /* CHOSEONG IEUNG-TIKEUT          = IEUNG + TIKEUT */
  { 0x110b1106, 0x1143 }, /* CHOSEONG IEUNG-MIEUM           = IEUNG + MIEUM */
  { 0x110b1107, 0x1144 }, /* CHOSEONG IEUNG-PIEUP           = IEUNG + PIEUP */
  { 0x110b1109, 0x1145 }, /* CHOSEONG IEUNG-SIOS            = IEUNG + SIOS */
  { 0x110b110b, 0x1147 }, /* CHOSEONG SSANGIEUNG            = IEUNG + IEUNG */
  { 0x110b110c, 0x1148 }, /* CHOSEONG IEUNG-CIEUC           = IEUNG + CIEUC */
  { 0x110b110e, 0x1149 }, /* CHOSEONG IEUNG-CHIEUCH         = IEUNG + CHIEUCH */
  { 0x110b1110, 0x114a }, /* CHOSEONG IEUNG-THIEUTH         = IEUNG + THIEUTH */
  { 0x110b1111, 0x114b }, /* CHOSEONG IEUNG-PHIEUPH         = IEUNG + PHIEUPH */
  { 0x110b1140, 0x1146 }, /* CHOSEONG IEUNG-PANSIOS         = IEUNG + PANSIOS */
  { 0x110c110b, 0x114d }, /* CHOSEONG CIEUC-IEUNG           = CIEUC + IEUNG */
  { 0x110c110c, 0x110d }, /* CHOSEONG SSANGCIEUC            = CIEUC + CIEUC */
  { 0x110e110f, 0x1152 }, /* CHOSEONG CHIEUCH-KHIEUKH       = CHIEUCH + KHIEUKH */
  { 0x110e1112, 0x1153 }, /* CHOSEONG CHIEUCH-HIEUH         = CHIEUCH + HIEUH */
  { 0x11111107, 0x1156 }, /* CHOSEONG PHIEUPH-PIEUP         = PHIEUPH + PIEUP */
  { 0x1111110b, 0x1157 }, /* CHOSEONG KAPYEOUNPHIEUPH       = PHIEUPH + IEUNG */
  { 0x11121112, 0x1158 }, /* CHOSEONG SSANGHIEUH            = HIEUH + HIEUH */
  { 0x11211100, 0x1122 }, /* CHOSEONG PIEUP-SIOS-KIYEOK     = PIEUP-SIOS + KIYEOK */
  { 0x11211103, 0x1123 }, /* CHOSEONG PIEUP-SIOS-TIKEUT     = PIEUP-SIOS + TIKEUT */
  { 0x11211107, 0x1124 }, /* CHOSEONG PIEUP-SIOS-PIEUP      = PIEUP-SIOS + PIEUP */
  { 0x11211109, 0x1125 }, /* CHOSEONG PIEUP-SSANGSIOS       = PIEUP-SIOS + SIOS */
  { 0x1121110c, 0x1126 }, /* CHOSEONG PIEUP-SIOS-CIEUC      = PIEUP-SIOS + CIEUC */
  { 0x11321100, 0x1133 }, /* CHOSEONG SIOS-PIEUP-KIYEOK     = SIOS-PIEUP + KIYEOK */
  { 0x113c113c, 0x113d }, /* CHOSEONG CHITUEUMSSANGSIOS     = CHITUEUMSIOS + CHITUEUMSIOS */
  { 0x113e113e, 0x113f }, /* CHOSEONG CEONGCHIEUMSSANGSIOS  = CEONGCHIEUMSIOS + CEONGCHIEUMSIOS */
  { 0x114e114e, 0x114f }, /* CHOSEONG CHITUEUMSSANGCIEUC    = CHITUEUMCIEUC + CHITUEUMCIEUC */
  { 0x11501150, 0x1151 }, /* CHOSEONG CEONGCHIEUMSSANGCIEUC = CEONGCHIEUMCIEUC + CEONGCHIEUMCIEUC */
  { 0x11611169, 0x1176 }, /* JUNGSEONG A-O                  = A + O */
  { 0x1161116e, 0x1177 }, /* JUNGSEONG A-U                  = A + U */
  { 0x11611175, 0x1162 }, /* JUNGSEONG AE                   = A */
  { 0x11631169, 0x1178 }, /* JUNGSEONG YA-O                 = YA + O */
  { 0x1163116d, 0x1179 }, /* JUNGSEONG YA-YO                = YA + YO */
  { 0x11631175, 0x1164 }, /* JUNGSEONG YAE                  = YA */
  { 0x11651169, 0x117a }, /* JUNGSEONG EO-O                 = EO + O */
  { 0x1165116e, 0x117b }, /* JUNGSEONG EO-U                 = EO + U */
  { 0x11651173, 0x117c }, /* JUNGSEONG EO-EU                = EO + EU */
  { 0x11651175, 0x1166 }, /* JUNGSEONG E                    = EO */
  { 0x11671169, 0x117d }, /* JUNGSEONG YEO-O                = YEO + O */
  { 0x1167116e, 0x117e }, /* JUNGSEONG YEO-U                = YEO + U */
  { 0x11671175, 0x1168 }, /* JUNGSEONG YE                   = YEO */
  { 0x11691161, 0x116a }, /* JUNGSEONG WA                   = O + A */
  { 0x11691165, 0x117f }, /* JUNGSEONG O-EO                 = O + EO */
  { 0x11691166, 0x1180 }, /* JUNGSEONG O-E                  = O + E */
  { 0x11691168, 0x1181 }, /* JUNGSEONG O-YE                 = O + YE */
  { 0x11691169, 0x1182 }, /* JUNGSEONG O-O                  = O + O */
  { 0x1169116e, 0x1183 }, /* JUNGSEONG O-U                  = O + U */
  { 0x11691175, 0x116c }, /* JUNGSEONG OE                   = O */
  { 0x116a1175, 0x116b }, /* JUNGSEONG WAE                  = WA */
  { 0x116d1163, 0x1184 }, /* JUNGSEONG YO-YA                = YO + YA */
  { 0x116d1164, 0x1185 }, /* JUNGSEONG YO-YAE               = YO + YAE */
  { 0x116d1167, 0x1186 }, /* JUNGSEONG YO-YEO               = YO + YEO */
  { 0x116d1169, 0x1187 }, /* JUNGSEONG YO-O                 = YO + O */
  { 0x116d1175, 0x1188 }, /* JUNGSEONG YO-I                 = YO + I */
  { 0x116e1161, 0x1189 }, /* JUNGSEONG U-A                  = U + A */
  { 0x116e1162, 0x118a }, /* JUNGSEONG U-AE                 = U + AE */
  { 0x116e1165, 0x116f }, /* JUNGSEONG WEO                  = U + EO */
  { 0x116e1168, 0x118c }, /* JUNGSEONG U-YE                 = U + YE */
  { 0x116e116e, 0x118d }, /* JUNGSEONG U-U                  = U + U */
  { 0x116e1175, 0x1171 }, /* JUNGSEONG WI                   = U */
  { 0x116f1173, 0x118b }, /* JUNGSEONG U-EO-EU              = WEO + EU */
  { 0x116f1175, 0x1170 }, /* JUNGSEONG WE                   = WEO */
  { 0x11721161, 0x118e }, /* JUNGSEONG YU-A                 = YU + A */
  { 0x11721165, 0x118f }, /* JUNGSEONG YU-EO                = YU + EO */
  { 0x11721166, 0x1190 }, /* JUNGSEONG YU-E                 = YU + E */
  { 0x11721167, 0x1191 }, /* JUNGSEONG YU-YEO               = YU + YEO */
  { 0x11721168, 0x1192 }, /* JUNGSEONG YU-YE                = YU + YE */
  { 0x1172116e, 0x1193 }, /* JUNGSEONG YU-U                 = YU + U */
  { 0x11721175, 0x1194 }, /* JUNGSEONG YU-I                 = YU + I */
  { 0x1173116e, 0x1195 }, /* JUNGSEONG EU-U                 = EU + U */
  { 0x11731173, 0x1196 }, /* JUNGSEONG EU-EU                = EU + EU */
  { 0x11731175, 0x1174 }, /* JUNGSEONG YI                   = EU */
  { 0x1174116e, 0x1197 }, /* JUNGSEONG YI-U                 = YI + U */
  { 0x11751161, 0x1198 }, /* JUNGSEONG I-A                  = I + A */
  { 0x11751163, 0x1199 }, /* JUNGSEONG I-YA                 = I + YA */
  { 0x11751169, 0x119a }, /* JUNGSEONG I-O                  = I + O */
  { 0x1175116e, 0x119b }, /* JUNGSEONG I-U                  = I + U */
  { 0x11751173, 0x119c }, /* JUNGSEONG I-EU                 = I + EU */
  { 0x1175119e, 0x119d }, /* JUNGSEONG I-ARAEA              = I */
  { 0x119e1165, 0x119f }, /* JUNGSEONG ARAEA-EO             = ARAEA + EO */
  { 0x119e116e, 0x11a0 }, /* JUNGSEONG ARAEA-U              = ARAEA + U */
  { 0x119e1175, 0x11a1 }, /* JUNGSEONG ARAEA-I              = ARAEA + I */
  { 0x119e119e, 0x11a2 }, /* JUNGSEONG SSANGARAEA           = ARAEA + ARAEA */
  { 0x11a811a8, 0x11a9 }, /* JONGSEONG SSANGKIYEOK          = KIYEOK + KIYEOK */
  { 0x11a811af, 0x11c3 }, /* JONGSEONG KIYEOK-RIEUL         = KIYEOK + RIEUL */
  { 0x11a811ba, 0x11aa }, /* JONGSEONG KIYEOK-SIOS          = KIYEOK */
  { 0x11aa11a8, 0x11c4 }, /* JONGSEONG KIYEOK-SIOS-KIYEOK   = KIYEOK-SIOS + KIYEOK */
  { 0x11ab11a8, 0x11c5 }, /* JONGSEONG NIEUN-KIYEOK         = NIEUN + KIYEOK */
  { 0x11ab11ae, 0x11c6 }, /* JONGSEONG NIEUN-TIKEUT         = NIEUN + TIKEUT */
  { 0x11ab11ba, 0x11c7 }, /* JONGSEONG NIEUN-SIOS           = NIEUN + SIOS */
  { 0x11ab11bd, 0x11ac }, /* JONGSEONG NIEUN-CIEUC          = NIEUN */
  { 0x11ab11c0, 0x11c9 }, /* JONGSEONG NIEUN-THIEUTH        = NIEUN + THIEUTH */
  { 0x11ab11c2, 0x11ad }, /* JONGSEONG NIEUN-HIEUH          = NIEUN */
  { 0x11ab11eb, 0x11c8 }, /* JONGSEONG NIEUN-PANSIOS        = NIEUN */
  { 0x11ae11a8, 0x11ca }, /* JONGSEONG TIKEUT-KIYEOK        = TIKEUT + KIYEOK */
  { 0x11ae11af, 0x11cb }, /* JONGSEONG TIKEUT-RIEUL         = TIKEUT + RIEUL */
  { 0x11af11a8, 0x11b0 }, /* JONGSEONG RIEUL-KIYEOK         = RIEUL + KIYEOK */
  { 0x11af11ab, 0x11cd }, /* JONGSEONG RIEUL-NIEUN          = RIEUL + NIEUN */
  { 0x11af11ae, 0x11ce }, /* JONGSEONG RIEUL-TIKEUT         = RIEUL + TIKEUT */
  { 0x11af11af, 0x11d0 }, /* JONGSEONG SSANGRIEUL           = RIEUL + RIEUL */
  { 0x11af11b7, 0x11b1 }, /* JONGSEONG RIEUL-MIEUM          = RIEUL */
  { 0x11af11b8, 0x11b2 }, /* JONGSEONG RIEUL-PIEUP          = RIEUL */
  { 0x11af11ba, 0x11b3 }, /* JONGSEONG RIEUL-SIOS           = RIEUL */
  { 0x11af11bf, 0x11d8 }, /* JONGSEONG RIEUL-KHIEUKH        = RIEUL + KHIEUKH */
  { 0x11af11c0, 0x11b4 }, /* JONGSEONG RIEUL-THIEUTH        = RIEUL */
  { 0x11af11c1, 0x11b5 }, /* JONGSEONG RIEUL-PHIEUPH        = RIEUL */
  { 0x11af11c2, 0x11b6 }, /* JONGSEONG RIEUL-HIEUH          = RIEUL */
  { 0x11af11eb, 0x11d7 }, /* JONGSEONG RIEUL-PANSIOS        = RIEUL */
  { 0x11af11f9, 0x11d9 }, /* JONGSEONG RIEUL-YEORINHIEUH    = RIEUL */
  { 0x11b011ba, 0x11cc }, /* JONGSEONG RIEUL-KIYEOK-SIOS    = RIEUL-KIYEOK + SIOS */
  { 0x11b111a8, 0x11d1 }, /* JONGSEONG RIEUL-MIEUM-KIYEOK   = RIEUL-MIEUM + KIYEOK */
  { 0x11b111ba, 0x11d2 }, /* JONGSEONG RIEUL-MIEUM-SIOS     = RIEUL-MIEUM + SIOS */
  { 0x11b211ba, 0x11d3 }, /* JONGSEONG RIEUL-PIEUP-SIOS     = RIEUL-PIEUP + SIOS */
  { 0x11b211bc, 0x11d5 }, /* JONGSEONG RIEUL-KAPYEOUNPIEUP  = RIEUL-PIEUP + IEUNG */
  { 0x11b211c2, 0x11d4 }, /* JONGSEONG RIEUL-PIEUP-HIEUH    = RIEUL-PIEUP + HIEUH */
  { 0x11b311ba, 0x11d6 }, /* JONGSEONG RIEUL-SSANGSIOS      = RIEUL-SIOS + SIOS */
  { 0x11b711a8, 0x11da }, /* JONGSEONG MIEUM-KIYEOK         = MIEUM + KIYEOK */
  { 0x11b711af, 0x11db }, /* JONGSEONG MIEUM-RIEUL          = MIEUM + RIEUL */
  { 0x11b711b8, 0x11dc }, /* JONGSEONG MIEUM-PIEUP          = MIEUM + PIEUP */
  { 0x11b711ba, 0x11dd }, /* JONGSEONG MIEUM-SIOS           = MIEUM + SIOS */
  { 0x11b711bc, 0x11e2 }, /* JONGSEONG KAPYEOUNMIEUM        = MIEUM + IEUNG */
  { 0x11b711be, 0x11e0 }, /* JONGSEONG MIEUM-CHIEUCH        = MIEUM + CHIEUCH */
  { 0x11b711c2, 0x11e1 }, /* JONGSEONG MIEUM-HIEUH          = MIEUM + HIEUH */
  { 0x11b711eb, 0x11df }, /* JONGSEONG MIEUM-PANSIOS        = MIEUM */
  { 0x11b811af, 0x11e3 }, /* JONGSEONG PIEUP-RIEUL          = PIEUP + RIEUL */
  { 0x11b811ba, 0x11b9 }, /* JONGSEONG PIEUP-SIOS           = PIEUP */
  { 0x11b811bc, 0x11e6 }, /* JONGSEONG KAPYEOUNPIEUP        = PIEUP + IEUNG */
  { 0x11b811c1, 0x11e4 }, /* JONGSEONG PIEUP-PHIEUPH        = PIEUP + PHIEUPH */
  { 0x11b811c2, 0x11e5 }, /* JONGSEONG PIEUP-HIEUH          = PIEUP + HIEUH */
  { 0x11ba11a8, 0x11e7 }, /* JONGSEONG SIOS-KIYEOK          = SIOS + KIYEOK */
  { 0x11ba11ae, 0x11e8 }, /* JONGSEONG SIOS-TIKEUT          = SIOS + TIKEUT */
  { 0x11ba11af, 0x11e9 }, /* JONGSEONG SIOS-RIEUL           = SIOS + RIEUL */
  { 0x11ba11b8, 0x11ea }, /* JONGSEONG SIOS-PIEUP           = SIOS + PIEUP */
  { 0x11ba11ba, 0x11bb }, /* JONGSEONG SSANGSIOS            = SIOS + SIOS */
  { 0x11bc11a8, 0x11ec }, /* JONGSEONG IEUNG-KIYEOK         = IEUNG + KIYEOK */
  { 0x11bc11bc, 0x11ee }, /* JONGSEONG SSANGIEUNG           = IEUNG + IEUNG */
  { 0x11bc11bf, 0x11ef }, /* JONGSEONG IEUNG-KHIEUKH        = IEUNG + KHIEUKH */
  { 0x11c111b8, 0x11f3 }, /* JONGSEONG PHIEUPH-PIEUP        = PHIEUPH + PIEUP */
  { 0x11c111bc, 0x11f4 }, /* JONGSEONG KAPYEOUNPHIEUPH      = PHIEUPH + IEUNG */
  { 0x11c211ab, 0x11f5 }, /* JONGSEONG HIEUH-NIEUN          = HIEUH + NIEUN */
  { 0x11c211af, 0x11f6 }, /* JONGSEONG HIEUH-RIEUL          = HIEUH + RIEUL */
  { 0x11c211b7, 0x11f7 }, /* JONGSEONG HIEUH-MIEUM          = HIEUH + MIEUM */
  { 0x11c211b8, 0x11f8 }, /* JONGSEONG HIEUH-PIEUP          = HIEUH + PIEUP */
  { 0x11ce11c2, 0x11cf }, /* JONGSEONG RIEUL-TIKEUT-HIEUH   = RIEUL-TIKEUT + HIEUH */
  { 0x11dd11ba, 0x11de }, /* JONGSEONG MIEUM-SSANGSIOS      = MIEUM-SIOS + SIOS */
  { 0x11ec11a8, 0x11ed }, /* JONGSEONG IEUNG-SSANGKIYEOK    = IEUNG-KIYEOK + KIYEOK */
  { 0x11f011ba, 0x11f1 }, /* JONGSEONG YESIEUNG-SIOS        = YESIEUNG + SIOS */
  { 0x11f011eb, 0x11f2 }, /* JONGSEONG YESIEUNG-PANSIOS     = YESIEUNG + PANSIOS */
};

static GtkIMContext *
im_hangul_new_2 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_2);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_2);

  return context;
}

static GtkIMContext *
im_hangul_new_32 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_3);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_32);

  return context;
}

static GtkIMContext *
im_hangul_new_390 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_3);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_390);

  return context;
}

static GtkIMContext *
im_hangul_new_3final (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_3);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_3final);

  return context;
}

static GtkIMContext *
im_hangul_new_3sun (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_3);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_3sun);

  return context;
}

static GtkIMContext *
im_hangul_new_3yet (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_set_composer (hcontext, IM_HANGUL_COMPOSER_3);
  gtk_im_context_hangul_set_keyboard_table (hcontext, keyboard_table_3yet);
  gtk_im_context_hangul_set_compose_table (hcontext,
					   compose_table_yet,
					   G_N_ELEMENTS (compose_table_yet));
  gtk_im_context_hangul_set_use_jamo (hcontext, TRUE);

  return context;
}

/* vim: set sw=2 : */

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
 * this module is for hangul 2bul sik mode users
 */

#include <string.h>

#include <gdk/gdkkeysyms.h>

/*
#include "gtk/gtkintl.h"
#include "gtk/gtkimmodule.h"
*/
#include <gtk/gtkimmodule.h>

#include "imhangul.h"

/* preferences */
static gboolean		pref_enable_inverse_sequence = TRUE;

static gunichar	im_hangul_compjungseong_to_single	(gunichar ch);
static gunichar	im_hangul_compjongseong_to_single	(gunichar ch);

static const GtkIMContextInfo hangul2_info = {
  "hangul2",
  "Hangul 2bul",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul2_info
};

/* Hangul keyboard map table:
 * 2set keyboard (dubulsik) */
static const gunichar keyboard_table_2[] = {
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
  0x1106,	/* GDK_A:		choseong mieum			*/
  0x1172,	/* GDK_B:		jungseong yu			*/
  0x110e,	/* GDK_C:		choseong chieuch		*/
  0x1106,	/* GDK_D:		choseong ieung			*/
  0x1104,	/* GDK_E:		choseong ssangtikeut		*/
  0x1105,	/* GDK_F:		choseong rieul			*/
  0x1112,	/* GDK_G:		choseong hieuh			*/
  0x1169,	/* GDK_H:		jungseong o			*/
  0x1163,	/* GDK_I:		jungseong ya			*/
  0x1165,	/* GDK_J:		jungseong eo			*/
  0x1161,	/* GDK_K:		jungseong a			*/
  0x1175,	/* GDK_L:		jungseong i			*/
  0x1173,	/* GDK_M:		jungseong eu			*/
  0x116e,	/* GDK_N:		jungseong u			*/
  0x1164,	/* GDK_O:		jungseong yae			*/
  0x1168,	/* GDK_P:		jungseong ye			*/
  0x1108,	/* GDK_Q:		choseong ssangpieup		*/
  0x1101,	/* GDK_R:		choseong ssangkiyeok		*/
  0x1102,	/* GDK_S:		choseong nieun			*/
  0x110a,	/* GDK_T:		choseong ssangsios		*/
  0x1167,	/* GDK_U:		jungseong yeo			*/
  0x1111,	/* GDK_V:		choseong phieuph		*/
  0x110d,	/* GDK_W:		choseong ssangcieuc		*/
  0x1110,	/* GDK_X:		choseong thieuth		*/
  0x116d,	/* GDK_Y:		jungseong yo			*/
  0x110f,	/* GDK_Z:		choseong khieukh		*/
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
im_hangul2_choseong_to_jongseong(gunichar ch)
{
  static gunichar table[] = {
    0x11a8,	/* choseong kiyeok	  ->	jongseong kiyeok */
    0x11a9,	/* choseong ssangkiyeok	  ->	jongseong ssangkiyeok */
    0x11ab,	/* choseong nieun	  ->	jongseong nieun */
    0x11ae,	/* choseong tikeut	  ->	jongseong tikeut */
    0x0,	/* choseong ssangtikeut	  ->	jongseong tikeut */
    0x11af,	/* choseong rieul	  ->	jongseong rieul */
    0x11b7,	/* choseong mieum 	  ->	jongseong mieum */
    0x11b8,	/* choseong pieup	  ->	jongseong pieup */
    0x0,	/* choseong ssangpieup	  ->	jongseong pieup */
    0x11ba,	/* choseong sios	  ->	jongseong sios */
    0x11bb,	/* choseong ssangsios     ->	jongseong ssangsios */
    0x11bc,	/* choseong ieung	  ->	jongseong ieung */
    0x11bd,	/* choseong cieuc	  ->	jongseong cieuc */
    0x0,	/* choseong ssangcieuc	  ->	jongseong cieuc */
    0x11be,	/* choseong chieuch	  ->	jongseong chieuch */
    0x11bf,	/* choseong khieukh	  ->	jongseong khieukh */
    0x11c0,	/* choseong thieuth	  ->	jongseong thieuth */
    0x11c1,	/* choseong phieuph	  ->	jongseong phieuph */
    0x11c2	/* choseong hieuh	  ->	jongseong hieuh */
  };

  if (ch < 0x1100 || ch > 0x1112)
  	return 0;
  return table[ch - 0x1100];
}

static gunichar
im_hangul2_jongseong_to_choseong(gunichar ch)
{
  static gunichar table[] = {
    0x1100,	/* jongseong kiyeok	  ->	choseong kiyeok */
    0x1101,	/* jongseong ssangkiyeok  ->	choseong ssangkiyeok */
    0x1109,	/* jongseong kiyeok-sios  ->	choseong sios */
    0x1102,	/* jongseong nieun	  ->	choseong nieun */
    0x110c,	/* jongseong nieun-cieuc  ->	choseong cieuc */
    0x1112,	/* jongseong nieun-hieuh  ->	choseong hieuh */
    0x1103,	/* jongseong tikeut	  ->	choseong tikeut */
    0x1105,	/* jongseong rieul	  ->	choseong rieul */
    0x1100,	/* jongseong rieul-kiyeok ->	choseong kiyeok */
    0x1106,	/* jongseong rieul-mieum  ->	choseong mieum */
    0x1107,	/* jongseong rieul-pieup  ->	choseong pieup */
    0x1109,	/* jongseong rieul-sios   ->	choseong sios */
    0x1110,	/* jongseong rieul-thieuth ->	choseong thieuth */
    0x1111,	/* jongseong rieul-phieuph ->	choseong phieuph */
    0x1112,	/* jongseong rieul-hieuh  ->	choseong hieuh */
    0x1106,	/* jongseong mieum 	  ->	choseong mieum */
    0x1107,	/* jongseong pieup	  ->	choseong pieup */
    0x1109,	/* jongseong pieup-sios   ->	choseong sios */
    0x1109,	/* jongseong sios	  ->	choseong sios */
    0x110a,	/* jongseong ssangsios    ->	choseong ssangsios */
    0x110b,	/* jongseong ieung	  ->	choseong ieung */
    0x110c,	/* jongseong cieuc	  ->	choseong cieuc */
    0x110e,	/* jongseong chieuch	  ->	choseong chieuch */
    0x110f,	/* jongseong khieukh	  ->	choseong khieukh */
    0x1110,	/* jongseong thieuth	  ->	choseong thieuth */
    0x1111,	/* jongseong phieuph	  ->	choseong phieuph */
    0x1112	/* jongseong hieuh	  ->	choseong hieuh */
  };
  return table[ch - 0x11a8];
}

static void
im_hangul2_jongseong_dicompose(gunichar ch,
			       gunichar* jong,
			       gunichar* cho)
{
  static gunichar table[][2] = {
    { 0,      0x1100 }, /* jong kiyeok	      = cho  kiyeok               */
    { 0x11a8, 0x1100 }, /* jong ssangkiyeok   = jong kiyeok + cho kiyeok  */
    { 0x11a8, 0x1109 }, /* jong kiyeok-sios   = jong kiyeok + cho sios    */
    { 0,      0x1102 }, /* jong nieun	      = cho  nieun                */
    { 0x11ab, 0x110c }, /* jong nieun-cieuc   = jong nieun  + cho cieuc   */
    { 0x11ab, 0x1112 }, /* jong nieun-hieuh   = jong nieun  + cho hieuh   */
    { 0,      0x1103 }, /* jong tikeut	      = cho  tikeut               */
    { 0,      0x1105 }, /* jong rieul         = cho  rieul                */
    { 0x11af, 0x1100 }, /* jong rieul-kiyeok  = jong rieul  + cho kiyeok  */
    { 0x11af, 0x1106 }, /* jong rieul-mieum   = jong rieul  + cho mieum   */
    { 0x11af, 0x1107 }, /* jong rieul-pieup   = jong rieul  + cho pieup   */
    { 0x11af, 0x1109 }, /* jong rieul-sios    = jong rieul  + cho sios    */
    { 0x11af, 0x1110 }, /* jong rieul-thieuth = jong rieul  + cho thieuth */
    { 0x11af, 0x1111 }, /* jong rieul-phieuph = jong rieul  + cho phieuph */
    { 0x11af, 0x1112 }, /* jong rieul-hieuh   = jong rieul  + cho hieuh   */
    { 0,      0x1106 }, /* jong mieum         = cho  mieum                */
    { 0,      0x1107 }, /* jong pieup         = cho  pieup                */
    { 0x11b8, 0x1109 }, /* jong pieup-sios    = jong pieup  + cho sios    */
    { 0,      0x1109 }, /* jong sios          = cho  sios                 */
    { 0x11ba, 0x1109 }, /* jong ssangsios     = jong sios   + cho sios    */
    { 0,      0x110b }, /* jong ieung         = cho  ieung                */
    { 0,      0x110c }, /* jong cieuc         = cho  cieuc                */
    { 0,      0x110e }, /* jong chieuch       = cho  chieuch              */
    { 0,      0x110f }, /* jong khieukh       = cho  khieukh              */
    { 0,      0x1110 }, /* jong thieuth       = cho  thieuth              */
    { 0,      0x1111 }, /* jong phieuph       = cho  phieuph              */
    { 0,      0x1112 }  /* jong hieuh         = cho  hieuh                */
  };

  *jong = table[ch - 0x11a8][0];
  *cho  = table[ch - 0x11a8][1];
}

static gboolean
im_hangul2_automata(GtkIMContextHangul *hcontext,
		    GdkEventKey *key)
{
  guint keyval, state;
  gunichar ch;
  gunichar comp_ch;
  gunichar jong_ch;

  if (pref_use_dvorak) {
    keyval = im_hangul_dvorak_to_qwerty(key->keyval);
    state = key->state;
  } else {
    keyval = key->keyval;
    state = key->state;
  }

  ch = im_hangul_mapping(keyval, state);

  if (hcontext->state == 0) {
    if (im_hangul_is_choseong(ch)) {
      hcontext->state = 1;
      hcontext->choseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      hcontext->state = 2;
      hcontext->choseong[0] = 0;
      hcontext->jungseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key))
      return FALSE;
  } else if (hcontext->state == 1) {
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->state = 1;
      hcontext->choseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      hcontext->jungseong[0] = ch;
      hcontext->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->choseong[0] = 0;
      hcontext->state = 0;
      goto done;
    }
  } else if (hcontext->state == 2) {
    if (im_hangul_is_choseong(ch)) {
      if (pref_enable_inverse_sequence) {
        hcontext->state = 3;
      } else {
        im_hangul_commit(hcontext);
        hcontext->state = 1;
      }
      hcontext->choseong[0] = ch;
      goto done;
    }
    comp_ch = im_hangul_compose(hcontext->jungseong[0], ch);
    if (im_hangul_is_jungseong(comp_ch)) {
      hcontext->jungseong[0] = comp_ch;
      hcontext->state = 4;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->jungseong[0] = ch;
      hcontext->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jungseong[0] = 0;
      hcontext->state = 0;
      goto done;
    }
  } else if (hcontext->state == 3) {
    jong_ch = im_hangul2_choseong_to_jongseong(ch);
    if (im_hangul_is_jongseong(jong_ch)) {
      hcontext->jongseong[0] = jong_ch;
      hcontext->state = 5;
      goto done;
    }
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      hcontext->state = 1;
      goto done;
    }
    comp_ch = im_hangul_compose(hcontext->jungseong[0], ch);
    if (im_hangul_is_jungseong(comp_ch)) {
      hcontext->jungseong[0] = comp_ch;
      hcontext->state = 6;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->jungseong[0] = ch;
      hcontext->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jungseong[0] = 0;
      hcontext->state = 1;
      goto done;
    }
  } else if (hcontext->state == 4) {
    if (im_hangul_is_choseong(ch)) {
      if (pref_enable_inverse_sequence) {
        hcontext->state = 3;
      } else {
        im_hangul_commit(hcontext);
        hcontext->state = 1;
      }
      hcontext->choseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->jungseong[0] = ch;
      hcontext->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jungseong[0] = 
		im_hangul_compjungseong_to_single(hcontext->jungseong[0]);
      hcontext->state = 2;
      goto done;
    }
  } else if (hcontext->state == 5) {
    jong_ch = im_hangul2_choseong_to_jongseong(ch);
    comp_ch = im_hangul_compose(hcontext->jongseong[0], jong_ch);
    if (im_hangul_is_jongseong(comp_ch)) {
      hcontext->jongseong[0] = comp_ch;
      hcontext->state = 7;
      goto done;
    }
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      hcontext->state = 1;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(hcontext->jongseong[0]);
      hcontext->jongseong[0] = 0;
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = choseong;
      hcontext->jungseong[0] = ch;
      hcontext->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jongseong[0] = 0;
      hcontext->state = 3;
      goto done;
    }
  } else if (hcontext->state == 6) {
    jong_ch = im_hangul2_choseong_to_jongseong(ch);
    if (im_hangul_is_jongseong(jong_ch)) {
      hcontext->jongseong[0] = jong_ch;
      hcontext->state = 8;
      goto done;
    }
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      hcontext->state = 1;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->jungseong[0] = ch;
      hcontext->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jungseong[0] = 
       im_hangul_compjungseong_to_single(hcontext->jungseong[0]);
      hcontext->state = 3;
      goto done;
    }
  } else if (hcontext->state == 7) {
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      hcontext->state = 1;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      /* complex final consonant
       * to single final consonant and initial consonant */
      gunichar choseong; 
      gunichar jongseong; 
      im_hangul2_jongseong_dicompose(hcontext->jongseong[0],
      				     &jongseong, &choseong);
      hcontext->jongseong[0] = jongseong;
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = choseong;
      hcontext->jungseong[0] = ch;
      hcontext->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jongseong[0] = 
		im_hangul_compjongseong_to_single(hcontext->jongseong[0]);
      hcontext->state = 5;
      goto done;
    }
  } else if (hcontext->state == 8) {
    jong_ch = im_hangul2_choseong_to_jongseong(ch);
    comp_ch = im_hangul_compose(hcontext->jongseong[0], jong_ch);
    if (im_hangul_is_jongseong(comp_ch)) {
      hcontext->jongseong[0] = comp_ch;
      hcontext->state = 9;
      goto done;
    }
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      hcontext->state = 1;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(hcontext->jongseong[0]);
      hcontext->jongseong[0] = 0;
      im_hangul_commit(hcontext);
      hcontext->state = 3;
      hcontext->choseong[0] = choseong;
      hcontext->jungseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->state = 6;
      hcontext->jongseong[0] = 0;
      goto done;
    }
  } else if (hcontext->state == 9) {
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(hcontext);
      hcontext->state = 1;
      hcontext->choseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_jungseong(ch)) {
      /* complex final consonant
       * to single final consonant and initial consonant */
      gunichar choseong; 
      gunichar jongseong; 
      im_hangul2_jongseong_dicompose(hcontext->jongseong[0],
      				     &jongseong, &choseong);
      hcontext->jongseong[0] = jongseong;
      im_hangul_commit(hcontext);
      hcontext->state = 3;
      hcontext->choseong[0] = choseong;
      hcontext->jungseong[0] = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      hcontext->jongseong[0] = 
      		im_hangul_compjongseong_to_single(hcontext->jongseong[0]);
      hcontext->state = 8;
      goto done;
    }
  }

  if (im_hangul_is_trigger(key)) {
    /* hangul mode change to englishmode */
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    im_hangul_mode_direct(hcontext);
    return TRUE;
  }

  if (hcontext->state != 0) {
    if (im_hangul_commit(hcontext))
      g_signal_emit_by_name (hcontext, "preedit_changed");
    hcontext->state = 0;
  }
  return im_hangul_process_nonhangul(hcontext, key); /* english */

done:
  g_signal_emit_by_name (hcontext, "preedit_changed");
  return TRUE;
}

void
im_module_init(GTypeModule *type_module)
{
  im_hangul_register_type(type_module, "GtkIMContextHangul2");
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
  if (strcmp (context_id, "hangul2") == 0) {
    GtkIMContext *context = im_hangul_new();
    if (context) {
      GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(hcontext, im_hangul2_automata);

      keyboard_table = keyboard_table_2;

      return context;
    }
  }
  return NULL;
}

/* vim: set nocindent : */

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

static const GtkIMContextInfo hangul2_info = {
  "hangul2",
  "Hangul 2bul(UTF8)",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul2_info
};

/* return ucs4 code jamo choseong value(U+1100 ~ U+1112) in 2bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul2_choseong(guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (keyval) {
      case GDK_R:
      case GDK_r:
        return 0x1101;			/* choseong ssangkiyeok */
      case GDK_S:
      case GDK_s:
        return 0x1102;  		/* choseong nieun */
      case GDK_E:
      case GDK_e:
        return 0x1104;  		/* choseong ssangtikeut */
      case GDK_F:
      case GDK_f:
        return 0x1105;  		/* choseong rieul */
      case GDK_A:
      case GDK_a:
        return 0x1106;  		/* choseong mieum */
      case GDK_Q:
      case GDK_q:
        return 0x1108;  		/* choseong ssangpieup */
      case GDK_T:
      case GDK_t:
        return 0x110a;  		/* choseong ssangsios */
      case GDK_D:
      case GDK_d:
        return 0x110b;  		/* choseong ieung */
      case GDK_W:
      case GDK_w:
        return 0x110d;  		/* choseong ssangcieuc */
      case GDK_C:
      case GDK_c:
        return 0x110e;  		/* choseong chieuch */
      case GDK_Z:
      case GDK_z:
        return 0x110f;  		/* choseong khieukh */
      case GDK_X:
      case GDK_x:
        return 0x1110;  		/* choseong thieuth */
      case GDK_V:
      case GDK_v:
        return 0x1111;  		/* choseong phieuph */
      case GDK_G:
      case GDK_g:
        return 0x1112;  		/* choseong hieuh */
    }
  } else {
    switch (keyval) {
      case GDK_R:
      case GDK_r:
        return 0x1100;			/* choseong kiyeok */
      case GDK_S:
      case GDK_s:
        return 0x1102;  		/* choseong nieun */
      case GDK_E:
      case GDK_e:
        return 0x1103;  		/* choseong tikeut */
      case GDK_F:
      case GDK_f:
        return 0x1105;  		/* choseong rieul */
      case GDK_A:
      case GDK_a:
        return 0x1106;  		/* choseong mieum */
      case GDK_Q:
      case GDK_q:
        return 0x1107;  		/* choseong pieup */
      case GDK_T:
      case GDK_t:
        return 0x1109;  		/* choseong sios */
      case GDK_D:
      case GDK_d:
        return 0x110b;  		/* choseong ieung */
      case GDK_W:
      case GDK_w:
        return 0x110c;  		/* choseong cieuc */
      case GDK_C:
      case GDK_c:
        return 0x110e;  		/* choseong chieuch */
      case GDK_Z:
      case GDK_z:
        return 0x110f;  		/* choseong khieukh */
      case GDK_X:
      case GDK_x:
        return 0x1110;  		/* choseong thieuth */
      case GDK_V:
      case GDK_v:
        return 0x1111;  		/* choseong phieuph */
      case GDK_G:
      case GDK_g:
        return 0x1112;  		/* choseong hieuh */
    }
  }
  return 0;
}

/* return unicode jamo jungseong value(U+1161 ~ U+1173) in 2bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul2_jungseong(guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (keyval) {
      case GDK_K:
      case GDK_k:
        return 0x1161;			/* jungseong a */
      case GDK_O:
      case GDK_o:
        return 0x1164;			/* jungseong yae */
      case GDK_I:
      case GDK_i:
        return 0x1163;			/* jungseong ya */
      case GDK_J:
      case GDK_j:
        return 0x1165;			/* jungseong eo */
      case GDK_P:
      case GDK_p:
        return 0x1168;			/* jungseong ye */
      case GDK_U:
      case GDK_u:
        return 0x1167;			/* jungseong yeo */
      case GDK_H:
      case GDK_h:
        return 0x1169;			/* jungseong o */
      case GDK_Y:
      case GDK_y:
        return 0x116d;			/* jungseong yo */
      case GDK_N:
      case GDK_n:
        return 0x116e;			/* jungseong u */
      case GDK_B:
      case GDK_b:
        return 0x1172;			/* jungseong yu */
      case GDK_M:
      case GDK_m:
        return 0x1173;			/* jungseong eu */
      case GDK_L:
      case GDK_l:
        return 0x1175;			/* jungseong i */
    }
  } else {
    switch (keyval) {
      case GDK_K:
      case GDK_k:
        return 0x1161;			/* jungseong a */
      case GDK_O:
      case GDK_o:
        return 0x1162;			/* jungseong ae */
      case GDK_I:
      case GDK_i:
        return 0x1163;			/* jungseong ya */
      case GDK_J:
      case GDK_j:
        return 0x1165;			/* jungseong eo */
      case GDK_P:
      case GDK_p:
        return 0x1166;			/* jungseong e */
      case GDK_U:
      case GDK_u:
        return 0x1167;			/* jungseong yeo */
      case GDK_H:
      case GDK_h:
        return 0x1169;			/* jungseong o */
      case GDK_Y:
      case GDK_y:
        return 0x116d;			/* jungseong yo */
      case GDK_N:
      case GDK_n:
        return 0x116e;			/* jungseong u */
      case GDK_B:
      case GDK_b:
        return 0x1172;			/* jungseong yu */
      case GDK_M:
      case GDK_m:
        return 0x1173;			/* jungseong eu */
      case GDK_L:
      case GDK_l:
        return 0x1175;			/* jungseong i */
    }
  }
  return 0;
}

/* return unicode jamo jungseong value(U+1161 ~ U+1173) in 2bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul2_comp_jungseong(gunichar ch, guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK))
    return 0;
  switch (ch) {
    case 0x1169:
      switch (keyval) {
        case GDK_K:
        case GDK_k:
          return 0x116a;	/* jungseong wa */
        case GDK_O:
        case GDK_o:
          return 0x116b;        /* jungseong wae */
        case GDK_L:
        case GDK_l:
          return 0x116c;        /* jungseong oe */
      }
      break;
    case 0x116e:
      switch (keyval) {
        case GDK_J:
        case GDK_j:
          return 0x116f;	/* jungseong weo */
        case GDK_P:
        case GDK_p:
          return 0x1170;        /* jungseong we */
        case GDK_L:
        case GDK_l:
          return 0x1171;        /* jungseong wi */
      }
      break;
    case 0x1173:
      switch (keyval) {
        case GDK_L:
        case GDK_l:
          return 0x1174;	/* jungseong yi */
      }
      break;
  }
  return 0;
}

/* return unicode jamo jongseong value(U+11a8 ~ U+11c2) in 2bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul2_jongseong(guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (keyval) {
      case GDK_R:
      case GDK_r:
        return 0x11a9;			/* jongseong ssangkiyeok */
      case GDK_S:
      case GDK_s:
        return 0x11ab;			/* jongseong nieun */
      case GDK_F:
      case GDK_f:
        return 0x11af;			/* jongseong rieul */
      case GDK_A:
      case GDK_a:
        return 0x11b7;			/* jongseong mieum */
      case GDK_T:
      case GDK_t:
        return 0x11bb;			/* jongseong ssangsios */
      case GDK_D:
      case GDK_d:
        return 0x11bc;			/* jongseong ieung */
      case GDK_C:
      case GDK_c:
        return 0x11be;			/* jongseong chieuch */
      case GDK_Z:
      case GDK_z:
        return 0x11bf;			/* jongseong khieukh */
      case GDK_X:
      case GDK_x:
        return 0x11c0;			/* jongseong thieuth */
      case GDK_V:
      case GDK_v:
        return 0x11c1;			/* jongseong phieuph */
      case GDK_G:
      case GDK_g:
        return 0x11c2;			/* jongseong hieuh */      
    }
  } else {
    switch (keyval) {
      case GDK_R:
      case GDK_r:
        return 0x11a8;			/* jongseong kiyeok */
      case GDK_S:
      case GDK_s:
        return 0x11ab;			/* jongseong nieun */
      case GDK_E:
      case GDK_e:
        return 0x11ae;			/* jongseong tikeut */
      case GDK_F:
      case GDK_f:
        return 0x11af;			/* jongseong rieul */
      case GDK_A:
      case GDK_a:
        return 0x11b7;			/* jongseong mieum */
      case GDK_Q:
      case GDK_q:
        return 0x11b8;			/* jongseong pieup */
      case GDK_T:
      case GDK_t:
        return 0x11ba;			/* jongseong sios */
      case GDK_D:
      case GDK_d:
        return 0x11bc;			/* jongseong ieung */
      case GDK_W:
      case GDK_w:
        return 0x11bd;			/* jongseong cieuc */
      case GDK_C:
      case GDK_c:
        return 0x11be;			/* jongseong chieuch */
      case GDK_Z:
      case GDK_z:
        return 0x11bf;			/* jongseong khieukh */
      case GDK_X:
      case GDK_x:
        return 0x11c0;			/* jongseong thieuth */
      case GDK_V:
      case GDK_v:
        return 0x11c1;			/* jongseong phieuph */
      case GDK_G:
      case GDK_g:
        return 0x11c2;			/* jongseong hieuh */      
    }
  }
  return 0;
}

/* return unicode jamo jongseong value(U+11a8 ~ U+11c2) in 2bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul2_comp_jongseong(gunichar ch, guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (ch) {
      case 0x11ab:		/* jongseong nieun */
        switch (keyval) {
          case GDK_G:
          case GDK_g:
            return 0x11ad;	/* jongseong nieun-hieuh */
        }
        break;
      case 0x11af:		/* jongseong rieul */
        switch (keyval) {
          case GDK_A:
          case GDK_a:
            return 0x11b1;	/* jongseong rieul-mieum */
          case GDK_X:
          case GDK_x:
            return 0x11b4;	/* jongseong rieul-thieuth */
          case GDK_V:
          case GDK_v:
            return 0x11b5;	/* jongseong rieul-phieuph */
          case GDK_G:
          case GDK_g:
            return 0x11b6;	/* jongseong rieul-hieuh */
        }
        break;
    }
  } else {
    switch (ch) {
      case 0x11a8:		/* jongseong kiyeok */
        switch (keyval) {
          case GDK_T:
          case GDK_t:
            return 0x11aa;	/* jongseong kiyeok-sios */
        }
        break;
      case 0x11ab:		/* jongseong nieun */
        switch (keyval) {
          case GDK_W:
          case GDK_w:
            return 0x11ac;	/* jongseong nieun-cieuc */
          case GDK_G:
          case GDK_g:
            return 0x11ad;	/* jongseong nieun-hieuh */
        }
        break;
      case 0x11af:		/* jongseong rieul */
        switch (keyval) {
          case GDK_R:
          case GDK_r:
            return 0x11b0;	/* jongseong rieul-kiyeok */
          case GDK_A:
          case GDK_a:
            return 0x11b1;	/* jongseong rieul-mieum */
          case GDK_Q:
          case GDK_q:
            return 0x11b2;	/* jongseong rieul-pieup */
          case GDK_T:
          case GDK_t:
            return 0x11b3;	/* jongseong rieul-sios */
          case GDK_X:
          case GDK_x:
            return 0x11b4;	/* jongseong rieul-thieuth */
          case GDK_V:
          case GDK_v:
            return 0x11b5;	/* jongseong rieul-phieuph */
          case GDK_G:
          case GDK_g:
            return 0x11b6;	/* jongseong rieul-hieuh */
        }
        break;
      case 0x11b8:		/* jongseong pieup */
        switch (keyval) {
          case GDK_T:
          case GDK_t:
            return 0x11b9;	/* jongseong pieup-sios */
        }
        break;
    }
  }
  return 0;
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

static gboolean
im_hangul2_automata(GtkIMContextHangul *context_hangul,
		    GdkEventKey *key)
{
  guint keyval, state;
  gunichar ch;

  if (pref_use_dvorak) {
    keyval = im_hangul_dvorak_to_qwerty(key->keyval);
    state = key->state;
  } else {
    keyval = key->keyval;
    state = key->state;
  }

  if (context_hangul->state == 0) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      context_hangul->state = 1;
      context_hangul->choseong = ch;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      context_hangul->state = 2;
      context_hangul->choseong = 0;
      context_hangul->jungseong = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key))
      return FALSE;
  } else if (context_hangul->state == 1) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->state = 1;
      context_hangul->choseong = ch;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      context_hangul->jungseong = ch;
      context_hangul->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->choseong = 0;
      context_hangul->state = 0;
      goto done;
    }
  } else if (context_hangul->state == 2) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      if (pref_enable_inverse_sequence) {
        context_hangul->state = 3;
      } else {
        im_hangul_commit(context_hangul);
        context_hangul->state = 1;
      }
      context_hangul->choseong = ch;
      goto done;
    }
    ch = im_hangul2_comp_jungseong(context_hangul->jungseong, keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      context_hangul->jungseong = ch;
      context_hangul->state = 4;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->jungseong = ch;
      context_hangul->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jungseong = 0;
      context_hangul->state = 0;
      goto done;
    }
  } else if (context_hangul->state == 3) {
    ch = im_hangul2_jongseong(keyval, state);
    if (im_hangul_is_jongseong(ch)) {
      context_hangul->jongseong = ch;
      context_hangul->state = 5;
      goto done;
    }
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->choseong = ch;
      context_hangul->state = 1;
      goto done;
    }
    ch = im_hangul2_comp_jungseong(context_hangul->jungseong, keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      context_hangul->jungseong = ch;
      context_hangul->state = 6;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->jungseong = ch;
      context_hangul->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jungseong = 0;
      context_hangul->state = 1;
      goto done;
    }
  } else if (context_hangul->state == 4) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      if (pref_enable_inverse_sequence) {
        context_hangul->state = 3;
      } else {
        im_hangul_commit(context_hangul);
        context_hangul->state = 1;
      }
      context_hangul->choseong = ch;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->jungseong = ch;
      context_hangul->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jungseong = 
		im_hangul_compjungseong_to_single(context_hangul->jungseong);
      context_hangul->state = 2;
      goto done;
    }
  } else if (context_hangul->state == 5) {
    ch = im_hangul2_comp_jongseong(context_hangul->jongseong, keyval, state);
    if (im_hangul_is_jongseong(ch)) {
      context_hangul->jongseong = ch;
      context_hangul->state = 7;
      goto done;
    }
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->choseong = ch;
      context_hangul->state = 1;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(context_hangul->jongseong);
      context_hangul->jongseong = 0;
      im_hangul_commit(context_hangul);
      context_hangul->choseong = choseong;
      context_hangul->jungseong = ch;
      context_hangul->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jongseong = 0;
      context_hangul->state = 3;
      goto done;
    }
  } else if (context_hangul->state == 6) {
    ch = im_hangul2_jongseong(keyval, state);
    if (im_hangul_is_jongseong(ch)) {
      context_hangul->jongseong = ch;
      context_hangul->state = 8;
      goto done;
    }
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->choseong = ch;
      context_hangul->state = 1;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->jungseong = ch;
      context_hangul->state = 2;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jungseong = 
       im_hangul_compjungseong_to_single(context_hangul->jungseong);
      context_hangul->state = 3;
      goto done;
    }
  } else if (context_hangul->state == 7) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->choseong = ch;
      context_hangul->state = 1;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      /* complex final consonant
       * to single final consonant and initial consonant */
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(context_hangul->jongseong);
      context_hangul->jongseong = 
		im_hangul_compjongseong_to_single(context_hangul->jongseong);
      im_hangul_commit(context_hangul);
      context_hangul->choseong = choseong;
      context_hangul->jungseong = ch;
      context_hangul->state = 3;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jongseong = 
		im_hangul_compjongseong_to_single(context_hangul->jongseong);
      context_hangul->state = 5;
      goto done;
    }
  } else if (context_hangul->state == 8) {
    ch = im_hangul2_comp_jongseong(context_hangul->jongseong, keyval, state);
    if (im_hangul_is_jongseong(ch)) {
      context_hangul->jongseong = ch;
      context_hangul->state = 9;
      goto done;
    }
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->choseong = ch;
      context_hangul->state = 1;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(context_hangul->jongseong);
      context_hangul->jongseong = 0;
      im_hangul_commit(context_hangul);
      context_hangul->state = 3;
      context_hangul->choseong = choseong;
      context_hangul->jungseong = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->state = 6;
      context_hangul->jongseong = 0;
      goto done;
    }
  } else if (context_hangul->state == 9) {
    ch = im_hangul2_choseong(keyval, state);
    if (im_hangul_is_choseong(ch)) {
      im_hangul_commit(context_hangul);
      context_hangul->state = 1;
      context_hangul->choseong = ch;
      goto done;
    }
    ch = im_hangul2_jungseong(keyval, state);
    if (im_hangul_is_jungseong(ch)) {
      /* complex final consonant
       * to single final consonant and initial consonant */
      gunichar choseong = 
		im_hangul2_jongseong_to_choseong(context_hangul->jongseong);
      context_hangul->jongseong = 
      		im_hangul_compjongseong_to_single(context_hangul->jongseong);
      im_hangul_commit(context_hangul);
      context_hangul->state = 3;
      context_hangul->choseong = choseong;
      context_hangul->jungseong = ch;
      goto done;
    }
    if (im_hangul_is_backspace(key)) {
      context_hangul->jongseong = 
      		im_hangul_compjongseong_to_single(context_hangul->jongseong);
      context_hangul->state = 8;
      goto done;
    }
  }

  if (im_hangul_is_trigger(key)) {
    /* hangul mode change to englishmode */
    im_hangul_commit(context_hangul);
    g_signal_emit_by_name (context_hangul, "preedit_changed");
    im_hangul_mode_direct(context_hangul);
    return TRUE;
  }

  if (context_hangul->state != 0) {
    im_hangul_commit(context_hangul);
    context_hangul->state = 0;
    g_signal_emit_by_name (context_hangul, "preedit_changed");
  }
  return im_hangul_process_nonhangul(context_hangul, key); /* english */

done:
  g_signal_emit_by_name (context_hangul, "preedit_changed");
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
      GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(context_hangul, im_hangul2_automata);
      return context;
    }
  }
  return NULL;
}

/* vim: nocindent
 */

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

static const GtkIMContextInfo hangul3y_info = {
  "hangul3y",
  "Hangul 3bul Yetgeul",
  "gtk+",
  GTK_LOCALEDIR,
  ""
};

static const GtkIMContextInfo *info_list[] = {
  &hangul3y_info
};

#include "imhangul3.h"

/* return ucs4 code jamo choseong value(U+1100 ~ U+1159) 
 * in 3bul yesgeul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_choseong(guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (keyval) {
      case GDK_K:
      case GDK_k:
        return 0x114E;		/* choseong chitueumcieuc */
      case GDK_I:		                           
      case GDK_i:		                           
        return 0x1154;		/* choseong chitueumchieuch */
      case GDK_N:		                           
      case GDK_n:		                           
        return 0x1140;		/* choseong pansios */
      case GDK_J:		                           
      case GDK_j:		                           
        return 0x114c;		/* choseong yesieung */
      case GDK_L:		                           
      case GDK_l:		                           
        return 0x1150;		/* choseong ceongchieumcieuc */
      case GDK_O:		                           
      case GDK_o:		                           
        return 0x1155;		/* choseong ceongchieumchieuch */
      case GDK_less:		                           
        return 0x113c;		/* choseong chitueumsios */
      case GDK_greater:		                           
        return 0x113e;		/* choseong ceongchieumsios */
      case GDK_M:		                           
      case GDK_m:		                           
        return 0x1159;		/* choseong yeorinhieuh */
      case GDK_H:		                           
      case GDK_h:		                           
        return 0x1102;		/* choseong nieun */
      case GDK_P:		                           
      case GDK_p:		                           
        return 0x1111;		/* choseong phieuph */
    }
  } else {
    switch (keyval) {
      case GDK_K:
      case GDK_k:
        return 0x1100;		/* choseong kiyeok */
      case GDK_H:		                           
      case GDK_h:		                           
        return 0x1102;		/* choseong nieun */
      case GDK_U:		                           
      case GDK_u:		                           
        return 0x1103;		/* choseong tikeut */
      case GDK_Y:		                           
      case GDK_y:		                           
        return 0x1105;		/* choseong rieul */
      case GDK_I:		                           
      case GDK_i:		                           
        return 0x1106;		/* choseong mieum */
      case GDK_semicolon:	                           
        return 0x1107;		/* choseong pieup */
      case GDK_N:		                           
      case GDK_n:		                           
        return 0x1109;		/* choseong sios */
      case GDK_J:		                           
      case GDK_j:		                           
        return 0x110b;		/* choseong ieung */
      case GDK_L:		                           
      case GDK_l:		                           
        return 0x110c;		/* choseong cieuc */
      case GDK_O:		                           
      case GDK_o:		                           
        return 0x110e;		/* choseong chieuch */
      case GDK_0:		                           
        return 0x110f;		/* choseong khieukh */
      case GDK_apostrophe:                           
        return 0x1110;		/* choseong thieuth */
      case GDK_P:		                           
      case GDK_p:		                           
        return 0x1111;		/* choseong phieuph */
      case GDK_M:		                           
      case GDK_m:		                           
        return 0x1112;		/* choseong hieuh */
    }
  }
  return 0;
}

/* return unicode jamo choseong value(U+1100 ~ U+1159) in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_comp_choseong(gunichar ch, guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (ch) {
      case 0x110b:		/* choseong ieung */
        switch (keyval) {
          case GDK_N:
          case GDK_n:
            return 0x1146;	/* choseong ieung-pansios */
        }
        break;
      case 0x113c:		/* choseong chitueumsios */
	switch (keyval) {
          case GDK_less:
            return 0x113d;	/* choseong chitueumssangsios */
        }
        break;
      case 0x113e:		/* choseong cheongchieumsios */
	switch (keyval) {
          case GDK_greater:
            return 0x113f;	/* choseong cheongchieumssangsios */
        }
        break;
      case 0x114e:		/* choseong chitueumcieuc */
	switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x114f;	/* choseong chitueumssangcieuc */
        }
        break;
      case 0x1150:		/* choseong cheongchieumcieuc */
	switch (keyval) {
          case GDK_L:
          case GDK_l:
            return 0x1151;	/* choseong cheongchieumssangcieuc */
        }
        break;
    }
  } else {
    switch (ch) {
      case 0x1100:		/* choseong kiyeok */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x1101;	/* choseong ssangkiyeok */
        }
        break;
      case 0x1102:		/* choseong nieun */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x1113;	/* choseong nieun-kiyeok */
          case GDK_H:
          case GDK_h:
            return 0x1114;	/* choseong ssangnieun */
          case GDK_U:
          case GDK_u:
            return 0x1115;	/* choseong nieun-tikeut */
          case GDK_semicolon:
            return 0x1116;	/* choseong nieun-pieup */
        }
        break;
      case 0x1103:		/* choseong tikeut */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x1117;	/* choseong tikeut-kiyeok */
          case GDK_U:
          case GDK_u:
            return 0x1104;	/* choseong ssangtikeut */
        }
        break;
      case 0x1105:		/* choseong rieul */
        switch (keyval) {
          case GDK_H:
          case GDK_h:
            return 0x1118;	/* choseong rieul-nieun */
          case GDK_Y:
          case GDK_y:
            return 0x1119;	/* choseong ssangrieul */
          case GDK_M:
          case GDK_m:
            return 0x111a;	/* choseong rieul-hieuh */
          case GDK_J:
          case GDK_j:
            return 0x111b;	/* choseong kapyeounrieul */
        }
        break;
      case 0x1106:		/* choseong mieum */
        switch (keyval) {
          case GDK_semicolon:
            return 0x111c;	/* choseong mieum-pieup */
          case GDK_J:
          case GDK_j:
            return 0x111d;	/* choseong kapyeonmieum */
        }
        break;
      case 0x1107:		/* choseong pieup */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x111e;	/* choseong pieup-kiyeok */
          case GDK_H:
          case GDK_h:
            return 0x111f;	/* choseong pieup-nieun */
          case GDK_U:
          case GDK_u:
            return 0x1120;	/* choseong pieup-tikeut */
          case GDK_semicolon:
            return 0x1108;	/* choseong ssangpieup */
          case GDK_N:
          case GDK_n:
            return 0x1121;	/* choseong pieup-sios */
          case GDK_L:
          case GDK_l:
            return 0x1127;	/* choseong pieup-cieuc */
          case GDK_O:
          case GDK_o:
            return 0x1128;	/* choseong pieup-chieuch */
	  case GDK_apostrophe:                           
            return 0x1129;	/* choseong pieup-thieuth */
          case GDK_P:
          case GDK_p:
            return 0x112a;	/* choseong pieup-phieuph */
          case GDK_J:
          case GDK_j:
            return 0x112b;	/* choseong kapyeounpieup */
        }
        break;
      case 0x1109:		/* choseong sios */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x112d;	/* choseong sios-kiyeok */
          case GDK_H:
          case GDK_h:
            return 0x112e;	/* choseong sios-nieun */
          case GDK_U:
          case GDK_u:
            return 0x112f;	/* choseong sios-tikeut */
          case GDK_Y:
          case GDK_y:
            return 0x1130;	/* choseong sios-rieul */
          case GDK_I:
          case GDK_i:
            return 0x1131;	/* choseong sios-mieum */
          case GDK_semicolon:
            return 0x1132;	/* choseong sios-pieup */
          case GDK_N:
          case GDK_n:
            return 0x110a;	/* choseong ssangsios */
          case GDK_L:
          case GDK_l:
            return 0x1136;	/* choseong sios-cieuc */
          case GDK_O:
          case GDK_o:
            return 0x1137;	/* choseong sios-chieuch */
          case GDK_0:
            return 0x1138;	/* choseong sios-khieukh */
	  case GDK_apostrophe:                           
            return 0x1139;	/* choseong sios-thieuth */
          case GDK_P:
          case GDK_p:
            return 0x113a;	/* choseong sios-phieuph */
          case GDK_M:
          case GDK_m:
            return 0x113b;	/* choseong sios-hieuh */
        }
        break;
      case 0x110b:		/* choseong ieung */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x1140;	/* choseong ieung-kiyeok */
          case GDK_U:
          case GDK_u:
            return 0x1142;	/* choseong ieung-tikeut */
          case GDK_Y:
          case GDK_y:
            return 0x1143;	/* choseong ieung-mieum */
          case GDK_semicolon:
            return 0x1144;	/* choseong ieung-pieup */
          case GDK_N:
          case GDK_n:
            return 0x1145;	/* choseong ieung-sios */
          case GDK_J:
          case GDK_j:
            return 0x1147;	/* choseong ssangieung */
          case GDK_L:
          case GDK_l:
            return 0x1148;	/* choseong ieung-cieuc */
          case GDK_O:
          case GDK_o:
            return 0x1149;	/* choseong ieung-chieuch */
	  case GDK_apostrophe:                           
            return 0x114a;	/* choseong ieung-thieuth */
          case GDK_P:
          case GDK_p:
            return 0x114b;	/* choseong ieung-phieuph */
        }
        break;
      case 0x110c:		/* choseong cieuc */
        switch (keyval) {
          case GDK_L:
          case GDK_l:
            return 0x110d;	/* choseong ssangcieuc */
          case GDK_J:
          case GDK_j:
            return 0x114d;	/* choseong cieuc-ieung */
        }
        break;
      case 0x110e:		/* choseong chieuch */
        switch (keyval) {
          case GDK_0:
            return 0x1152;	/* choseong chieuch-khieukh */
          case GDK_M:
          case GDK_m:
            return 0x1153;	/* choseong chieuch-hieuh */
        }
        break;
      case 0x1111:		/* choseong phieuph */
        switch (keyval) {
          case GDK_semicolon:
            return 0x1156;	/* choseong phieuph-pieup */
          case GDK_J:
          case GDK_j:
            return 0x1157;	/* choseong phieuph-ieung */
        }
        break;
      case 0x1112:		/* choseong hieuh */
        switch (keyval) {
          case GDK_M:
          case GDK_m:
            return 0x1158;	/* choseong ssanghieuh */
        }
        break;
      case 0x1121:		/* choseong pieup-sios */
        switch (keyval) {
          case GDK_K:
          case GDK_k:
            return 0x1122;	/* choseong pieup-sios-kiyeok */
          case GDK_U:
          case GDK_u:
            return 0x1123;	/* choseong pieup-sios-tikeut */
          case GDK_semicolon:
            return 0x1124;	/* choseong pieup-sios-pieup */
          case GDK_N:
          case GDK_n:
            return 0x1125;	/* choseong pieup-ssangsios */
          case GDK_L:
          case GDK_l:
            return 0x1126;	/* choseong pieup-sios-cieuc */
        }
        break;
    }
  }
  return 0;
}


/* return unicode jamo jungseong value(U+1161 ~ U+11A2) in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_jungseong(guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (keyval) {
      case GDK_G:
      case GDK_g:
      	return 0x119e;		/* jungseong araea */
      case GDK_R:
      case GDK_r:
      	return 0x1164;		/* jungseong yae */
    }
  } else {
    switch (keyval) {
      case GDK_F:
      case GDK_f:
        return 0x1161;		/* jungseong a */
      case GDK_R:		                      
      case GDK_r:		                      
        return 0x1162;		/* jungseong ae */
      case GDK_6:		                      
        return 0x1163;		/* jungseong ya */
      case GDK_T:		                      
      case GDK_t:		                      
        return 0x1165;		/* jungseong eo */
      case GDK_C:		                      
      case GDK_c:		                      
        return 0x1166;		/* jungseong e */
      case GDK_E:		                      
      case GDK_e:		                      
        return 0x1167;		/* jungseong yeo */
      case GDK_7:		                      
        return 0x1168;		/* jungseong ye */
      case GDK_V:		                      
      case GDK_v:		                      
      case GDK_slash:
        return 0x1169;		/* jungseong o */
      case GDK_4:		                      
        return 0x116d;		/* jungseong yo */
      case GDK_B:		                      
      case GDK_b:		                      
      case GDK_9:	
        return 0x116e;		/* jungseong u */
      case GDK_5:		                      
        return 0x1172;		/* jungseong yu */
      case GDK_G:		                      
      case GDK_g:		                      
        return 0x1173;		/* jungseong eu */
      case GDK_8:		                      
        return 0x1174;		/* jungseong yi */
      case GDK_D:		                      
      case GDK_d:		                      
        return 0x1175;		/* jungseong i */    
    }
  }
  return 0;
}

/* return unicode jamo jungseong value(U+1161 ~ U+11A2) in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_comp_jungseong(gunichar ch, guint keyval, guint state) {
  if (is_set(state, GDK_SHIFT_MASK))
    return 0;
  switch (ch) {
    case 0x1169:		/* jungseong o */
      switch (keyval) {
        case GDK_F:	
        case GDK_f:	
          return 0x116a;	/* jungseong wa */
        case GDK_R:	        
        case GDK_r:	        
          return 0x116b;        /* jungseong wae */
        case GDK_D:	        
        case GDK_d:	        
          return 0x116c;        /* jungseong oe */
      }
      break;
    case 0x116e:		/* jungseong u */
      switch (keyval) {
        case GDK_T:	
        case GDK_t:	
          return 0x116f;	/* jungseong weo */
        case GDK_C:	        
        case GDK_c:	        
          return 0x1170;        /* jungseong we */
        case GDK_D:	        
        case GDK_d:	        
          return 0x1171;        /* jungseong wi */  
      }
      break;
    case 0x1173:		/* jungseong eu */
      switch (keyval) {
        case GDK_D:
        case GDK_d:
          return 0x1174;	/* jungseong yi */
      }
  }
  return 0;
}

/* return unicode jamo jongseong value(U+11A8 ~ U+11F9) in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_jongseong(guint keyval, guint state)
{
  if (is_set(state, GDK_SHIFT_MASK)) { 
    switch (keyval) {
      case GDK_X:
      case GDK_x:
        return 0x11b9;    /* jongseong pieup-sios */
      case GDK_F:
      case GDK_f:
        return 0x11a9;    /* jongseong ssangkiyeok */
      case GDK_S:
      case GDK_s:
        return 0x11ad;    /* jongseong nieun-hieuh */
      case GDK_A:
      case GDK_a:
        return 0x11ae;    /* jongseong tikeut */
      case GDK_W:
      case GDK_w:
        return 0x11c0;    /* jongseong thieuth */
      case GDK_D:
      case GDK_d:
        return 0x11b0;    /* jongseong rieul-kiyeok */
      case GDK_C:
      case GDK_c:
        return 0x11b1;    /* jongseong rieul-mieum */
      case GDK_V:
      case GDK_v:
        return 0x11b6;    /* jongseong rieul-hieuh */
      case GDK_Z:
      case GDK_z:
        return 0x11be;    /* jongseong chieuch */
      case GDK_Q:
      case GDK_q:
        return 0x11c1;    /* jongseong phieuph */
      case GDK_exclam:
        return 0x11bd;    /* jongseong cieuc */
      case GDK_E:
      case GDK_e:
        return 0x11bf;    /* jongseong khieukh */
      case GDK_at:
        return 0x11eb;    /* jongseong pansios */
      case GDK_asciitilde:
        return 0x11f0;    /* jongseong yesieung */
    }
  } else {
    switch (keyval) {
      case GDK_X:
      case GDK_x:
        return 0x11a8;    /* jongseong kiyeok */
      case GDK_S:
      case GDK_s:
        return 0x11ab;    /* jongseong nieun */
      case GDK_W:
      case GDK_w:
        return 0x11af;    /* jongseong rieul */
      case GDK_Z:
      case GDK_z:
        return 0x11b7;    /* jongseong mieum */
      case GDK_3:
        return 0x11b8;    /* jongseong pieup */
      case GDK_Q:
      case GDK_q:
        return 0x11ba;    /* jongseong sios */
      case GDK_2:
        return 0x11bb;    /* jongseong ssangsios */
      case GDK_A:
      case GDK_a:
        return 0x11bc;    /* jongseong ieung */
      case GDK_1:
        return 0x11c2;    /* jongseong hieuh */
      case GDK_grave:
        return 0x11f9;    /* jongseong yeorinhieuh */
    }
  }
  return 0;
}

/* return unicode jamo jongseong value(U+11a8 ~ U+11c2) in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_comp_jongseong(gunichar ch, guint keyval, guint state)
{
  if (is_set(state, GDK_SHIFT_MASK)) {
    switch (ch) {
      case 0x11ab:		/* jongseong nieun */
        switch (keyval) {
	  case GDK_A:
	  case GDK_a:
            return 0x11c6;	/* jongseong nieun-tikeut */
          case GDK_exclam:
            return 0x11ac;	/* jongseong nieun-cieuc */
          case GDK_at:
            return 0x11c8;	/* jongseong nieun-pansios */
	  case GDK_W:
	  case GDK_w:
            return 0x11c9;	/* jongseong nieun-thieuth */
	}
	break;
      case 0x11af:		/* jongseong rieul */
        switch (keyval) {
          case GDK_A:
          case GDK_a:
            return 0x11ce;	/* jongseong rieul-tikeut */
          case GDK_at:
            return 0x11d7;	/* jongseong rieul-pansios */
          case GDK_E:
          case GDK_e:
            return 0x11d8;	/* jongseong rieul-khieukh */
          case GDK_W:
          case GDK_w:
            return 0x11b4;	/* jongseong rieul-thieuth */
          case GDK_Q:
          case GDK_q:
            return 0x11b5;	/* jongseong rieul-phieuph */
        }
	break;
      case 0x11b7:		/* jongseong mieum */
        switch (keyval) {
          case GDK_at:
            return 0x11df;	/* jongseong mieum-pansios */
          case GDK_Z:
          case GDK_z:
            return 0x11e0;	/* jongseong mieum-chieuch */
	}
	break;
      case 0x11b8:		/* jongseong pieup */
	switch (keyval) {
          case GDK_Q:
          case GDK_q:
            return 0x11e4;	/* jongseong pieup-phieuph */
        }
	break;
      case 0x11ba:		/* jongseong sios */
	switch (keyval) {
          case GDK_A:
          case GDK_a:
            return 0x11e8;	/* jongseong sios-tikeut */
        }
	break;
      case 0x11bc:		/* jongseong ieung */
	switch (keyval) {
          case GDK_E:
          case GDK_e:
            return 0x11ef;	/* jongseong ieung-khiyeokh */
        }
	break;
      case 0x11f0:		/* jongseong yesieung */
	switch (keyval) {
          case GDK_at:
            return 0x11f2;	/* jongseong yesieung-pansios */
        }
	break;
    }
  } else {
    switch (ch) {
      case 0x11a8:		/* jongseong kiyeok */
        switch (keyval) {
          case GDK_X:
	  case GDK_x:
            return 0x11a9;	/* jongseong ssangkiyeok */
	  case GDK_W:
	  case GDK_w:
            return 0x11c3;	/* jongseong kiyeok-rieul */
	  case GDK_Q:
	  case GDK_q:
            return 0x11aa;	/* jongseong kiyeok-sios */
	}
	break;
      case 0x11ab:		/* jongseong nieun */
        switch (keyval) {
	  case GDK_X:
	  case GDK_x:
            return 0x11c5;	/* jongseong nieun-kiyeok */
	  case GDK_Q:
	  case GDK_q:
            return 0x11c7;	/* jongseong nieun-sios */
	  case GDK_1:
            return 0x11ad;	/* jongseong nieun-hieuh */
	}
	break;
      case 0x11ae:		/* jongseong tikeut */
        switch (keyval) {
	  case GDK_X:
	  case GDK_x:
            return 0x11ca;	/* jongseong tikeut-kiyeok */
	  case GDK_W:
	  case GDK_w:
            return 0x11cb;	/* jongseong tikeut-rieul */
	}
	break;
      case 0x11af:		/* jongseong rieul */
        switch (keyval) {
          case GDK_X:
          case GDK_x:
            return 0x11b0;	/* jongseong rieul-kiyeok */
          case GDK_S:
          case GDK_s:
            return 0x11cd;	/* jongseong rieul-nieun */
          case GDK_Z:
          case GDK_z:
            return 0x11b1;	/* jongseong rieul-mieum */
          case GDK_3:
            return 0x11b2;	/* jongseong rieul-pieup */
          case GDK_Q:
          case GDK_q:
            return 0x11b3;	/* jongseong rieul-sios */
          case GDK_1:
            return 0x11b6;	/* jongseong rieul-hieuh */
          case GDK_grave:
            return 0x11d9;	/* jongseong rieul-yeorinhieuh */
	}
	break;
      case 0x11b7:		/* jongseong mieum */
        switch (keyval) {
          case GDK_X:
          case GDK_x:
            return 0x11da;	/* jongseong mieum-kiyeok */
          case GDK_W:
          case GDK_w:
            return 0x11db;	/* jongseong mieum-rieul */
          case GDK_3:
            return 0x11dc;	/* jongseong mieum-pieup */
          case GDK_Q:
          case GDK_q:
            return 0x11dd;	/* jongseong mieum-sios */
          case GDK_1:
            return 0x11e1;	/* jongseong mieum-hieuh */
          case GDK_A:
          case GDK_a:
            return 0x11e2;	/* jongseong kapyeounmieum */
	}
	break;
      case 0x11b8:		/* jongseong pieup */
	switch (keyval) {
          case GDK_W:
          case GDK_w:
            return 0x11e3;	/* jongseong pieup-rieul */
          case GDK_Q:
          case GDK_q:
            return 0x11b9;	/* jongseong pieup-sios */
          case GDK_1:
            return 0x11e5;	/* jongseong pieup-hieuh */
        }
	break;
      case 0x11ba:		/* jongseong sios */
	switch (keyval) {
          case GDK_X:
          case GDK_x:
            return 0x11e7;	/* jongseong sios-kiyeok */
          case GDK_W:
          case GDK_w:
            return 0x11e9;	/* jongseong sios-rieul */
          case GDK_3:
            return 0x11ea;	/* jongseong sios-pieup */
          case GDK_Q:
          case GDK_q:
            return 0x11bb;	/* jongseong ssangsios */
        }
	break;
      case 0x11bc:		/* jongseong ieung */
	switch (keyval) {
          case GDK_X:
          case GDK_x:
            return 0x11e7;	/* jongseong ieung-kiyeok */
          case GDK_A:
          case GDK_a:
            return 0x11ee;	/* jongseong ssangieung */
        }
	break;
      case 0x11f0:		/* jongseong yesieung */
	switch (keyval) {
          case GDK_Q:
          case GDK_q:
            return 0x11f1;	/* jongseong yesieung-sios */
        }
	break;
      case 0x11c1:		/* jongseong phieuph */
	switch (keyval) {
          case GDK_3:
            return 0x11f3;	/* jongseong phieuph-pieup */
          case GDK_A:
          case GDK_a:
            return 0x11f4;	/* jongseong kapyeounphieuph */
        }
	break;
      case 0x11c2:		/* jongseong hieuh */
	switch (keyval) {
          case GDK_S:
          case GDK_s:
            return 0x11f5;	/* jongseong hieuh-nieun */
          case GDK_W:
          case GDK_w:
            return 0x11f6;	/* jongseong hieuh-rieul */
          case GDK_Z:
          case GDK_z:
            return 0x11f7;	/* jongseong hieuh-mieum */
          case GDK_3:
            return 0x11f8;	/* jongseong hieuh-pieup */
        }
	break;
    }
  }
  return 0;
}

/* return unicode number and punctuation value in 3bul keyboard
 * if it is not hangul key, return 0 */
static gunichar
im_hangul3y_punct(guint keyval, guint state)
{
  return 0;
}

void
im_module_init(GTypeModule *type_module)
{
  im_hangul_register_type(type_module, "GtkIMContextHangul3y");
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
  if (strcmp (context_id, "hangul3y") == 0) {
    GtkIMContext *context = im_hangul_new();
    if (context) {
      GtkIMContextHangul *context_hangul = GTK_IM_CONTEXT_HANGUL(context);
      im_hangul_set_automata(context_hangul, im_hangul3_automata);

      im_hangul3_choseong = im_hangul3y_choseong;
      im_hangul3_jungseong = im_hangul3y_jungseong;
      im_hangul3_jongseong = im_hangul3y_jongseong;
      im_hangul3_punct = im_hangul3y_punct;
      im_hangul3_comp_choseong = im_hangul3y_comp_choseong;
      im_hangul3_comp_jungseong = im_hangul3y_comp_jungseong;
      im_hangul3_comp_jongseong = im_hangul3y_comp_jongseong;
      return context;
    }
  }
  return NULL;
}

/* vim: nocindent
 */

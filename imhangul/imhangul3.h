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


/* we dont use this func anymore
static gunichar	im_hangul_compchoseong_to_single	(gunichar ch);
*/

static gunichar	im_hangul_pop	(GtkIMContextHangul *context_hangul);
static gunichar	im_hangul_peek	(GtkIMContextHangul *context_hangul);
static void	im_hangul_push	(GtkIMContextHangul *context_hangul, gunichar ch);

static void	im_hangul_commit_unicode(GtkIMContextHangul *context_hangul,
					 gunichar ch);

static gunichar (*im_hangul3_choseong)		(guint, guint);
static gunichar (*im_hangul3_jungseong)		(guint, guint);
static gunichar (*im_hangul3_jongseong)		(guint, guint);
static gunichar (*im_hangul3_punct)		(guint, guint);
static gunichar (*im_hangul3_comp_choseong)	(gunichar, guint, guint);
static gunichar (*im_hangul3_comp_jungseong)	(gunichar, guint, guint);
static gunichar (*im_hangul3_comp_jongseong)	(gunichar, guint, guint);

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


#if 0
/* this funcs used for backspace in hangul 3 set keyboard */
static gunichar
im_hangul_compchoseong_to_single(gunichar ch)
{
  switch (ch) {
    case 0x1101:	/* hangul choseong ssangkiyeok */
      return 0x1100;	/* hangul choseong kiyeok */
    case 0x1104:	/* hangul choseong ssangtikeut */
    case 0x1117:	/* hangul choseong tikeut-kiyeok */
      return 0x1103;	/* hangul choseong tikeut */
    case 0x1108:	/* hangul choseong ssangpieup */
    case 0x111e:	/* hangul choseong pieup-kiyeok */
    case 0x111f:	/* hangul choseong pieup-nieun */
    case 0x1120:	/* hangul choseong pieup-tikeut */
    case 0x1121:	/* hangul choseong pieup-sios */
    case 0x1127:	/* hangul choseong pieup-cieuc */
    case 0x1128:	/* hangul choseong pieup-chieuch */
    case 0x1129:	/* hangul choseong pieup-thieuth */
    case 0x112a:	/* hangul choseong pieup-phieuph */
    case 0x112b:	/* hangul choseong kapyeounpieup */
      return 0x1107;	/* hangul choseong pieup */
    case 0x110a:	/* hangul choseong ssangsios */
    case 0x112d:	/* hangul choseong sios-kiyeok */
    case 0x112e:	/* hangul choseong sios-nieun */
    case 0x112f:	/* hangul choseong sios-tikeut */
    case 0x1130:	/* hangul choseong sios-rieul */
    case 0x1131:	/* hangul choseong sios-mieum */
    case 0x1132:	/* hangul choseong sios-pieup */
    case 0x1135:	/* hangul choseong sios-ieung */
    case 0x1136:	/* hangul choseong sios-cieuc */
    case 0x1137:	/* hangul choseong sios-chieuch */
    case 0x1138:	/* hangul choseong sios-khieukh */
    case 0x1139:	/* hangul choseong sios-thieuth */
    case 0x113a:	/* hangul choseong sios-phieuph */
    case 0x113b:	/* hangul choseong sios-hieuh */
      return 0x1109;	/* hangul choseong sios */
    case 0x110d:	/* hangul choseong ssangcieuc */
      return 0x110c;	/* hangul choseong cieuc */
    case 0x1113:	/* hangul choseong nieun-kiyeok */
    case 0x1114:	/* hangul choseong ssangnieun */
    case 0x1115:	/* hangul choseong nieun-tikeut */
    case 0x1116:	/* hangul choseong nieun-pieup */
      return 0x1102;	/* hangul choseong nieun */
    case 0x1118:	/* hangul choseong rieul-neiun */
    case 0x1119:	/* hangul choseong ssangrieul */
    case 0x111a:	/* hangul choseong rieul-hieuh */
    case 0x111b:	/* hangul choseong kapyeounrieul */
      return 0x1105;	/* hangul choseong rieul */
    case 0x111c:	/* hangul choseong mieum-pieup */
    case 0x111d:	/* hangul choseong kapyeounmieum */
      return 0x1106;	/* hangul choseong mieum */
    case 0x1122:	/* hangul choseong pieup-sios-kiyeok */
    case 0x1123:	/* hangul choseong pieup-sios-tikeut */
    case 0x1124:	/* hangul choseong pieup-sios-pieup */
    case 0x1125:	/* hangul choseong pieup-ssangsois */
    case 0x1126:	/* hangul choseong pieup-sios-cieuc */
      return 0x1121;	/* hangul choseong pieup-sios */
  }
  return 0;
}

#endif

static gboolean
im_hangul3_automata(GtkIMContextHangul *context_hangul,
		    GdkEventKey *key)
{
  gunichar ch;
  guint keyval, state;

  if (pref_use_dvorak) {
    keyval = im_hangul_dvorak_to_qwerty(key->keyval);
    state = key->state;
  } else {
    keyval = key->keyval;
    state = key->state;
  }

  /* choseong */
  ch = im_hangul3_choseong(keyval, state);
  if (im_hangul_is_choseong(ch)) {
    gunichar choseong;
    if (context_hangul->choseong == 0) {
      context_hangul->choseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_choseong(im_hangul_peek(context_hangul))) {
      choseong = im_hangul3_comp_choseong(context_hangul->choseong,
					  keyval, state);
      if (choseong) {
        context_hangul->choseong = choseong;
        im_hangul_push(context_hangul, choseong);
        goto done;
      }
    }
    im_hangul_commit(context_hangul);
    context_hangul->choseong = ch;
    im_hangul_push(context_hangul, ch);
    goto done;
  }

  /* junseong */
  ch = im_hangul3_jungseong(keyval, state);
  if (im_hangul_is_jungseong(ch)) {
    gunichar jungseong;
    if (context_hangul->jungseong == 0) {
      context_hangul->jungseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_jungseong(im_hangul_peek(context_hangul))) {
      jungseong = im_hangul3_comp_jungseong(context_hangul->jungseong,
					    keyval, state);
      if (jungseong) {
        context_hangul->jungseong = jungseong;
        im_hangul_push(context_hangul, jungseong);
        goto done;
      }
    }
    im_hangul_commit(context_hangul);
    context_hangul->jungseong = ch;
    im_hangul_push(context_hangul, ch);
    goto done;
  }

  /* jongseong */
  ch = im_hangul3_jongseong(keyval, state);
  if (im_hangul_is_jongseong(ch)) {
    if (context_hangul->jongseong == 0) {
      context_hangul->jongseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_jongseong(im_hangul_peek(context_hangul))) {
      gunichar jongseong;
      jongseong = im_hangul3_comp_jongseong(context_hangul->jongseong,
					    keyval, state);
      if (jongseong) {
        context_hangul->jongseong = jongseong;
        im_hangul_push(context_hangul, jongseong);
        goto done;
      }
    }
    im_hangul_commit(context_hangul);
    context_hangul->jongseong = ch;
    im_hangul_push(context_hangul, ch);
    goto done;
  }

  /* number and puctuation */
  ch = im_hangul3_punct(keyval, state);
  if (ch) {
    im_hangul_commit(context_hangul);
    im_hangul_commit_unicode(context_hangul, ch);
    context_hangul->state = 0;
    goto done;
  }

  /* backspace
  if (im_hangul_is_backspace(key)) {
    ch = im_hangul_pop(context_hangul);
    if (ch == 0)
      return FALSE;
    if (context_hangul->choseong == ch) {
      context_hangul->choseong = im_hangul_compchoseong_to_single(ch);
      goto done;
    }
    if (context_hangul->jungseong == ch) {
      context_hangul->jungseong = im_hangul_compjungseong_to_single(ch);
      if (context_hangul->jungseong != im_hangul_peek(context_hangul))
        context_hangul->jungseong = 0;
      goto done;
    }
    if (context_hangul->jongseong == ch) {
      context_hangul->jongseong = im_hangul_compjongseong_to_single(ch);
      if (context_hangul->jongseong != im_hangul_peek(context_hangul))
        context_hangul->jongseong = 0;
      goto done;
    }
    return FALSE;
  }
  */

  /* new backspace treat routine */
  if (im_hangul_is_backspace(key)) {
    ch = im_hangul_pop(context_hangul);
    if (ch == 0)
      return FALSE;
    if (context_hangul->choseong == ch) {
      ch = im_hangul_peek(context_hangul);
      context_hangul->choseong = im_hangul_is_choseong(ch) ? ch : 0;
      goto done;
    }
    if (context_hangul->jungseong == ch) {
      ch = im_hangul_peek(context_hangul);
      context_hangul->jungseong = im_hangul_is_jungseong(ch) ? ch : 0;
      goto done;
    }
    if (context_hangul->jongseong == ch) {
      ch = im_hangul_peek(context_hangul);
      context_hangul->jongseong = im_hangul_is_jongseong(ch) ? ch : 0;
      goto done;
    }
    return FALSE;
  }

  if (im_hangul_is_trigger(key)) {
    im_hangul_commit(context_hangul);
    g_signal_emit_by_name(context_hangul, "preedit_changed");
    im_hangul_mode_direct(context_hangul);
    return TRUE;
  }

  if (context_hangul->index >= 0) {
    im_hangul_commit(context_hangul);
    g_signal_emit_by_name(context_hangul, "preedit_changed");
  }
  return im_hangul_process_nonhangul(context_hangul, key);

done:
  g_signal_emit_by_name(context_hangul, "preedit_changed");
  return TRUE;
}

/* vim: set nocindent: */

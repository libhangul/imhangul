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

static const gunichar *keyboard_table;
/*
static gunichar (*im_hangul3_choseong)		(guint, guint);
static gunichar (*im_hangul3_jungseong)		(guint, guint);
static gunichar (*im_hangul3_jongseong)		(guint, guint);
static gunichar (*im_hangul3_punct)		(guint, guint);
static gunichar (*im_hangul3_comp_choseong)	(gunichar, gunichar);
static gunichar (*im_hangul3_comp_jungseong)	(gunichar, gunichar);
static gunichar (*im_hangul3_comp_jongseong)	(gunichar, gunichar);
*/

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

static gunichar
im_hangul3_mapping(guint keyval, guint state)
{
  if (keyboard_table == NULL)
    return 0;

  /*  hangul jamo keysym */
  if (keyval >= 0x01001100 && keyval <= 0x010011ff)
    return keyval & 0x0000ffff;

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

  if (keyval >= GDK_exclam  && keyval <= GDK_asciitilde)
    return keyboard_table[keyval - GDK_exclam];
  else
    return 0;
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

  ch = im_hangul3_mapping(keyval, state);

  /* choseong */
  if (im_hangul_is_choseong(ch)) {
    gunichar choseong;
    if (context_hangul->choseong == 0) {
      context_hangul->choseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_choseong(im_hangul_peek(context_hangul))) {
      choseong = im_hangul_compose(context_hangul->choseong, ch);
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
  if (im_hangul_is_jungseong(ch)) {
    gunichar jungseong;
    if (context_hangul->jungseong == 0) {
      context_hangul->jungseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_jungseong(im_hangul_peek(context_hangul))) {
      jungseong = im_hangul_compose(context_hangul->jungseong, ch);
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
  if (im_hangul_is_jongseong(ch)) {
    if (context_hangul->jongseong == 0) {
      context_hangul->jongseong = ch;
      im_hangul_push(context_hangul, ch);
      goto done;
    }
    if (im_hangul_is_jongseong(im_hangul_peek(context_hangul))) {
      gunichar jongseong;
      jongseong = im_hangul_compose(context_hangul->jongseong, ch);
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
  if (ch > 0) {
    im_hangul_commit(context_hangul);
    im_hangul_commit_unicode(context_hangul, ch);
    context_hangul->state = 0;
    goto done;
  }

  /* treat backspace */
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

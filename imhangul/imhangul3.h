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
static gunichar (*im_hangul3_choseong)		(guint, guint);
static gunichar (*im_hangul3_jungseong)		(guint, guint);
static gunichar (*im_hangul3_jongseong)		(guint, guint);
static gunichar (*im_hangul3_punct)		(guint, guint);
static gunichar (*im_hangul3_comp_choseong)	(gunichar, guint, guint);
static gunichar (*im_hangul3_comp_jungseong)	(gunichar, guint, guint);
static gunichar (*im_hangul3_comp_jongseong)	(gunichar, guint, guint);

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

  /* backspace */
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

/* vim: nocindent
 */

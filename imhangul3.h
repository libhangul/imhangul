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

static void	im_hangul_commit_unicode(GtkIMContextHangul *hcontext,
					 gunichar ch);

static gboolean
im_hangul_add_choseong(GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->lindex >= 3)
    return FALSE;
  hcontext->lindex++;
  hcontext->choseong[hcontext->lindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_add_jungseong(GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->vindex >= 3)
    return FALSE;
  hcontext->vindex++;
  hcontext->jungseong[hcontext->vindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_add_jongseong(GtkIMContextHangul *hcontext, gunichar ch)
{
  if (hcontext->tindex >= 3)
    return FALSE;
  hcontext->tindex++;
  hcontext->jongseong[hcontext->tindex] = ch;
  return TRUE;
}

static gboolean
im_hangul_sub_choseong(GtkIMContextHangul *hcontext)
{
  hcontext->choseong[hcontext->lindex] = 0;
  if (hcontext->lindex <= 0)
    return FALSE;
  hcontext->lindex--;
  return TRUE;
}

static gboolean
im_hangul_sub_jungseong(GtkIMContextHangul *hcontext)
{
  hcontext->jungseong[hcontext->vindex] = 0;
  if (hcontext->vindex <= 0)
    return FALSE;
  hcontext->vindex--;
  return TRUE;
}

static gboolean
im_hangul_sub_jongseong(GtkIMContextHangul *hcontext)
{
  hcontext->jongseong[hcontext->tindex] = 0;
  if (hcontext->tindex <= 0)
    return FALSE;
  hcontext->tindex--;
  return TRUE;
}

static void
im_hangul_commit_unicode(GtkIMContextHangul *hcontext, gunichar ch)
{
  int n;
  gchar buf[6];

  n = g_unichar_to_utf8(ch, buf);
  buf[n] = '\0';

  im_hangul_clear_buf(hcontext);

  g_signal_emit_by_name(hcontext, "commit", buf);
}

static gboolean
im_hangul3_automata(GtkIMContextHangul *hcontext,
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

  ch = im_hangul_mapping(keyval, state);

  if (manual_mode) {
    if (hcontext->jongseong[0]) {
      if (im_hangul_is_choseong(ch)) {
        im_hangul_commit(hcontext);
        hcontext->choseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        im_hangul_commit(hcontext);
        hcontext->jungseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        if (!im_hangul_add_jongseong(hcontext, ch)) {
          im_hangul_commit(hcontext);
          hcontext->jongseong[0] = ch;
        }
        im_hangul_push(hcontext, ch);
        goto done;
      }
    } else if (hcontext->jungseong[0]) {
      if (im_hangul_is_choseong(ch)) {
        im_hangul_commit(hcontext);
        hcontext->choseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        if (!im_hangul_add_jungseong(hcontext, ch)) {
          im_hangul_commit(hcontext);
          hcontext->jungseong[0] = ch;
        }
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        hcontext->jongseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
    } else if (hcontext->choseong[0]) {
      if (im_hangul_is_choseong(ch)) {
        if (!im_hangul_add_choseong(hcontext, ch)) {
          im_hangul_commit(hcontext);
          hcontext->choseong[0] = ch;
        }
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        hcontext->jungseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        im_hangul_commit(hcontext);
        hcontext->jongseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
    } else {
      if (im_hangul_is_choseong(ch)) {
        hcontext->choseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        hcontext->jungseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        hcontext->jongseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
    }

    /* treat backspace */
    if (im_hangul_is_backspace(key)) {
      ch = im_hangul_pop(hcontext);
      if (ch == 0)
        return FALSE;
 
      if (im_hangul_is_choseong(ch)) {
        im_hangul_sub_choseong(hcontext);
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        im_hangul_sub_jungseong(hcontext);
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        im_hangul_sub_jongseong(hcontext);
        goto done;
      }
      return FALSE;
    }
  } else {
    /* choseong */
    if (im_hangul_is_choseong(ch)) {
      if (hcontext->choseong[0] == 0) {
        hcontext->choseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_choseong(im_hangul_peek(hcontext))) {
        gunichar choseong = im_hangul_compose(hcontext->choseong[0], ch);
        if (choseong) {
          hcontext->choseong[0] = choseong;
          im_hangul_push(hcontext, choseong);
          goto done;
        }
      }
      im_hangul_commit(hcontext);
      hcontext->choseong[0] = ch;
      im_hangul_push(hcontext, ch);
      goto done;
    }
    /* junseong */
    if (im_hangul_is_jungseong(ch)) {
      if (hcontext->jungseong[0] == 0) {
        hcontext->jungseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jungseong(im_hangul_peek(hcontext))) {
        gunichar jungseong = im_hangul_compose(hcontext->jungseong[0], ch);
        if (jungseong) {
          hcontext->jungseong[0] = jungseong;
          im_hangul_push(hcontext, jungseong);
          goto done;
        }
      }
      im_hangul_commit(hcontext);
      hcontext->jungseong[0] = ch;
      im_hangul_push(hcontext, ch);
      goto done;
    }
    /* jongseong */
    if (im_hangul_is_jongseong(ch)) {
      if (hcontext->jongseong[0] == 0) {
        hcontext->jongseong[0] = ch;
        im_hangul_push(hcontext, ch);
        goto done;
      }
      if (im_hangul_is_jongseong(im_hangul_peek(hcontext))) {
        gunichar jongseong = im_hangul_compose(hcontext->jongseong[0], ch);
        if (jongseong) {
          hcontext->jongseong[0] = jongseong;
          im_hangul_push(hcontext, jongseong);
          goto done;
        }
      }
      im_hangul_commit(hcontext);
      hcontext->jongseong[0] = ch;
      im_hangul_push(hcontext, ch);
      goto done;
    }
    /* treat backspace */
    if (im_hangul_is_backspace(key)) {
      ch = im_hangul_pop(hcontext);
      if (ch == 0)
        return FALSE;

      if (im_hangul_is_choseong(ch)) {
        ch = im_hangul_peek(hcontext);
        hcontext->choseong[0] = im_hangul_is_choseong(ch) ? ch : 0;
        goto done;
      }
      if (im_hangul_is_jungseong(ch)) {
        ch = im_hangul_peek(hcontext);
        hcontext->jungseong[0] = im_hangul_is_jungseong(ch) ? ch : 0;
        goto done;
      }
      if (im_hangul_is_jongseong(ch)) {
        ch = im_hangul_peek(hcontext);
        hcontext->jongseong[0] = im_hangul_is_jongseong(ch) ? ch : 0;
        goto done;
      }
      return FALSE;
    }
  }

  /* number and puctuation */
  if (ch > 0) {
    im_hangul_commit(hcontext);
    im_hangul_commit_unicode(hcontext, ch);
    hcontext->state = STATE_HANGUL;
    goto done;
  }

  if (im_hangul_is_trigger(key)) {
    im_hangul_commit(hcontext);
    g_signal_emit_by_name(hcontext, "preedit_changed");
    im_hangul_mode_direct(hcontext);
    return TRUE;
  }

  if (hcontext->index >= 0) {
    im_hangul_commit(hcontext);
    g_signal_emit_by_name(hcontext, "preedit_changed");
  }
  return im_hangul_process_nonhangul(hcontext, key);

done:
  g_signal_emit_by_name(hcontext, "preedit_changed");
  return TRUE;
}

/* vim: set nocindent: */

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
static GtkIMContext *im_hangul_new_roma   (void);

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

static const GtkIMContextInfo hangulro_info = {
  "hangulro",
  N_("Hangul Romaja"),
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
  &hangul3y_info,
  &hangulro_info
};

void
im_module_init (GTypeModule *type_module)
{
  gtk_im_context_hangul_register_type (type_module);
  im_hangul_init();
}

void
im_module_exit (void)
{
  im_hangul_finalize ();
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
  } else if (strcmp (context_id, "hangulro") == 0) {
    return im_hangul_new_roma ();
  }

  g_warning("imhangul:unknown context id: %s", context_id); 
  g_assert_not_reached ();

  return NULL;
}

static GtkIMContext *
im_hangul_new_2 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "2");

  return context;
}

static GtkIMContext *
im_hangul_new_32 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "32");

  return context;
}

static GtkIMContext *
im_hangul_new_390 (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "39");

  return context;
}

static GtkIMContext *
im_hangul_new_3final (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "3f");

  return context;
}

static GtkIMContext *
im_hangul_new_3sun (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "3s");

  return context;
}

static GtkIMContext *
im_hangul_new_3yet (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "3y");

  return context;
}

static GtkIMContext *
im_hangul_new_roma (void)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, "ro");

  return context;
}

/* vim: set sw=2 : */

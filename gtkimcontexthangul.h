/* ImHangul - Gtk+ 2.0 Input Method Module for Hangul
 * Copyright (C) 2002-2008 Choe Hwanjin
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

#ifndef __GTK_IM_CONTEXT_HANGUL_H__
#define __GTK_IM_CONTEXT_HANGUL_H__

#include <hangul.h>
#include <gtk/gtk.h>

extern GType gtk_type_im_context_hangul;

#define GTK_TYPE_IM_CONTEXT_HANGUL	    (gtk_type_im_context_hangul)
#define GTK_IM_CONTEXT_HANGUL(obj)	    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_IM_CONTEXT_HANGUL, GtkIMContextHangul))
#define GTK_IS_IM_CONTEXT_HANGUL(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_IM_CONTEXT_HANGUL))

typedef struct _GtkIMContextHangul	GtkIMContextHangul;
typedef struct _GtkIMContextHangulClass	GtkIMContextHangulClass;
typedef struct _Candidate               Candidate;
typedef struct _Toplevel                Toplevel;

typedef enum
{
  IM_HANGUL_COMPOSER_2,
  IM_HANGUL_COMPOSER_3,
} IMHangulComposerType;

struct _GtkIMContextHangul
{
  GtkIMContext object;

  /* default input module: simple */
  GtkIMContext *slave;
  gboolean slave_preedit_started;

  /* window */
  GdkWindow *client_window;
  Toplevel *toplevel;
  GdkRectangle cursor;

  /* hangul ic */
  HangulInputContext* hic;
  GString* preedit;

  /* candidate data */
  Candidate *candidate;
  GArray *candidate_string;

  /* options */
  gboolean use_preedit : 1;
};

struct _GtkIMContextHangulClass
{
  GtkIMContextClass parent_class;
};

void          gtk_im_context_hangul_register_type (GTypeModule *type_module);
void          im_hangul_init (void);
void          im_hangul_finalize (void);

GtkIMContext *gtk_im_context_hangul_new      (void);

/* configuration */
void gtk_im_context_hangul_select_keyboard(GtkIMContextHangul *hcontext,
		                           const char         *keyboard);

#endif /* __GTK_IM_CONTEXT_HANGUL_H__ */

/* vim: set sw=2 : */

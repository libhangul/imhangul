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
 */

#ifndef __GTK_IM_CONTEXT_HANGUL_H__
#define __GTK_IM_CONTEXT_HANGUL_H__

#include <gtk/gtkimcontext.h>


extern GType gtk_type_im_context_hangul;

#define GTK_TYPE_IM_CONTEXT_HANGUL	gtk_type_im_context_hangul
#define GTK_IM_CONTEXT_HANGUL(obj)	(GTK_CHECK_CAST ((obj), GTK_TYPE_IM_CONTEXT_HANGUL, GtkIMContextHangul))
#define GTK_IS_IM_CONTEXT_HANGUL(obj)   (GTK_CHECK_TYPE ((obj), GTK_TYPE_IM_CONTEXT_HANGUL))

typedef struct _GtkIMContextHangul	GtkIMContextHangul;
typedef struct _GtkIMContextHangulClass	GtkIMContextHangulClass;

typedef struct _Candidate               Candidate;
typedef struct _IMHangulCombination	IMHangulCombination;
typedef gboolean (*IMHangulComposer)   (GtkIMContextHangul *, GdkEventKey *);

typedef enum
{
  IM_HANGUL_COMPOSER_2,
  IM_HANGUL_COMPOSER_3,
} IMHangulComposerType;

struct _GtkIMContextHangul
{
  GtkIMContext object;

  /* hangul keyboard information */
  IMHangulComposer composer;
  const gunichar *keyboard_table;
  int compose_table_size;
  const IMHangulCombination *compose_table;

  /* window */
  GdkWindow *client_window;
  GtkWidget *toplevel;
  Candidate *candidate;

  /* hangul buffer */
  int input_mode;
  int index;			/* stack index */
  gunichar stack[12];

  int lindex;			/* leading consonant index */
  int vindex;			/* vowel index */
  int tindex;			/* trailing consonant index */
  gunichar choseong[4];
  gunichar jungseong[4];
  gunichar jongseong[4];

  /* options */
  gboolean always_use_jamo : 1;
  gboolean use_preedit : 1;
};

struct _GtkIMContextHangulClass
{
  GtkIMContextClass parent_class;
};

/* Candidate window */
struct _Candidate {
    GtkWidget *window;
    GdkWindow *parent;
    gchar *label;
    GtkWidget **children;
    const gunichar *data;
    int first;
    int n_per_window;
    int n_per_row;
    int n;
    int current;
};

struct _IMHangulCombination 
{
  guint32 key;
  gunichar code;
};

void          gtk_im_context_hangul_register_type (GTypeModule *type_module);

GtkIMContext *gtk_im_context_hangul_new      (void);
void          gtk_im_context_hangul_shutdown (void);

/* configuration */
void gtk_im_context_hangul_set_composer       (GtkIMContextHangul        *hcontext,
		                               IMHangulComposerType       type);
void gtk_im_context_hangul_set_keyboard_table (GtkIMContextHangul        *hcontext,
		                               const gunichar            *keyboard_table);
void gtk_im_context_hangul_set_compose_table  (GtkIMContextHangul        *hcontext,
		                               const IMHangulCombination *compose_table,
		                               int                        compose_table_size);
void gtk_im_context_hangul_set_use_jamo       (GtkIMContextHangul        *hcontext,
    				               gboolean		          use_jamo);

#endif /* __GTK_IM_CONTEXT_HANGUL_H__ */

/* vim: set sw=2 : */

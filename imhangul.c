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

/* 키보드 이름의 번역을 위해서 여기에 사용하지 않는 매크로로 키보드 이름을 
 * 리스팅 한다.
 * GTK+는 im_module_list() 함수로 지원하는 context 리스트를 얻은후 나중에 
 * 메뉴에 보여 줄때에는 그 스트링에 해당하는 번역 스트링을 각 모듈에서 지정한
 * mo 파일에서 로딩을 시도한다. 따라서 키보드 이름에 해당하는 번역 스트링을 
 * imhangul의 mo 파일이 가지고 있어야 한다. 그러므로 imhangul이 libhangul의 
 * 키보드 리스트를 동적으로 얻는다 하더라도, po 파일에 추가할 스트링은 
 * 여기에 하드코딩할 수밖에 없다. 
 * 아래에 리스팅한 키보드 이름은 번역되어 나타나겠지만, 없는 것들은 번역이 
 * 안된상태로 메뉴에 나타나게 될 것이다. */
#define CONTEXT_NAME_LIST 		    \
    N_("Hangul Dubeolsik")		    \
    N_("Hangul Dubeolsik yetgeul")	    \
    N_("Hangul Sebeolsik dubeol layout")    \
    N_("Hangul Sebeolsik 390")		    \
    N_("Hangul Sebeolsik final")	    \
    N_("Hangul Sebeolsik no-shift")	    \
    N_("Hangul Sebeolsik yetgeul")	    \
    N_("Hangul Romaja")			    \
    N_("Hangul Ahnmatae")		    \
}

static GtkIMContext *im_hangul_new (const char *libhangul_id);

/* 임의로 지원가능한 키보드 갯수를 16개로 잡는다. */
static gchar context_ids[16][16];
static gchar context_names[16][64];
static GtkIMContextInfo info_array[16];
static const GtkIMContextInfo *info_list[16];

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
    int i;
    int n;

    n = hangul_ic_get_n_keyboards();

    /* 일단 현재로서는 info_array 개수를 벗어난 것은 리스팅 하지 않는다. */
    if (n > G_N_ELEMENTS(info_array))
	n = G_N_ELEMENTS(info_array);

    for (i = 0; i < n; ++i) {
	const char* id;
	const char* name;
	id = hangul_ic_get_keyboard_id(i);
	name = hangul_ic_get_keyboard_name(i);
	g_snprintf(context_ids[i], sizeof(context_ids[i]), "hangul%s", id);
	g_snprintf(context_names[i], sizeof(context_names[i]),
		    "Hangul %s", name);
	info_array[i].context_id = context_ids[i];
	info_array[i].context_name = context_names[i];
	info_array[i].domain = GETTEXT_PACKAGE;
	info_array[i].domain_dirname = IM_HANGUL_LOCALEDIR;
	if (strcmp(id, DEFAULT_KEYBOARD_ID) == 0)
	    info_array[i].default_locales = "ko";
	else
	    info_array[i].default_locales = "";
	info_list[i] = &info_array[i];
    }

    *contexts = info_list;
    *n_contexts = n;
}

GtkIMContext *
im_module_create (const gchar *context_id)
{
    if (strncmp(context_id, "hangul", 6) == 0) {
	const char *id = context_id + 6;
	return im_hangul_new(id);
    }

    g_warning("imhangul:unknown context id: %s", context_id);
    g_assert_not_reached();

    return NULL;
}

static GtkIMContext *
im_hangul_new (const char *libhangul_id)
{
  GtkIMContext *context = gtk_im_context_hangul_new ();
  GtkIMContextHangul *hcontext = GTK_IM_CONTEXT_HANGUL (context);
  
  gtk_im_context_hangul_select_keyboard(hcontext, libhangul_id);

  return context;
}

/* vim: set sw=2 : */

/* imhangul_status_applet
 * Copyright (C) 2002 Choe Hwanjin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libgnomeui/gnome-about.h>
#include <libgnomeui/gnome-window-icon.h>
#include <panel-applet.h>
#include <panel-applet-gconf.h>

#include "none.xpm"
#include "english.xpm"
#include "hangul.xpm"

typedef struct _AppletData AppletData;

struct _AppletData {
	PanelApplet *applet;

	GdkPixbuf *none_pixbuf;
	GdkPixbuf *english_pixbuf;
	GdkPixbuf *hangul_pixbuf;
	int width;
	int height;

	GtkWidget *none_image;
	GtkWidget *english_image;
	GtkWidget *hangul_image;

	GdkWindow *root_window;
	GdkAtom mode_info_atom;
	GdkAtom mode_info_type;
	Atom mode_info_xatom;
};

enum {
	THEMES_LIST_PATH = 0,
	THEMES_LIST_NONE,
	THEMES_LIST_HANGUL,
	THEMES_LIST_ENGLISH,
	THEMES_LIST_NAME,
	N_COLS
};

static void themes_cb (BonoboUIComponent *uic, gpointer data, const gchar *verbname);
static void about_cb (BonoboUIComponent *uic, gpointer data, const gchar *verbname);

const BonoboUIVerb imhangul_status_applet_menu_verbs[] = {
	BONOBO_UI_UNSAFE_VERB ("Themes", themes_cb),
	BONOBO_UI_UNSAFE_VERB ("About", about_cb),
	BONOBO_UI_VERB_END
};

static void
applet_resize_image (PanelApplet *applet, AppletData *data)
{
	double factor;
	int new_width, new_height;
	int panel_size;
	PanelAppletOrient orient;
	GdkPixbuf *pixbuf;

	panel_size = panel_applet_get_size(applet);
	orient = panel_applet_get_orient(applet);

	if (orient == PANEL_APPLET_ORIENT_LEFT ||
	    orient == PANEL_APPLET_ORIENT_RIGHT) {
		factor = (double)panel_size / (double)data->width;
		new_width  = panel_size;
		new_height = (int)(data->height * factor);
	} else {
		factor = (double)panel_size / (double)data->height;
		new_width  = (int)(data->width * factor);
		new_height = panel_size;
	}

	pixbuf = gdk_pixbuf_scale_simple(data->none_pixbuf,
					 new_width, new_height,
					 GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data->none_image), pixbuf);
	g_object_unref(G_OBJECT(pixbuf));

	pixbuf = gdk_pixbuf_scale_simple(data->english_pixbuf,
					 new_width, new_height,
					 GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data->english_image), pixbuf);
	g_object_unref(G_OBJECT(pixbuf));

	pixbuf = gdk_pixbuf_scale_simple(data->hangul_pixbuf,
					 new_width, new_height,
					 GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data->hangul_image), pixbuf);
	g_object_unref(G_OBJECT(pixbuf));
}

static void
load_image(gchar *theme_dir, gchar *imagename, const char **default_xpm,
	   GdkPixbuf **pixbuf, GtkWidget **image)
{
	gchar *filename;
	GError *gerror = NULL;

	filename = g_build_filename(theme_dir, imagename, NULL);

	if (*pixbuf != NULL)
		g_object_unref(*pixbuf);

	*pixbuf = gdk_pixbuf_new_from_file(filename, &gerror);
	if (gerror != NULL) {
		g_assert(*pixbuf == NULL);
		g_print("Error on reading image file: %s\n", gerror->message);
		g_error_free(gerror);
		*pixbuf = gdk_pixbuf_new_from_xpm_data(default_xpm);
	}

	if (*image != NULL) 
		gtk_image_set_from_pixbuf(GTK_IMAGE(*image), *pixbuf);
	else
		*image = gtk_image_new_from_pixbuf(*pixbuf);

	g_free(filename);
}

static void
load_theme (AppletData *data, gchar *theme_dir)
{
	load_image(theme_dir, "none.png", none_xpm,
		   &data->none_pixbuf, &data->none_image);
	load_image(theme_dir, "english.png", english_xpm,
		   &data->english_pixbuf, &data->english_image);
	load_image(theme_dir, "hangul.png", hangul_xpm,
		   &data->hangul_pixbuf, &data->hangul_image);

	data->width = gdk_pixbuf_get_width(data->none_pixbuf);
	data->height = gdk_pixbuf_get_height(data->none_pixbuf);
	applet_resize_image (data->applet, data);
}

static void
applet_data_init (PanelApplet *applet, AppletData *data)
{
	gchar *theme;
	gchar *theme_dir;

	data->applet = applet;
	data->none_pixbuf = NULL;
	data->english_pixbuf = NULL;
	data->hangul_pixbuf = NULL;
	data->none_image = NULL;
	data->english_image = NULL;
	data->hangul_image = NULL;

	panel_applet_add_preferences(applet,
				     "/schemas/apps/imhangul_status/prefs",
				     NULL);

	/* init image */
	theme = panel_applet_gconf_get_string (PANEL_APPLET(applet),
					       "theme", NULL);
	theme_dir = g_build_filename(IMHANGUL_STATUS_THEMES_DIR, theme, NULL);
	load_theme(data, theme_dir);
	g_free(theme_dir);
	g_free(theme);

	data->width = gdk_pixbuf_get_width(data->none_pixbuf);
	data->height = gdk_pixbuf_get_height(data->none_pixbuf);
	applet_resize_image (applet, data);

	/* set atoms */
	data->mode_info_atom = gdk_atom_intern("_HANGUL_INPUT_MODE", TRUE);
	data->mode_info_type = gdk_atom_intern("INTEGER", TRUE);
	data->mode_info_xatom = gdk_x11_atom_to_xatom(data->mode_info_atom);
}

static GtkTreeModel *
get_themes_list(AppletData *data)
{
	gchar *path;
	gchar *theme_dir;
	gchar *file_none;
	gchar *file_hangul;
	gchar *file_english;
	GdkPixbuf *pixbuf_none;
	GdkPixbuf *pixbuf_hangul;
	GdkPixbuf *pixbuf_english;
	GtkListStore *store;
	GtkTreeIter iter;
	DIR *dir;
	struct dirent *dent;

	store = gtk_list_store_new(N_COLS,
				   G_TYPE_STRING,
				   GDK_TYPE_PIXBUF,
				   GDK_TYPE_PIXBUF,
				   GDK_TYPE_PIXBUF,
				   G_TYPE_STRING);

	path = IMHANGUL_STATUS_THEMES_DIR;

	dir = opendir(path);
	if (dir == NULL)
		return NULL;

	for (dent = readdir(dir);
	     dent != NULL;
	     dent = readdir(dir)) {
		if (dent->d_name[0] == '.')
			continue;

		theme_dir = g_build_filename(path, dent->d_name, NULL);
		file_none = g_build_filename(theme_dir, "none.png", NULL);
		file_hangul = g_build_filename(theme_dir, "hangul.png", NULL);
		file_english = g_build_filename(theme_dir, "english.png", NULL);
		pixbuf_none = gdk_pixbuf_new_from_file(file_none, NULL);
		pixbuf_hangul = gdk_pixbuf_new_from_file(file_hangul, NULL);
		pixbuf_english = gdk_pixbuf_new_from_file(file_english, NULL);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set (store, &iter,
				    THEMES_LIST_PATH, theme_dir,
				    THEMES_LIST_NONE, pixbuf_none,
				    THEMES_LIST_HANGUL, pixbuf_hangul,
				    THEMES_LIST_ENGLISH, pixbuf_english,
				    THEMES_LIST_NAME, dent->d_name,
				    -1);
		g_free(theme_dir);
		g_free(file_none);
		g_free(file_hangul);
		g_free(file_english);
		gdk_pixbuf_unref(pixbuf_none);
		gdk_pixbuf_unref(pixbuf_hangul);
		gdk_pixbuf_unref(pixbuf_english);
	}

	return GTK_TREE_MODEL(store);
}

/* callbacks */
static GtkTreePath *
search_theme_in_model (GtkTreeModel *model, gchar *target_theme)
{
	gchar *theme = "";
	GtkTreeIter iter;
	GtkTreePath *path;

	gtk_tree_model_get_iter_first(model, &iter);
	do {
		gtk_tree_model_get(model, &iter,
				   THEMES_LIST_NAME, &theme,
				   -1);
		if (strcmp(target_theme, theme) == 0) {
			path = gtk_tree_model_get_path(model, &iter);
			return path;
		}
	} while (gtk_tree_model_iter_next(model, &iter));

	return NULL;
}

static void
selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *path;
	gchar *theme;
	gboolean ret;
	AppletData *applet_data = (AppletData*)data;
	GError *gerror = NULL;

	ret = gtk_tree_selection_get_selected(selection, &model, &iter);
	if (!ret)
		return;
	gtk_tree_model_get(model, &iter,
			   THEMES_LIST_PATH, &path,
			   THEMES_LIST_NAME, &theme,
			   -1);

	load_theme(applet_data, path);

	/* saving theme setting */
	panel_applet_gconf_set_string(applet_data->applet,
				      "theme", theme,
				      &gerror);
	if (gerror)
		g_print("%s:error on saving theme setting: %s\n",
			PACKAGE, gerror->message);
}

static void
themes_cb (BonoboUIComponent *uic,
	   gpointer data,
	   const gchar *verbname)
{
	static GtkWidget *dialog = NULL;
	GtkWidget *vbox;
	GtkWidget *scrolledwindow;
	gchar *theme;

	GtkWidget *treeview;
	GtkTreeModel *model;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeSelection *selection;
	GtkTreePath *path;
	GtkRequisition treeview_size;

	if (dialog != NULL) {
		gtk_window_present(GTK_WINDOW(dialog));
		return;
	}

	dialog = gtk_dialog_new_with_buttons(_("imhangul status applet: Select Theme"),
					     NULL,
					     GTK_DIALOG_MODAL,
					     GTK_STOCK_CLOSE,
					     GTK_RESPONSE_CLOSE,
					     NULL);

	vbox = GTK_DIALOG(dialog)->vbox;
	gtk_widget_show(vbox);

	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scrolledwindow);
	gtk_container_set_border_width(GTK_CONTAINER(scrolledwindow), 8);
	gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);

	/* loading themes list */
	model = get_themes_list(data);
	treeview = gtk_tree_view_new_with_model(model);
	g_object_unref(G_OBJECT(model));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), FALSE);
	gtk_widget_show(treeview);
	gtk_container_add(GTK_CONTAINER(scrolledwindow), treeview);

	/* theme icons */
	/* state None */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("None"));
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "pixbuf", THEMES_LIST_NONE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* state Hangul */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Hangul"));
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "pixbuf", THEMES_LIST_HANGUL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* state English */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("English"));
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "pixbuf", THEMES_LIST_ENGLISH);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(_("Theme Name"),
							  renderer,
							  "text",
							  THEMES_LIST_NAME,
							  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(selection), "changed",
			 G_CALLBACK(selection_changed_cb), data);

	theme = panel_applet_gconf_get_string (((AppletData*)data)->applet,
					       "theme", NULL);
	path = search_theme_in_model(model, theme);
	g_free(theme);
	if (path) {
		gtk_tree_view_set_cursor (GTK_TREE_VIEW(treeview),
					  path, NULL, FALSE);
		gtk_tree_path_free(path);
	}

	gtk_widget_size_request(treeview, &treeview_size);
	gtk_widget_set_size_request(scrolledwindow,
		CLAMP(treeview_size.width + 50, 200, gdk_screen_width()), 250);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	dialog = NULL;
}

static void
about_cb (BonoboUIComponent *uic,
	  gpointer data,
	  const gchar *verbname)
{
	static GtkWidget *about = NULL;
	gchar *pixmap_file;
	GdkPixbuf *pixbuf;
	static const gchar *authors[] = {
		"Choe Hwanjin <krisna@kldp.org>",
		NULL
	};

	const gchar *documenters[] = {
		NULL
	};

	if (about != NULL) {
		gtk_window_present (GTK_WINDOW(about));
		return;
	}

	pixmap_file = gnome_program_locate_file(NULL, GNOME_FILE_DOMAIN_PIXMAP,
						"imhangul-status-applet-logo.png",
						TRUE, NULL);
	pixbuf = gdk_pixbuf_new_from_file (pixmap_file, NULL);
	g_free(pixmap_file);

	about = gnome_about_new (_("IMHangul Status Applet"), VERSION,
				 _("(C) 2002 Choe Hwanjin"),
				 _("Showing status of IMHangul input module"),
				 authors,
				 documenters,
				 NULL,
				 pixbuf);
	gtk_window_set_wmclass(GTK_WINDOW(about), "imhangul status",
			       "IMHangul Status");
	gnome_window_icon_set_from_file(GTK_WINDOW (about),
					GNOME_ICONDIR"/imhangul-status-applet.png");
	g_signal_connect (G_OBJECT (about), "destroy",
			  G_CALLBACK (gtk_widget_destroyed),
			  &about);
	gtk_widget_show (about);
}

static void
change_size_cb (PanelApplet *applet, gint arg, gpointer data)
{
	applet_resize_image (applet, (AppletData*)data);
}

static void
change_orient_cb (PanelApplet *applet, gint arg, gpointer data)
{
	applet_resize_image (applet, (AppletData*)data);
}

static void
update_state (AppletData *data, int state)
{
	switch (state) {
	case 0:
		gtk_widget_show(data->none_image);
		gtk_widget_hide(data->english_image);
		gtk_widget_hide(data->hangul_image);
		break;
	case 1:
		gtk_widget_show(data->english_image);
		gtk_widget_hide(data->none_image);
		gtk_widget_hide(data->hangul_image);
		break;
	case 2:
		gtk_widget_show(data->hangul_image);
		gtk_widget_hide(data->english_image);
		gtk_widget_hide(data->none_image);
		break;
	default:
		gtk_widget_show(data->none_image);
		gtk_widget_hide(data->english_image);
		gtk_widget_hide(data->hangul_image);
		break;
	}
}

static GdkFilterReturn
mode_info_cb (GdkXEvent *gxevent, GdkEvent *event, gpointer data)
{
	AppletData *applet_data;
	XEvent *xevent;
	XPropertyEvent *pevent;

	applet_data = (AppletData*)data;
	xevent = (XEvent*)gxevent;
	if (xevent->type != PropertyNotify)
		return GDK_FILTER_CONTINUE;

	pevent = (XPropertyEvent*)xevent;
	if (pevent->atom == applet_data->mode_info_xatom) {
		int state;
		guchar *buf;
		gboolean ret;

		ret = gdk_property_get (applet_data->root_window,
					applet_data->mode_info_atom,
					applet_data->mode_info_type,
					0, 32, 0,
					NULL, NULL, NULL,
					&buf);
		memcpy(&state, buf, sizeof(state));
		update_state(applet_data, state);
		g_free(buf);
	}

	return GDK_FILTER_CONTINUE;
}

static void
imhangul_status_create (PanelApplet *applet)
{
	GtkWidget *hbox;
	AppletData *data;

	data = g_new0 (AppletData, 1);
	applet_data_init(applet, data);

	hbox = gtk_hbox_new (TRUE, 0);
	gtk_widget_show(hbox);
	gtk_box_pack_start (GTK_BOX(hbox), data->none_image, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), data->english_image, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX(hbox), data->hangul_image, TRUE, TRUE, 0);
	gtk_widget_show(data->none_image);
	gtk_widget_hide(data->english_image);
	gtk_widget_hide(data->hangul_image);

	gtk_container_add(GTK_CONTAINER(applet), hbox);
	gtk_widget_show (GTK_WIDGET(applet));

	panel_applet_setup_menu_from_file (applet,
					   NULL,
					   "GNOME_IMHangulStatusApplet.xml",
					   NULL,
					   imhangul_status_applet_menu_verbs,
					   data);

	/* install event filter */
	data->root_window = GDK_ROOT_PARENT();
	gdk_window_set_events(data->root_window, GDK_PROPERTY_CHANGE_MASK);
	gdk_window_add_filter(data->root_window, mode_info_cb, data);

	/* install callback funcs. */
	g_signal_connect (G_OBJECT(applet), "change_orient",
			  G_CALLBACK(change_orient_cb), data);
	g_signal_connect (G_OBJECT(applet), "change_size",
			  G_CALLBACK(change_size_cb), data);
}

static gboolean
imhangul_status_factory (PanelApplet *applet, const gchar *iid, gpointer data)
{
	if (strcmp (iid, "OAFIID:GNOME_IMHangulStatusApplet") != 0)
		return FALSE;

	imhangul_status_create (applet);
	return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY ("OAFIID:GNOME_IMHangulStatusApplet_Factory",
			     PANEL_TYPE_APPLET,
			     "imhangul-status-applet",
			     "0",
			     imhangul_status_factory,
			     NULL)


/* example program for imhangul input test */
/* if you run this program, use ./test.sh instead of run this directly. */

#include <gtk/gtk.h>

void
on_status_window_clicked (GtkWidget *window) 
{
    static gboolean value = TRUE;
    GdkScreen *screen;
    GtkSettings *settings;

    screen = gdk_drawable_get_screen (window->window);
    settings = gtk_settings_get_for_screen (screen);

    g_object_set (settings,
	    	  "gtk-im-hangul-status-window", value,
		  NULL);
    g_print ("gtk-im-hangul-status-window: %d\n", value);
    value = !value;
}

void
on_use_capslock_clicked (GtkWidget *window) 
{
    static gboolean value = TRUE;
    GdkScreen *screen;
    GtkSettings *settings;

    screen = gdk_drawable_get_screen (window->window);
    settings = gtk_settings_get_for_screen (screen);

    g_object_set (settings,
	    	  "gtk-im-hangul-use-capslock", value,
		  NULL);
    g_print ("gtk-im-hangul-use-capslock: %d\n", value);
    value = !value;
}

void
on_use_dvorak_clicked (GtkWidget *window) 
{
    static gboolean value = TRUE;
    GdkScreen *screen;
    GtkSettings *settings;

    screen = gdk_drawable_get_screen (window->window);
    settings = gtk_settings_get_for_screen (screen);

    g_object_set (settings,
	    	  "gtk-im-hangul-use-dvorak", value,
		  NULL);
    g_print ("gtk-im-hangul-use-dvorak: %d\n", value);
    value = !value;
}

void
on_preedit_style_clicked (GtkWidget *window) 
{
    static gint value = 0;
    GdkScreen *screen;
    GtkSettings *settings;

    screen = gdk_drawable_get_screen (window->window);
    settings = gtk_settings_get_for_screen (screen);

    g_object_set (settings,
	    	  "gtk-im-hangul-preedit-style", value,
		  NULL);
    g_print ("gtk-im-hangul-preedit-style: %d\n", value);
    value++;
    if (value > 4)
	value = 0;
}

void
on_quit_clicked (GtkWidget *window)
{
    gtk_widget_destroy (window);
    gtk_main_quit();
}

void
on_destroy (GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *entry;
    GtkWidget *button;

    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "destroy",
                      G_CALLBACK(on_destroy), NULL);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);

    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    entry = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);

    button = gtk_button_new_with_label ("gtk-im-hangul-status-window");
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
		              G_CALLBACK(on_status_window_clicked), window);

    button = gtk_button_new_with_label ("gtk-im-hangul-user-capslock");
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
		              G_CALLBACK(on_use_capslock_clicked), window);

    button = gtk_button_new_with_label ("gtk-im-hangul-use-dvorak");
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
		              G_CALLBACK(on_use_dvorak_clicked), window);

    button = gtk_button_new_with_label ("gtk-im-hangul-preedit-style");
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
		              G_CALLBACK(on_preedit_style_clicked), window);

    button = gtk_button_new_with_label ("quit");
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    g_signal_connect_swapped (G_OBJECT (button), "clicked",
		              G_CALLBACK(on_quit_clicked), window);

    gtk_widget_show_all(window);

    gtk_main ();

    return 0;
}

/* vim: set sw=4 : */

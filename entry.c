/* example-start entry entry.c */

#include <gtk/gtk.h>

void enter_callback( GtkWidget *widget,
                     GtkWidget *entry )
{
  const gchar *entry_text;
  entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
  g_print("Entry contents: %s\n", entry_text);
  gtk_entry_set_text(GTK_ENTRY(entry), "");
}

void entry_toggle_visibility( GtkWidget *checkbutton,
                              GtkWidget *entry )
{
  gtk_entry_set_visibility(GTK_ENTRY(entry),
			 GTK_TOGGLE_BUTTON(checkbutton)->active);
}

void quit( int ret )
{
    gtk_main_quit();
}

int main( int   argc,
          char *argv[] )
{

    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    GtkWidget *entry;
    GtkWidget *button;
    GtkWidget *check;

    gtk_init (&argc, &argv);

    /* create a new window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "GTK Entry");
    g_signal_connect_swapped(G_OBJECT (window), "delete_event",
                             G_CALLBACK (quit), NULL);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    entry = gtk_entry_new ();
    g_signal_connect(G_OBJECT(entry), "activate",
		       G_CALLBACK(enter_callback),
		       entry);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);

    entry = gtk_entry_new ();
    g_signal_connect(G_OBJECT(entry), "activate",
		       G_CALLBACK(enter_callback),
		       entry);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);

    entry = gtk_entry_new ();
    g_signal_connect(G_OBJECT(entry), "activate",
		       G_CALLBACK(enter_callback),
		       entry);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);

    entry = gtk_entry_new ();
    g_signal_connect(G_OBJECT(entry), "activate",
		       G_CALLBACK(enter_callback),
		       entry);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);

    entry = gtk_entry_new ();
    g_signal_connect(G_OBJECT(entry), "activate",
		       G_CALLBACK(enter_callback),
		       entry);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (vbox), hbox);
    gtk_widget_show (hbox);
                                  
    check = gtk_check_button_new_with_label("Visible");
    gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
    g_signal_connect (G_OBJECT(check), "toggled",
			G_CALLBACK(entry_toggle_visibility), entry);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), TRUE);
    gtk_widget_show (check);
                                   
    button = gtk_button_new_with_label ("Close");
    g_signal_connect (G_OBJECT (button), "clicked",
		      G_CALLBACK(quit), NULL);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
    
    gtk_widget_show(window);

    gtk_main();
    return(0);
}

/* vim: set sw=4 ts=8 : */

/*
 * deletehost.c
 *
 * Copyright (C) 2012 Cian Mc Govern <cian@cianmcgovern.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#include "driver.h"

GtkWidget *window;
GtkWidget *combo;

void delete_window(GtkWidget *widget, gpointer user_data)
{
    gtk_widget_destroy(window);
}

void find_matching_hostname(gpointer data, gpointer user_data)
{
    char *target = (char*) user_data;
    struct hostdetails *currenthost = (struct hostdetails*) data;

    if(strcmp (target,currenthost->hostname) == 0)
        list = g_list_remove(list, data);

}
void delete_selected_host(GtkWidget *widget, gpointer user_data)
{
    char *hostname = (char*) gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo));
    if(hostname != NULL)
        g_list_foreach(list, find_matching_hostname, hostname);
    gtk_widget_destroy(window);
    free(hostname);
}

void add_to_dropdown(gpointer data, gpointer user_data)
{
    struct hostdetails *currenthost = (struct hostdetails*) data;
    gtk_combo_box_append_text(GTK_COMBO_BOX(combo), currenthost->hostname);
}

void delete_hosts(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *fixed;
    GtkBuilder *builder;
    GError *error = NULL;
    char *gladetemp;

    asprintf(&gladetemp, "%s/glade/deletehost.glade", SRC_DIR);

    builder = gtk_builder_new();
    gtk_builder_add_from_file( builder, gladetemp, &error);

    g_error_free(error);

    window = GTK_WIDGET( gtk_builder_get_object( builder, "dialog1" ) );
    fixed = GTK_WIDGET( gtk_builder_get_object( builder, "fixed1") );

    gtk_builder_connect_signals( builder, NULL );

    g_object_unref(builder);
    free(gladetemp);

    gtk_window_set_title(GTK_WINDOW(window), "Delete host");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    combo = gtk_combo_box_new_text();

    g_list_foreach(list, add_to_dropdown, NULL);

    gtk_fixed_put(GTK_FIXED(fixed), combo, 50, 50);
    gtk_container_add(GTK_CONTAINER(window), fixed);

    gtk_widget_show_all(window);
}

/*
 * displayhosts.c
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
#include <stdbool.h>
#include <stdlib.h>

#include "driver.h"

GtkWidget *window;
GtkTreeIter treeiter;
GtkListStore *liststore;

char *enabled(bool in)
{
    if(in)
        return "YES";
    else
        return "NO";
}

void add_item(gpointer data, gpointer user_data)
{
    struct hostdetails *currenthost = (struct hostdetails*) data;

    char *threshload;
    char *threshmem;
    char *threshcpu;
    
    asprintf(&threshload, "%s - %f", enabled(currenthost->load), currenthost->threshload);
    asprintf(&threshmem, "%s - %f", enabled(currenthost->memory), currenthost->threshmem);
    asprintf(&threshcpu, "%s - %f", enabled(currenthost->cpu), currenthost->threshcpu);

    gtk_list_store_append(liststore, &treeiter);
    gtk_list_store_set(liststore, &treeiter, 0, currenthost->hostname,
                                            1, currenthost->port, 
                                            2, threshload,
                                            3, threshmem,
                                            4, threshcpu,
                                            -1);
    free(threshload);
    free(threshmem);
    free(threshcpu);
}

void display_current_hosts(GtkWidget *widget, gpointer user_data)
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_window_set_title(GTK_WINDOW(window), "Current hosts");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    liststore = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);

    g_list_foreach(list, add_item, NULL);
    
    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
    gtk_container_add(GTK_CONTAINER(window), treeview);

    GtkCellRenderer *cellrenderertext = gtk_cell_renderer_text_new();

    GtkTreeViewColumn *treeviewcolumn = gtk_tree_view_column_new_with_attributes("Hostname",
                                                                                 cellrenderertext,
                                                                                 "text", 0,
                                                                                 NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

    treeviewcolumn = gtk_tree_view_column_new_with_attributes("Port", cellrenderertext, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

    treeviewcolumn = gtk_tree_view_column_new_with_attributes("Load Monitoring", cellrenderertext, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

    treeviewcolumn = gtk_tree_view_column_new_with_attributes("Memory Monitoring", cellrenderertext, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

    treeviewcolumn = gtk_tree_view_column_new_with_attributes("CPU Usage Monitoring", cellrenderertext, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

    gtk_widget_show_all(window); 
}

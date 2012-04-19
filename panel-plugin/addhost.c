/*
 * addhost.c
 *
 * Copyright (C) 2012 Cian Mc Govern <cian@cianmcgovern.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by * the Free Software Foundation; either version 2, or (at your option)
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
#include <libxfce4panel/xfce-panel-plugin.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#include "addhost.h"
#include "driver.h"
#include "displayhosts.h"

#define PLUGIN_WEBSITE "https://www.cianmcgovern.com"

struct host {
    GtkWidget *dialog;
    GtkEntry *hostname;
    GtkEntry* port;
    GtkCheckButton *load;
    GtkCheckButton *memory;
    GtkCheckButton *cpu;
    GtkEntry *username;
    GtkEntry *password;
    GtkEntry *threshload;
    GtkEntry *threshmem;
    GtkEntry *threshcpu;
    GtkFileChooser *chooser;
    GtkFileChooser *chooser2;
    GtkEntry *privatekeypassword;
};

struct host currenthost;

char *parse_host(char *path)
{
    const char *error;
    int erroffset;
    int ovector[186];
    char re[8192]="";

    char re1[]=".*?"; // Non-greedy match on filler
    strcat(re,re1);
    char re2[]="((?:\\/[\\w\\.\\-]+)+)";  // Unix Path 1
    strcat(re,re2);

    pcre *r =  pcre_compile(re, PCRE_CASELESS|PCRE_DOTALL, &error, &erroffset, NULL);
    int rc = pcre_exec(r, NULL, path, strlen(path), 0, 0, ovector, 186);
    if (rc>0)
    {
        char unixpath1[1024];
        pcre_copy_substring(path, ovector, rc,1,unixpath1, 1024);
        return unixpath1;
    }
    return NULL;
}

void on_help_clicked(GtkButton *button, gpointer user_data)
{
    g_spawn_command_line_async ("exo-open --launch WebBrowser " PLUGIN_WEBSITE, NULL);
}

void on_host_added(GtkWidget *widget, gpointer hostdetails)
{
    struct host currenthost;
    currenthost = *((struct host*) hostdetails);

    struct hostdetails *newhost;
    newhost = (struct hostdetails*) malloc(sizeof(struct hostdetails));

    newhost->publickeyfile = (char*)malloc(sizeof(char*));
    newhost->privatekeyfile = (char*)malloc(sizeof(char*));

    asprintf(&(newhost->hostname), "%s", gtk_entry_get_text(GTK_ENTRY(currenthost.hostname))); 
    asprintf(&(newhost->username), "%s", gtk_entry_get_text(GTK_ENTRY(currenthost.username)));
    asprintf(&(newhost->password), "%s", gtk_entry_get_text(GTK_ENTRY(currenthost.password)));
    
    newhost->port = atoi(gtk_entry_get_text(GTK_ENTRY(currenthost.port)));
    newhost->load = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(currenthost.load));
    newhost->memory = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(currenthost.memory));
    newhost->cpu = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(currenthost.cpu));
    newhost->threshload = atof(gtk_entry_get_text(GTK_ENTRY(currenthost.threshload)));
    newhost->threshmem = atof(gtk_entry_get_text(GTK_ENTRY(currenthost.threshmem)));
    newhost->threshcpu = atof(gtk_entry_get_text(GTK_ENTRY(currenthost.threshcpu)));

    asprintf(&(newhost->publickeyfile), "%s", parse_host(gtk_file_chooser_get_uri(currenthost.chooser2)));
    asprintf(&(newhost->privatekeyfile), "%s", parse_host(gtk_file_chooser_get_uri(currenthost.chooser)));

    list = g_list_append(list, newhost);
    gtk_widget_destroy(currenthost.dialog);
}

void on_host_closed(GtkWidget *widget, gpointer hostdetails)
{
    struct host currenthost;
    currenthost = *((struct host*) hostdetails);

    gtk_widget_destroy(currenthost.dialog);
}

void on_hosts_config(GtkButton *button, gpointer user_data)
{
    GtkBuilder *builder;
    GtkEntry *add, *cancel;
    GtkWidget *label;
    char *gladetemp;

    asprintf(&gladetemp, "%s/glade/addhost.glade", SRC_DIR);

    GError *error = NULL;

    builder = gtk_builder_new();

    gtk_builder_add_from_file( builder, gladetemp, &error ); 

    g_error_free(error);

    currenthost.dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dialog1" ) );

    currenthost.hostname = GTK_ENTRY( gtk_builder_get_object( builder, "hostname" ) );
    currenthost.port = GTK_ENTRY( gtk_builder_get_object( builder, "port" ) );
    currenthost.load = GTK_CHECK_BUTTON( gtk_builder_get_object( builder, "load" ) );
    currenthost.memory = GTK_CHECK_BUTTON( gtk_builder_get_object( builder, "memory" ) );
    currenthost.cpu = GTK_CHECK_BUTTON( gtk_builder_get_object( builder, "cpu" ) );
    currenthost.username = GTK_ENTRY( gtk_builder_get_object( builder, "username" ) );
    currenthost.password = GTK_ENTRY( gtk_builder_get_object( builder, "password" ) );
    currenthost.threshload = GTK_ENTRY( gtk_builder_get_object( builder, "threshload" ) );
    currenthost.threshmem = GTK_ENTRY( gtk_builder_get_object( builder, "threshmem" ) );
    currenthost.threshcpu = GTK_ENTRY( gtk_builder_get_object( builder, "threshcpu" ) );
    currenthost.chooser = GTK_FILE_CHOOSER( gtk_builder_get_object( builder, "privatekeyfile"));
    currenthost.chooser2 = GTK_FILE_CHOOSER( gtk_builder_get_object( builder, "publickeyfile"));

    add = GTK_ENTRY( gtk_builder_get_object ( builder, "add" ) );
    cancel = GTK_ENTRY( gtk_builder_get_object ( builder, "cancel" ) );

    gtk_builder_connect_signals( builder, NULL );

    g_object_unref( G_OBJECT( builder ) );
    free(gladetemp);

    g_signal_connect( G_OBJECT(add), "clicked", G_CALLBACK(on_host_added), &currenthost);
    g_signal_connect( G_OBJECT(cancel), "clicked", G_CALLBACK(on_host_closed), &currenthost);

    gtk_window_set_position (GTK_WINDOW (currenthost.dialog), GTK_WIN_POS_CENTER);

    gtk_widget_show (currenthost.dialog); 
}

/*
 * dialog.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <syslog.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include "xfce4-remotenotify-plugin.h"
#include "configure.h"
#include "addhost.h"
#include "displayhosts.h"
#include "deletehost.h"

GtkWidget *dialog;
GtkToggleButton *cbnotify, *cbsound;
RemoteNotifyPlugin *remote;
XfcePanelPlugin *plug;

void on_dialog1_close(GtkWidget *widget, gpointer user_data)
{
    xfce_panel_plugin_unblock_menu(plug);
}

void save_config(GtkWidget *widget, gpointer user_data)
{
    remote->displaynotifications = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cbnotify));
    remote->playsounds = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cbsound));
    remotenotify_save(plug, remote);
    xfce_panel_plugin_unblock_menu(plug);
    gtk_widget_destroy(dialog);
}

void remotenotify_configure(XfcePanelPlugin *plugin, RemoteNotifyPlugin *remotenotify)
{
    GtkBuilder *builder;
    GtkButton *button;
    GError *error = NULL;
    char *gladetemp;

    asprintf(&gladetemp, "%s/glade/config.glade", SRC_DIR);

    plug = plugin;
    remote = remotenotify;

    xfce_panel_plugin_block_menu(plugin);

    builder = gtk_builder_new();

    gtk_builder_add_from_file( builder, gladetemp, &error ); 

    g_error_free(error);

    dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dialog1" ) );
    button = GTK_BUTTON( gtk_builder_get_object( builder, "save" ) );
    cbnotify = GTK_TOGGLE_BUTTON( gtk_builder_get_object( builder, "cbnotify" ) );
    cbsound = GTK_TOGGLE_BUTTON( gtk_builder_get_object( builder, "cbsound" ) );

    gtk_builder_connect_signals( builder, NULL );

    g_object_unref( G_OBJECT( builder ) );
    free(gladetemp);

    g_signal_connect( G_OBJECT(button), "clicked", G_CALLBACK(save_config), NULL);

    gtk_toggle_button_set_active(cbnotify, remotenotify->displaynotifications);
    gtk_toggle_button_set_active(cbsound, remotenotify->playsounds);

    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

    gtk_widget_show (dialog);
}

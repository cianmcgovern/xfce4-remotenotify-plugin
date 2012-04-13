/*
 * xfce4-remotenotify-plugin.c
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
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4panel/xfce-hvbox.h>
#include <stdbool.h>

#include "xfce4-remotenotify-plugin.h"
#include "driver.h"
#include "configure.h"

XfceRc *rc;
int hostcount = 0;

static void remotenotify_construct(XfcePanelPlugin *plugin);

XFCE_PANEL_PLUGIN_REGISTER_EXTERNAL(remotenotify_construct);

void free_struct(gpointer data)
{
    struct hostdetails *currenthost;
    currenthost = (struct hostdetails*) data;

    free(currenthost->hostname);
    free(currenthost->username);
    free(currenthost->password);
    free(currenthost);
}

void free_data()
{
    g_list_free_full(list, free_struct);
}

void save_group(gpointer data, gpointer user_data)
{
    struct hostdetails *currenthost;
    currenthost = (struct hostdetails*) data;

    char *group, *threshload, *threshmem, *threshcpu;

    asprintf(&threshload, "%f", currenthost->threshload);
    asprintf(&threshmem, "%f", currenthost->threshmem);
    asprintf(&threshcpu, "%f", currenthost->threshcpu);
    asprintf(&group, "Host%i", hostcount);

    xfce_rc_set_group(rc,group);
    xfce_rc_write_entry(rc, "hostname", currenthost->hostname);
    xfce_rc_write_int_entry(rc, "port", currenthost->port);
    xfce_rc_write_bool_entry(rc, "load", currenthost->load);
    xfce_rc_write_bool_entry(rc, "memory", currenthost->memory);
    xfce_rc_write_bool_entry(rc, "cpu", currenthost->cpu);
    xfce_rc_write_entry(rc, "username", currenthost->username);
    xfce_rc_write_entry(rc, "password", currenthost->password);
    xfce_rc_write_entry(rc, "threshload", threshload);
    xfce_rc_write_entry(rc, "threshmem", threshmem);
    xfce_rc_write_entry(rc, "threshcpu", threshcpu);
    xfce_rc_write_int_entry(rc, "interval", currenthost->interval);

    free(threshload);
    free(threshmem);
    free(threshcpu);
    free(group);

    hostcount++;
}

void remotenotify_save (XfcePanelPlugin *plugin, RemoteNotifyPlugin *remotenotify)
{
    gchar  *file;
    
    hostcount = 0;

    /* get the config file location */
    file = xfce_panel_plugin_save_location (plugin, TRUE);

    if (G_UNLIKELY (file == NULL))
    {   
        DBG ("Failed to open config file");
        return;
    }   

    /* open the config file, read/write */
    rc = xfce_rc_simple_open (file, FALSE);
    g_free (file);

    if (G_LIKELY (rc != NULL))
    {   
        /* save the settings */
        DBG(".");
        
        g_list_foreach(list, save_group, NULL);

        xfce_rc_set_group(rc, "General");
        xfce_rc_write_int_entry(rc, "hosts", hostcount);
        xfce_rc_write_bool_entry(rc, "displaynotifcations", remotenotify->displaynotifications);
        xfce_rc_write_bool_entry(rc, "playsounds", remotenotify->playsounds);

        /* close the rc file */
        xfce_rc_close (rc);
    }   
}

static void remotenotify_read (RemoteNotifyPlugin *remotenotify)
{
    XfceRc      *rc;
    gchar       *file;
    const gchar *value;

    /* get the plugin config file location */
    file = xfce_panel_plugin_save_location (remotenotify->plugin, TRUE);

    if (G_LIKELY (file != NULL))
    {
        /* open the config file, readonly */
        rc = xfce_rc_simple_open (file, TRUE);

        /* cleanup */
        g_free (file);

        if (G_LIKELY (rc != NULL))
        {
            /* read the settings */
            xfce_rc_set_group(rc, "General");
            hostcount = xfce_rc_read_int_entry(rc, "hosts", 0);

            int i;
            for(i = 1; i <= hostcount; i++) {

                char *group;
                asprintf(&group, "Host%i", i);

                struct hostdetails *loadhost;
                loadhost = (struct hostdetails *) malloc(sizeof(struct hostdetails));

                xfce_rc_set_group(rc, group);
                asprintf(&(loadhost->hostname), "%s", xfce_rc_read_entry(rc, "hostname", NULL));
                loadhost->port = xfce_rc_read_int_entry(rc, "port", 0);
                loadhost->load = xfce_rc_read_bool_entry(rc, "load", false);
                loadhost->memory = xfce_rc_read_bool_entry(rc, "memory", false);
                loadhost->cpu = xfce_rc_read_bool_entry(rc, "cpu", false);
                asprintf(&(loadhost->username), "%s", xfce_rc_read_entry(rc, "username", NULL));
                asprintf(&(loadhost->password), "%s", xfce_rc_read_entry(rc, "password", NULL));
                loadhost->threshload = atof(xfce_rc_read_entry(rc, "threshload", 0));
                loadhost->threshmem = atof(xfce_rc_read_entry(rc, "threshmem", 0));
                loadhost->threshcpu = atof(xfce_rc_read_entry(rc, "threshcpu", 0));
                loadhost->interval = xfce_rc_read_int_entry(rc, "interval", 0);

                list = g_list_prepend(list, loadhost);

                free(group);
            }

            /* cleanup */
            xfce_rc_close (rc);

            /* leave the function, everything went well */
            return;
        }
    }
    else {
        g_free(file);
    }
}

static RemoteNotifyPlugin *remotenotify_new (XfcePanelPlugin *plugin)
{
    RemoteNotifyPlugin   *remotenotify;
    GtkOrientation  orientation;
    GtkWidget      *label;

    /* allocate memory for the plugin structure */
    remotenotify = panel_slice_new0 (RemoteNotifyPlugin);

    /* pointer to plugin */
    remotenotify->plugin = plugin;

    /* read the user settings */
    remotenotify_read (remotenotify);

    /* get the current orientation */
    orientation = xfce_panel_plugin_get_orientation (plugin);

    /* create some panel widgets */
    remotenotify->ebox = gtk_event_box_new ();
    gtk_widget_show (remotenotify->ebox);

    remotenotify->hvbox = xfce_hvbox_new (orientation, FALSE, 2);
    gtk_widget_show (remotenotify->hvbox);
    gtk_container_add (GTK_CONTAINER (remotenotify->ebox), remotenotify->hvbox);

    /* some sample widgets */
    label = gtk_label_new (_("Remote Notify"));
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (remotenotify->hvbox), label, FALSE, FALSE, 0);

    label = gtk_label_new (_("Plugin"));
    gtk_widget_show (label);
    gtk_box_pack_start (GTK_BOX (remotenotify->hvbox), label, FALSE, FALSE, 0);

    return remotenotify;
}

static void remotenotify_free (XfcePanelPlugin *plugin, RemoteNotifyPlugin *remotenotify)
{
    GtkWidget *dialog;

    /* check if the dialog is still open. if so, destroy it */
    dialog = g_object_get_data (G_OBJECT (plugin), "dialog");
    if (G_UNLIKELY (dialog != NULL))
        gtk_widget_destroy (dialog);

    /* destroy the panel widgets */
    gtk_widget_destroy (remotenotify->hvbox);

    /* cleanup the settings */
    free_data();

    /* free the plugin structure */
    panel_slice_free (RemoteNotifyPlugin, remotenotify);
}

static void remotenotify_orientation_changed (XfcePanelPlugin *plugin, GtkOrientation orientation, RemoteNotifyPlugin *remotenotify)
{
    /* change the orienation of the box */
    xfce_hvbox_set_orientation (XFCE_HVBOX (remotenotify->hvbox), orientation);
}

static gboolean remotenotify_size_changed (XfcePanelPlugin *plugin, gint size, RemoteNotifyPlugin *remotenotify)
{
    GtkOrientation orientation;

    /* get the orientation of the plugin */
    orientation = xfce_panel_plugin_get_orientation (plugin);

    /* set the widget size */
    if (orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
    else
        gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

    /* we handled the orientation */
    return TRUE;
}

static void remotenotify_construct (XfcePanelPlugin *plugin)
{
    RemoteNotifyPlugin *remotenotify;

    /* Intialise number of hosts */
    numberhosts = 0;

    /* setup transation domain */
    xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

    /* create the plugin */
    remotenotify = remotenotify_new (plugin);

    /* add the ebox to the panel */
    gtk_container_add (GTK_CONTAINER (plugin), remotenotify->ebox);

    /* show the panel's right-click menu on this ebox */
    xfce_panel_plugin_add_action_widget (plugin, remotenotify->ebox);

    /* connect plugin signals */
    g_signal_connect (G_OBJECT (plugin), "free-data", G_CALLBACK (remotenotify_free), remotenotify);

    g_signal_connect (G_OBJECT (plugin), "save", G_CALLBACK (remotenotify_save), remotenotify);

    g_signal_connect (G_OBJECT (plugin), "size-changed", G_CALLBACK (remotenotify_size_changed), remotenotify);

    g_signal_connect (G_OBJECT (plugin), "orientation-changed", G_CALLBACK (remotenotify_orientation_changed), remotenotify);

    /* show the configure menu item and connect signal */
    xfce_panel_plugin_menu_show_configure (plugin);
    g_signal_connect (G_OBJECT (plugin), "configure-plugin", G_CALLBACK (remotenotify_configure), remotenotify);

    /* show the about menu item and connect signal */
    //xfce_panel_plugin_menu_show_about (plugin);
    //g_signal_connect (G_OBJECT (plugin), "about", G_CALLBACK (remotenotify_about), NULL);
}

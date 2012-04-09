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

#include "xfce4-remotenotify-plugin.h"

static void remotenotify_construct(XfcePanelPlugin *plugin);

XFCE_PANEL_PLUGIN_REGISTER_EXTERNAL(remotenotify_construct);

void remotenotify_save (XfcePanelPlugin *plugin, RemoteNotifyPlugin *remotenotify)
{
    XfceRc *rc;
    gchar  *file;

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
        if (remotenotify->setting1)
            xfce_rc_write_entry(rc, "setting1", remotenotify->setting1);
        
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
            value = xfce_rc_read_entry (rc, "setting1", 0);
            remotenotify->setting1 = g_strdup (value);

            /* cleanup */
            xfce_rc_close (rc);

            /* leave the function, everything went well */
            return;
        }
    }

    /* something went wrong, apply default values */
    //DBG ("Applying default settings");

    //sample->setting1 = g_strdup (DEFAULT_SETTING1);
    //sample->setting2 = DEFAULT_SETTING2;
    //sample->setting3 = DEFAULT_SETTING3;
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
    label = gtk_label_new (_("RemoteNotify"));
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
    if (G_LIKELY (remotenotify->setting1 != NULL))
        g_free (remotenotify->setting1);

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
    //xfce_panel_plugin_menu_show_configure (plugin);
    //g_signal_connect (G_OBJECT (plugin), "configure-plugin", G_CALLBACK (remotenotify_configure), remotenotify);

    /* show the about menu item and connect signal */
    //xfce_panel_plugin_menu_show_about (plugin);
    //g_signal_connect (G_OBJECT (plugin), "about", G_CALLBACK (remotenotify_about), NULL);
}

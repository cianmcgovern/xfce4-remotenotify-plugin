/*
 * xfce4-remotenotify-plugin.h
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

#ifndef REMOTE_NOTIFY_H_
#define REMOTE_NOTIFY_H_

#include "driver.h"

G_BEGIN_DECLS

typedef struct
{
    XfcePanelPlugin *plugin;

    GtkWidget *ebox;
    GtkWidget *hvbox;
    GtkWidget *label;

    bool displaynotifications;
    bool playsounds;
} RemoteNotifyPlugin;

int numberhosts;
extern bool displaynotifications;

void remotenotify_save (XfcePanelPlugin *plugin, RemoteNotifyPlugin *remotenotify);

G_END_DECLS

#endif

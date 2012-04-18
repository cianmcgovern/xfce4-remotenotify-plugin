/*
 * notify.c
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

#include <libnotify/notify.h>
#include <stdlib.h>

void display_load_notification(char *hostname, float load)
{

    char *title = (char*) malloc(sizeof(char*));
    asprintf(&title, "Host: %s", hostname);

    char *message = (char*) malloc(sizeof(char*));
    asprintf(&message, "The load average for this host has crossed above the threshold: %f", load);

    notify_init("Remote Notify");

    NotifyNotification *notify = notify_notification_new(title, message, "dialog-information");

    notify_notification_show(notify, NULL);

    notify_uninit();

    free(title);
    free(message);
}

void display_memory_notification(char *hostname, float usage)
{
    char *title = (char*) malloc(sizeof(char*));
    asprintf(&title, "Host: %s", hostname);

    char *message = (char*) malloc(sizeof(char*));
    asprintf(&message, "The memory usage for this host has crossed above the threshold: %f", usage);

    notify_init("Remote Notify");

    NotifyNotification *notify = notify_notification_new(title, message, "dialog-information");

    notify_notification_show(notify, NULL);

    notify_uninit();

    free(title);
    free(message);
}

void display_cpu_notification(char *hostname, float usage)
{
    char *title = (char*) malloc(sizeof(char*));
    asprintf(&title, "Host: %s", hostname);

    char *message = (char*) malloc(sizeof(char*));
    asprintf(&message, "The CPU usage for this host has crossed above the threshold: %f", usage);

    notify_init("Remote Notify");

    NotifyNotification *notify = notify_notification_new(title, message, "dialog-information");

    notify_notification_show(notify, NULL);

    notify_uninit();

    free(title);
    free(message);
}

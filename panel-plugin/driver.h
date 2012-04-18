/*
 * driver.h
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

#ifndef REMOTE_NOTIFY_DRIVER_H_
#define REMOTE_NOTIFY_DRIVER_H_

#include <stdbool.h>
#include <glib.h>
#include <gtk/gtk.h>

extern GList *list;

extern int pause_exec;

extern  pthread_mutex_t sshinit_lock;

struct hostdetails
{
    char *hostname;
    int port;
    bool load;
    bool memory;
    bool cpu;
    char *username;
    char *password;
    float threshload;
    float threshmem;
    float threshcpu;
    int interval;
};

void update_pause();
void *execute_threads(void *ptr);

#endif

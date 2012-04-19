/*
 * driver.c
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

#include <glib.h>
#include <stdbool.h>
#include <syslog.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include "driver.h"
#include "remote.h"
#include "ssh.h"
#include "parse.h"
#include "notification.h"
#include "xfce4-remotenotify-plugin.h"

GList *list = NULL;
int pause_exec = 1;
int interval = 0;

pthread_mutex_t sshinit_lock = PTHREAD_MUTEX_INITIALIZER;

void callssh(struct remote *ptr);

void update_pause()
{
    if(pause_exec == 0) {
        pause_exec = 1;
    }
    else
        pause_exec = 0;
}

void update_interval(int time)
{
    interval = time;
}

void prepare_threads(gpointer data, gpointer user_data)
{

    struct hostdetails *currenthost = (struct hostdetails*) data;
    struct remote a;

    a.num_commands = 3;

    a.commands = malloc(3 * sizeof(char*));
    a.results = malloc(3 * sizeof(char*));

    a.username = (char*)malloc(sizeof(char*));
    a.password = (char*)malloc(sizeof(char*));
    a.hostname = (char*)malloc(sizeof(char*));

    int i;
    for(i = 0; i < 3; i++) {
        a.commands[i] = malloc(2048 * sizeof(char));
        a.results[i] = malloc(2048 * sizeof(char));
        a.results[i] = "";
    }

    if(currenthost->load)
        strcpy(a.commands[0], "uptime");
    else
        strcpy(a.commands[0], "");
    if(currenthost->memory)
        strcpy(a.commands[1], "free -m");
    else
        strcpy(a.commands[1], "");
    if(currenthost->cpu)
        strcpy(a.commands[2], "mpstat | awk '{ print $NF; }'");
    else
        strcpy(a.commands[2], "");

    strcpy(a.hostname, currenthost->hostname);
    a.port = currenthost->port;
    strcpy(a.username, currenthost->username);

    if(strlen(currenthost->privatekeyfile) > 0) {
        asprintf(&(a.privatekey), "%s", currenthost->privatekeyfile);
        asprintf(&(a.publickey), "%s", currenthost->publickeyfile);
        strcpy(a.password, "");
    }
    else
        strcpy(a.password, currenthost->password);

    callssh(&a);

    if(strlen(a.results[0]) > 0) {
        float load = strtof(a.results[0], NULL);
        if(load >= currenthost->threshload && displaynotifications)
            display_load_notification(a.hostname, load);
    }

    if(strlen(a.results[1]) > 0 ) {

        char *full = a.results[1];
        char *seperator = strstr(a.results[1], ":");
        seperator++;

        int colon = seperator - full;
        full[colon-1] = '\0';
        float used = atof(full);
        
        int len = strlen(seperator) + 1;
        seperator[len] = '\0';
        float max = atof(seperator);
        
        float usedpercent = (used/max) * 100;
        if(usedpercent > currenthost->threshmem && displaynotifications)
            display_memory_notification(a.hostname, usedpercent);

        free(a.results[1]);
    }

    if(strlen(a.results[2]) > 0 ) {

        float used = 100 - atof(a.results[2]);
        if(used > currenthost->threshcpu && displaynotifications)
            display_cpu_notification(a.hostname, used);
    }
    free(a.username);
    free(a.password);
    for(i = 0; i < 3; i++) {
        free(a.commands[i]);
    }
    free(a.results);
    free(a.commands);
}

void callssh(struct remote *ptr)
{
    struct remote *rm;

    rm = ptr;

    execute_command(rm);

    if( strlen(rm->results[0]) > 0 ) {
        parse_uptime(rm->results[0]);
        syslog(LOG_INFO, "Got load for %s: %s\n",rm->hostname,rm->results[0]);
    }

    if( strlen(rm->results[1]) > 0 ) {
        parse_memory(rm->results[1]);
        syslog(LOG_INFO, "Got memory for %s: %s\n", rm->hostname,rm->results[1]);
    }
    if( strlen(rm->results[2]) > 0 ) {
        parse_mpstat(rm->results[2]);
        syslog(LOG_INFO, "Got CPU load for %s: %s\n", rm->hostname,rm->results[2]);
    }
}

void *execute_threads(void *ptr)
{
    for(;;) {
        if(pause_exec!=0 && interval!=0) {
            g_list_foreach(list, prepare_threads, NULL);
            sleep(interval);
        }
        else
            sleep(1);
    }
}

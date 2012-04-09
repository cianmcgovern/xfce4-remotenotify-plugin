/*
 * parse.h
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

#ifndef _PARSE
#define _PARSE

/* 
 *
 * parse_uptime()
 *
 * Parses the output from the uptime command for the first load average and saves the result in the input 
 * 
 * Returns 0 on success or 1 on failure
 *
 */
int parse_uptime(char*);

/* 
 * parse_memory()
 *
 * Parses the free -m command for the used and free memory in the cache and saves the result in input as used:free
 *
 * Reutns 0 on success or 1 on failure
 *
 */
int parse_memory(char*);

/* 
 * parse_mpstat()
 *
 * Parses the output from the mpstat command and extracts the average idle percentage for all CPUs and stores the result in the input
 *
 * Returns 0 on success or 1 for failure 
 *
 */
int parse_mpstat(char*);

#endif

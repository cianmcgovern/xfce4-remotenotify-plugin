/*
 * parse.c
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

#include <string.h>
#include <pcre.h>
#include <stdio.h>

/*
 * This file uses code generated from http://www.txt2re.com
 */

/* 
 *
 * parse_uptime()
 *
 * Parses the output from the uptime command for the first load average and saves the result in the input 
 * 
 * Returns 0 on success or 1 on failure
 *
 */
int parse_uptime(char *input)
{
    
    const char *error;
    int erroffset;
    int ovector[186];
    char re[8192]="";

    char re1[]=".*?"; // Non-greedy match on filler
    strcat(re,re1);
    char re2[]="([+-]?\\d*\\.\\d+)(?![-+0-9\\.])";    // Float 1
    strcat(re,re2);

    pcre *r =  pcre_compile(re, PCRE_CASELESS|PCRE_DOTALL, &error, &erroffset, NULL);
    int rc = pcre_exec(r, NULL, input, strlen(input), 0, 0, ovector, 186);
    if (rc>0)
    {
        char float1[5];
        pcre_copy_substring(input, ovector, rc,1,float1, 5);

        if( strlen(float1) > 0 ) {
            snprintf(input,strlen(input),"%s",float1);
            pcre_free(r);
            return 0;
        }
        
        pcre_free(r);
        return 1;
    }
   
    pcre_free(r);
    return 1;
}

/* 
 * parse_memory()
 *
 * Parses the free -m command for the used and free memory in the cache and saves the result in input as used:free
 *
 * Reutns 0 on success or 1 on failure
 *
 */
int parse_memory(char *input)
{
    const char *error;
    int erroffset;
    int ovector[186];
    char re[8192]="";

    char re1[]=".*?";   // Non-greedy match on filler
    strcat(re,re1);
    char re2[]="\\d+";    // Uninteresting: int
    strcat(re,re2);
    char re3[]=".*?"; // Non-greedy match on filler
    strcat(re,re3);
    char re4[]="\\d+";    // Uninteresting: int
    strcat(re,re4);
    char re5[]=".*?"; // Non-greedy match on filler
    strcat(re,re5);
    char re6[]="\\d+";    // Uninteresting: int
    strcat(re,re6);
    char re7[]=".*?"; // Non-greedy match on filler
    strcat(re,re7);
    char re8[]="\\d+";    // Uninteresting: int
    strcat(re,re8);
    char re9[]=".*?"; // Non-greedy match on filler
    strcat(re,re9);
    char re10[]="\\d+";   // Uninteresting: int
    strcat(re,re10);
    char re11[]=".*?";    // Non-greedy match on filler
    strcat(re,re11);
    char re12[]="\\d+";   // Uninteresting: int
    strcat(re,re12);
    char re13[]=".*?";    // Non-greedy match on filler
    strcat(re,re13);
    char re14[]="(\\d+)"; // Used memory
    strcat(re,re14);
    char re15[]=".*?";    // Non-greedy match on filler
    strcat(re,re15);
    char re16[]="(\\d+)"; // Free memory
    strcat(re,re16); 

    pcre *r =  pcre_compile(re, PCRE_CASELESS|PCRE_DOTALL, &error, &erroffset, NULL);
    int rc = pcre_exec(r, NULL, input, strlen(input), 0, 0, ovector, 186);
    if (rc>0)
    {
        char int1[10];
        pcre_copy_substring(input, ovector, rc,1,int1, 10);
        char int2[10];
        pcre_copy_substring(input, ovector, rc,2,int2, 10);

        if( (strlen(int1)) > 0 && (strlen(int2)) > 0 ) {
            snprintf(input,strlen(input),"%s:%s",int1,int2);
            pcre_free(r);
            return 0;
        }
       
        pcre_free(r);
        return 1;
    }
    
    return 1;

}

/* 
 * parse_mpstat()
 *
 * Parses the output from the mpstat command and extracts the average idle percentage for all CPUs and stores the result in the input
 *
 * Returns 0 on success or 1 for failure 
 *
 */
int parse_mpstat(char *input)
{
    const char *error;
    int erroffset;
    int ovector[186];
    char re[8192]="";

    char re1[]=".*?"; // Non-greedy match on filler
    strcat(re,re1);
    char re2[]="([+-]?\\d*\\.\\d+)(?![-+0-9\\.])";    // Float 1
    strcat(re,re2);

    pcre *r =  pcre_compile(re, PCRE_CASELESS|PCRE_DOTALL, &error, &erroffset, NULL);
    int rc = pcre_exec(r, NULL, input, strlen(input), 0, 0, ovector, 186);
    if (rc>0)
    {
        char float1[7];
        pcre_copy_substring(input, ovector, rc,1,float1, 7);
        
        if( (strlen(float1) > 0) ) {
            snprintf(input,strlen(input),"%s",float1);
            pcre_free(r);
            return 0;
        }
        
        pcre_free(r);
        return 1;
    } 
       
    pcre_free(r);
    return 1;
}

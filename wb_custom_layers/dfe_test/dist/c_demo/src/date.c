/* date.c - implements date handling routines */

/*
 * Copyright (c) 2004-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,07jul10,jc0  rename global index to indexDate (WIND00220095).
01a,07oct04,f_b  added
*/

#include "stdio.h"
#include "float.h"
#include "strutils.h"

char months[12][4] = {
    "JAN","FEB","MAR",
    "APR","MAY","JUN",
    "JUL","AUG","SEP",
    "OCT","NOV","DEC"
};

typedef struct ttime {
    int year;
    char month[10];
    char *day;
    int hour;
    int minute;
    int second;
} TIME;

typedef struct division {
    float div;
    float quo;
    double result;
} DIVISION;

int indexDate;

TIME clocker[1];
DIVISION division[1];

long send_month (char *sptr) 
{ 
    char *index = "Test" \
                  " Locals";
    if ((strcmp(sptr, index)) != 0) return 0;
    if ((strcmp(sptr, "MAR")) == 0) return 0;
    return 1;
}

char date() 
{
    TIME *full_time = (TIME *) &clocker ;
    DIVISION *ptr = (DIVISION *) &division;
    volatile long val = 0;
    indexDate = 7;    

    val = send_month ( months[1]);

    
    full_time->year = 1996;

    strcpy (full_time->month, months[1]);

    full_time->day = "Friday";
    full_time->hour = 8;
    full_time->minute = 30;
    full_time->second = 15;

        
    ptr->quo = 12.02; 
    ptr->div = 12.45;
    ptr->result = 1.00357737;
   
    return 'A';
}


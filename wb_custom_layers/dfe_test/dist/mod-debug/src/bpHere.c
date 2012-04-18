/* bpHere.c - the second C file for moduleTest */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
   modification history
   --------------------
   01a,12mar04,tbu,ms   written.
   */

/* Module can span across multiple files. */

#include <linux/kernel.h>
#include <linux/init.h>

void putABreakPointHere(void) { 
    static int siLocal = 0; 

    siLocal++;
    printk(KERN_ALERT "in putABreakPointHere(), siLocal = %d\n", siLocal);

}

/* edrDemo.c - edrDemo in an RTP */

/* Copyright 2003-2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,16dec04,aeg  added inclusion of stdlib.h (SPR #105335).
01b,01oct04,job   Renamed to edrdemo and updated to WR docs format
01a,31oct03,dbs   written
*/

/*
SYNOPSIS

\cs
edrdemo.vxe <hex address to cause page fault>
\ce

DESCRIPTION

ED&R demonstration (i.e. causes a user-level page fault) RTP.

For example, try to access address 0 

\NOROUTINES
*/ 

#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
* main - User application entry function
*
* This routine is the entry point for user application. A real time process
* is created with first task starting at this entry point. 
*
*/

int main
    (
    int	   argc,	/* number of arguments */
    char * argv[],	/* array of arguments */
    char * envp[],	/* array of environment strings */
    void * auxp		/* implementation specific auxiliary vector */
    )
    {
    void * addr;

    if (argc == 2)
	{
	/* get the address to deref from the supplied arg */
	
	addr = (void*) strtoul (argv [1], NULL, 16);

	printf("ED&R Demo - about to page-fault at address %p...\n", addr);

	* ((char *) addr) = 0;
	}
    else
	{
	printf ("usage: edrdemo.rtp <hex-address-to-cause-page-fault>\n");
	}
    
    return 0;
    }

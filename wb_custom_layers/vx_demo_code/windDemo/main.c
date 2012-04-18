/* main.c - example of RTP */

/* Copyright (c) 2003-2005, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,26may10,b_c  LP64 updates.
01j,31mar10,gls  removed taskLockTest()
01i,14aug05,pad  Removed declaration of _exit() prototype since it is provided
                 by unistd.h (part of SPR 110875).
01h,16dec04,aeg  added inclusion of unistd.h (SPR #105335).
01g,04dec03,tam  updated code to use taskRtpLock/taskRtpUnlock
01f,24sep03,aeg  added usage of 'errno' and taskName().
01e,23sep03,kk   remove lib inits from here, done in crt0 now
01d,22sep03,kk   updated to add TLS and heap initialization.
01c,09sep03,aeg  written (based on v01b of rtpDemo/hello2)
*/

/*
DESCRIPTION
User application code.
*/ 

/* local */

#include <vxWorks.h>
#include <taskLib.h>
#include <semLib.h>
#include <stdio.h>
#include <string.h>
#include <errnoLib.h>
#include <unistd.h>

#define TEST_LOG(x,str)     printf(str)

LOCAL SEM_ID    mutexSemId;
LOCAL STATUS    gblErr;

LOCAL int windDemoEntry (_Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                         _Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                         _Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                         _Vx_usr_arg_t);
STATUS rtpWindTest (void);
void semMutexHelper (void);
STATUS semMutexTest1 (void);
STATUS taskPrioritySetTest (void);
void taskPriorityGetTest (_Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                          _Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                          _Vx_usr_arg_t, _Vx_usr_arg_t, _Vx_usr_arg_t,
                          _Vx_usr_arg_t);
int main (int argc, char * argv[], char * envp[], void * auxp);

/*******************************************************************************
* main - User application entry function
*
* This routine is the entry point for user application. A real time process
* is created with first task starting at this entry point. 
* This function writes few messages on the console and exists. 
*
*/

int main
    (
    int    argc,    /* number of arguments */
    char * argv[],  /* array of arguments */
    char * envp[],  /* array of environment strings */
    void * auxp     /* implementation specific auxiliary vector */
    )
    {
    char *  str;    /* temp string pointer */
    size_t  len;    /* length of string */
    int     i = 0;  /* counter */
    ssize_t bytes;  /* written bytes */

    str = "RTP printing all args...\n";

    bytes = write (0, str, 25);  /* system call */

    /* For All arguments print using write system call */

    while (i < argc)
        {
        str =  (char *)argv[i];
        len = strlen (str);

        if (len != 0)
            {

            str[len] = '\n';

            bytes = write (0, str, len+1);  /* system call */
            }

        ++i;
        }
#if 0
    memHeapCreate(0x100000);

    /* -------------------------------------------------------------- */
    /*  START of libvx.a initialization sequence                      */
    /* -------------------------------------------------------------- */

    if (tlsLibInit() == ERROR)
        printf ("FAILED: tlsLibInit() init\n");

    taskLibInit ();

    if (tlsLibInit2() == ERROR)
        printf ("FAILED: tlsLibInit() init\n");

#endif

    /* -------------------------------------------------------------- */
    /*  END of libvx.a initialization sequence                        */
    /* -------------------------------------------------------------- */

#if 0
    rtpWindTest();
#else
    /* spawn a few tasks using the taskLib user-land API */

    taskSpawn ("windDemoTask1", 250, 0, (size_t) 5000, (FUNCPTR) windDemoEntry,
               0L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L);

    taskSpawn ("windDemoTask2", 250, 0, (size_t) 5000, (FUNCPTR) windDemoEntry,
               1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L);
#endif

    

    /* prevent destruction of RTP */

    taskSuspend (0);

    _exit(1);  /* system call */

    /* Never here */
    return 1;
    }


#if 0

/* /wind/river/host/diab/PPCF/rtp/crt0.o contains _start() */

/*******************************************************************************
* _start - underlying application entry function
*
* This routine is the first one being called in user mode by the RTP
* management library. It simply calls main() for now.
*
* Note that the _start() routine will be ultimately provided by the compiler
* toolchain. We declare it here as a temporary solution until we have a crt0
* file to link with.
*/

void _start
    (
    int	   argc,	/* number of arguments */
    char * argv[],	/* array of arguments */
    char * envp[],	/* array of environment strings */
    void * auxp		/* implementation specific auxiliary vector */
    )
    {
    char * str;	/* temporary string pointer */
    int bytes;	/* written bytes */

    str = "_start routine executed...\n";
    bytes = write (0, str, 27);

    main (argc, argv, envp, auxp);
    }
#endif


SEM_ID  semId;

LOCAL int windDemoEntry
    (
    _Vx_usr_arg_t arg1,
    _Vx_usr_arg_t arg2,
    _Vx_usr_arg_t arg3,
    _Vx_usr_arg_t arg4,
    _Vx_usr_arg_t arg5,
    _Vx_usr_arg_t arg6,
    _Vx_usr_arg_t arg7,
    _Vx_usr_arg_t arg8,
    _Vx_usr_arg_t arg9,
    _Vx_usr_arg_t arg10
    )
    {
    printf ("windDemoEntry (%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld)\n"
            "taskName = %s, errno = 0x%x\n",
            arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
            taskName(0), errno);

    
    /* behaviour of remainder of routine based on 'arg1' */

    if (arg1 == 0L)
        {
        semId = semBCreate (0, SEM_EMPTY);

        if (semId == NULL)
            {
            printf ("windDemoEntry: NULL semId\n");
            taskSuspend (0);
            }
        else
            printf ("%s: created semId = 0x%lx\n", taskName (0), semId);

        FOREVER
            {
            semTake (semId, WAIT_FOREVER);
            printf ("%s: acquired semaphore, errno = 0x%x\n", 
                    taskName(taskIdCurrent), errno);
            taskDelay (120 /* sysClkRateGet? */);
            errnoSet (0xdeadbeef);
            semGive (semId);
            }
        }
    else
        {
        if (semId == NULL)
            {
            printf ("windDemoEntry: NULL semId\n");
            taskSuspend (0);
            }

        FOREVER
            {
            semGive (semId);
            semTake (semId, WAIT_FOREVER);
            printf ("%s: acquired semaphore, errno = 0x%x\n", 
                    taskName (taskIdSelf()), errno);
            }
        }

    /* should never reach here */

    taskSuspend (0);
    return 1;
    }

/***************************************************************************
*
* semMutexHelper -
*/

void semMutexHelper (void)
    {
    gblErr = OK;

    if (semTake (mutexSemId, NO_WAIT) != OK)
        {
        TEST_LOG (0, "**** semMutexHelper FAILED to take semaphore!\n");
        gblErr = ERROR;
        return;
        }

    taskDelay (3);          /* Let main task run. */

    semGive (mutexSemId);

    taskDelay (5);          /* Let main task run. */

    semTake (mutexSemId, 60);       /* Pend, and raise pri of owner */
    semGive (mutexSemId);
    }

/***************************************************************************
*
* semMutexTest1 - 
*/

STATUS semMutexTest1 (void)
    {
    int priority;
    int newPriority;
    int i;

    taskPriorityGet (taskIdSelf (), &priority);

    taskSpawn (NULL, priority - 5, 0, (size_t) 20000, (FUNCPTR) semMutexHelper,
               0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L);

    if(gblErr == ERROR)
        return (ERROR);

    if (semTake (mutexSemId, NO_WAIT) == OK)
        {
        TEST_LOG (0, "**** semMutexTest1 FAILED.\n"
                     "**** semTake(NO_WAIT) should not have worked!\n");
        return (ERROR);
        }

    if (semTake (mutexSemId, 5) == ERROR)
        {
        TEST_LOG (0, "**** semMutexTest1 FAILED.\n"
                     "**** semTake(5 ticks) should have worked!\n");
        return (ERROR);
        }

    taskDelay (10);

    taskPriorityGet (taskIdSelf (), &newPriority);
    if (newPriority != (priority - 5))
        {
        TEST_LOG (0, "**** priority inversion not working!\n");
        return (ERROR);
        }

    semGive (mutexSemId);
    taskPriorityGet (taskIdSelf (), &newPriority);
    if (newPriority != priority)
        {
        TEST_LOG (0, "**** priority inversion FAILED to work!\n");
        return (ERROR);
        }

    semTake (mutexSemId, WAIT_FOREVER);

    for (i = 0; i < 10; i++)
        {
        taskSpawn (NULL, priority - 20, 0, (size_t) 10000, (FUNCPTR) semTake,
                   (_Vx_usr_arg_t) mutexSemId, (_Vx_usr_arg_t) WAIT_FOREVER,
                   0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L);
        }

    if (semFlush (mutexSemId) != OK)
        {
        TEST_LOG (0, "**** semFlush() FAILED!\n");
        return (ERROR);
        }

    TEST_LOG (0, "Basic mutex semaphore tests PASSED.\n");
    return (OK);
    }

/***************************************************************************
*
* taskPrioritySetTest - tests taskPrioritySet
*/

STATUS taskPrioritySetTest (void)
    {
    int newPriority;
    int oldPriority;
    int priority;

    taskPriorityGet (taskIdSelf (), &oldPriority);

    newPriority = (oldPriority < 5) ? oldPriority + 5 : oldPriority - 2;

    if (taskPrioritySet (taskIdSelf (), 255) == OK)
        {
        TEST_LOG (0, "**** taskPrioritySet FAILED!\n"
                     "**** Should not have changed priority!\n");
        return (ERROR);
        }

    taskPriorityGet (taskIdSelf (), &priority);
    if (priority != oldPriority)
        {
        TEST_LOG (0, "**** taskPrioritySet() FAILED!\n"
                     "**** Should not have changed priority!\n");
        return (ERROR);
        }

    if (taskPrioritySet (taskIdSelf (), newPriority) == OK)
        {
        TEST_LOG (0, "**** taskPrioritySet FAILED!\n"
                     "**** Should have changed priority!\n");
        return (ERROR);
        }

    taskPriorityGet (taskIdSelf (), &priority);
    if (priority != newPriority)
        {
        TEST_LOG (0, "**** taskPrioritySet() FAILED!\n"
                     "**** Should have changed priority!\n");
        return (ERROR);
        }

    taskPrioritySet (taskIdSelf (), oldPriority);

    TEST_LOG (0, "taskPrioritySet() PASSED.\n");
    return (OK);
    }

/***************************************************************************
*
* taskPriorityGetTest - tests taskPriorityGet(), taskDelay(), taskSpawn()
*/

void taskPriorityGetTest
    (
    _Vx_usr_arg_t myPriority,
    _Vx_usr_arg_t arg2,
    _Vx_usr_arg_t arg3,
    _Vx_usr_arg_t arg4,
    _Vx_usr_arg_t arg5,
    _Vx_usr_arg_t arg6,
    _Vx_usr_arg_t arg7,
    _Vx_usr_arg_t arg8,
    _Vx_usr_arg_t arg9,
    _Vx_usr_arg_t arg10
    )
    {
    int     priority;
    STATUS  status;

    status = taskPriorityGet (taskIdSelf (), &priority);

    if ((status != OK) || ((int) myPriority != priority))
        {
        TEST_LOG (0, "**** taskPriorityGet() FAILED!\n");

        gblErr = ERROR;
        return;
        }
    gblErr = OK;

    return ;
    }

/***************************************************************************
*
* rtpWindTest - test entry point
*/

STATUS rtpWindTest (void)
    {
    int     i;
    TASK_ID tid;

    mutexSemId = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    if (mutexSemId == NULL)
        {
        TEST_LOG (0, "**** FAILED to create mutex semaphore!\n");

        return (ERROR);
        }

    for (i = 0; i <= 255; i++)
        {
        gblErr = ERROR;

        tid = taskSpawn ("spawnTest", i, 0, (size_t) 5000,
                         (FUNCPTR) taskPriorityGetTest, (_Vx_usr_arg_t) i,
                         0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L);

        if (tid == TASK_ID_ERROR)
            {
            TEST_LOG (0, "**** rtpWindTest FAILED to spawn task!\n");

            return (ERROR);
            }

        if (taskDelay (1) != OK)
            {
            TEST_LOG (0, "**** rtpWindTest FAILED to delay task!\n");
            return (ERROR);
            }

        if (gblErr != OK)
            {
            TEST_LOG (0, "rtpWindTest - FAILED!!!\n");
            return (ERROR);
            }
        }

    TEST_LOG (0, "rtpWindTest - taskPrioritySpawn() PASSED!\n"
                 "rtpWindTest - taskPriorityGet() PASSED!\n"
                 "rtpWindTest - taskDelay() PASSED!\n");

    if (taskPrioritySetTest () == ERROR)
        {
        TEST_LOG (0, "**** rtpWindTest - taskPrioritySetTest() FAILED!\n");
        return (ERROR);
        }

    if (semMutexTest1 () == ERROR)
        {
        TEST_LOG (0, "**** rtpWindTest - semMutexTest1() FAILED!\n");
        return (ERROR);
        }

    if (semDelete (mutexSemId) == ERROR)
        {
        TEST_LOG (0, "**** rtpWindTest - FAILED to delete mutex semaphore!\n");
        return (ERROR);
        }

    TEST_LOG (0, "**** rtpWindTest - All tests PASSED.\nDone.\n");
    return (OK);
    }


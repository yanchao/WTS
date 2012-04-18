/* phil.c - Dining Philosophers problem */

/*
 * Copyright (c) 2009-2010 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,28jul10,pad  completed LP64 adaptation: use TASK_ID type and TASK_ID_ERROR
		 macro where necessary.
01e,03feb10,jpb  Updated for LP64 adaptation.  Replaced sleep()
                 with taskDelay() to remove dependency on pthreadLib.
01d,31aug09,kk   clean up documentation
01c,12may09,h_l  adjust philosopher and fork index in log message.
01b,22apr09,h_l  add CPU index info in log message.
01a,21jan09,pad  written.
*/

/*
SYNOPSIS
RTP:
phil.vxe [-s <solution>] [-n <number of philosophers>] [-d <duration (seconds)>]

Kernel:
philDemo ([0/solution], [0/number of philosophers], [0/duration (seconds)])

DESCRIPTION
This demo demonstrates multitasking within the VxWorks kernel by providing two 
solutions to Dijkstra's famous dining philosophers problem. The main goal 
of the problem is to find ways to avoid both deadlock and starvation 
when a finite set of actors share a finite set of resources that
can be used by one actor only at a time.

The problem is described as follows: five philosophers sit around a table to
think and eat. Between each adjacent philosopher there is a fork. When a
philosopher is done thinking she/he needs to grab the two forks on his/her
immediate right and left to be able to eat. When the philosopher is done eating
she/he puts down the forks and go back to thinking for a while till he or she
is hungry again, etc., etc.

In this implementation the number of philosophers can be changed (five being
the default). Also the duration of a philosopher's thinking phase is not the
same for all. This brings a more realistic touch to the situation since,
usually, actors accessing a resource do not all have the same frequency of
access to that resource. This implementation simply lets a philosopher think for
a number of seconds equal to the philosopher's order number around the table
(i.e. 1 to x) instead of some random time or an equal amount of time for all.

The two solutions implemented here are 1) a ticket-based one, and 2) a
claim-based one. The ticket-based solution is fair in the sense that all
philosophers get to access the resource for the same amount of time in average
whether they think quick (or shallowy...) or long. The drawback is that the
faster thinkers get to wait more for accessing the resource. The claim-based
solution is addressing this by letting the faster thinkers access the resource
as long as it is not claimed by another philosopher (in which case the
requestor still has to wait until the other philosopher has gotten a chance to
use the resource).

CONFIGURATION
To run this demo for SMP, configure the INCLUDE_SMP_DEMO component. This
demo will then be configured into the VxWorks image.

For VxWorks UP, this demo is not available yet.

To execute the demo, please refer to the SYNOPSIS section above.

INCLUDE FILES: N/A
*/

/* Includes */

#include <stdio.h>
#include <stdlib.h>
#include <semLib.h>
#include <taskLib.h>
#include <string.h>  /* for strcpy */
#include <sysLib.h>  /* for sysClkRateGet() API */

#ifdef _WRS_KERNEL
#include <vxCpuLib.h>
#endif /* _WRS_KERNEL */

/* Defines */

#define FORK_AVAILABLE    0
#define FORK_IN_USE       1

#define NOBODY           -1

#define CLAIM_BASED       1
#define TICKET_BASED      2

#define PRINT_LOCK_ACQUIRE   semTake (semOutId, WAIT_FOREVER)
#define PRINT_LOCK_RELEASE   semGive (semOutId)

/* externs */

extern char * optarg;      /* argument associated with option */

/* Typedefs */

typedef struct philosophers_attributes
    {
    int numPhil;	/* number of philosophers */
    TASK_ID * pPhil;	/* Points to an array of philosopher task IDs */
    SEM_ID syncSemId;	/* Synchronization semaphore */
    BOOL * iAmReady;	/* Array collecting philosopher's ready status */
    } PHILOSOPHERS_ATTR;

typedef struct fork
    {
    int status;         /* FORK_AVAILABLE or FORK_IN_USE */
    int philosopher;    /* Philosopher requesting the fork */
    } A_FORK;

typedef struct forks_attributes
    {
    int numForks;       /* number of forks */
    A_FORK * forks;     /* The set of forks on the table */
    SEM_ID forkMutex;   /* Protection mutex for the fork resource */
    } FORKS_ATTR;

typedef struct statictics
    {
    unsigned int * goHungry;    /* The number of times a philosopher waits */
    unsigned int * foodIntake;  /* the number of times a philosopher eats */
    } STATS;

/* locals */

LOCAL SEM_ID   semOutId = NULL;       /* semaphore for output message */

/* Forward declarations */

void philosopherRun (int myNum, int solution, 
                     PHILOSOPHERS_ATTR * pPhilsAttr,
                     FORKS_ATTR * pForksAttr, STATS * pStats);
BOOL forksGet       (int myNum, int solution, 
                     FORKS_ATTR * pForksAttr, char *cpuNumStr);
void forksPut       (int myNum, int solution, 
                     FORKS_ATTR * pForksAttr, char *cpuNumStr);

/*******************************************************************************
*
* philDemo - Dining philosopher demo
* 
* This is the main entry of the dining philosopher's demo.
* 
* Options to this demo are:
*
* \is
* \i solutionNum
* Selection the solution to run the demo. 1 = ticket-based solution, 2=
* claim-based solution. Default (0) is set to solution #1.
* \i philosopherNum
* Indicate the number of philosopher tasks to run. Default (0) is 5 tasks.
* \i duration
* Length of the time to run the demo. Default (0) is run forever.
* \ie
*
* If the demo is left to run forever (the default) then it will stop and
* print statistics when the main task is resumed from the shell.
*
* RETURNS: N/A
* ERRNO: N/A
*/

#ifdef _WRS_KERNEL
void philDemo
    (
    int solutionNum,     /* Solution to run (1 or 2). Default (0) is sol. 1 */
    int philosopherNum,  /* Number of philosopher tasks. Default (0) is 5 */
    int duration         /* Duration of demo. Default (0) is forever */
    )
#else
int main
    (
    int argc,
    char * argv[]
    )
#endif /* _WRS_KERNEL */
    {
#ifndef _WRS_KERNEL
    int                 options;        /* Demo's options */
#endif /* _WRS_KERNEL */
    unsigned int        solution;       /* Either one of the impl. solutions */
    PHILOSOPHERS_ATTR   philosAttr;     /* Philosopher attributes */
    FORKS_ATTR          forksAttr;      /* Fork attributes */
    STATS               stats;          /* Statistic data */
    int                 i;              /* Loop counter */
    int     demoDuration = 0;           /* By default the demo runs forever */
    BOOL    philosophersNotReady = TRUE;/* FALSE when all philosopher tasks */
                                        /* are ready to go on */

    /* By default let's run the claim-based solution */

    solution = CLAIM_BASED;

    /* Default number of philosophers */

    philosAttr.numPhil = 5;

    /* Decipher the options, if any is passed */

#ifdef _WRS_KERNEL
    if ((solutionNum > 0) && (solutionNum < 3))
        solution = solutionNum;

    if (philosopherNum > 5)
        philosAttr.numPhil = philosopherNum;

    demoDuration = duration;
#else
    while ((options = getopt (argc, argv, "n:s:d:")) != EOF)
        {
        switch (options)
            {
            case 'd':
                demoDuration = atoi (optarg);
                break;

            case 'n':
                philosAttr.numPhil = atoi (optarg);
                break;

            case 's':
                solution = atoi (optarg);
                if (solution > 2)
                    solution = CLAIM_BASED;
                break;

            default:
                printf ("\nOnly -s <solution>, -n <number of philosophers> and "
                        "-d <seconds> are supported, with <solution> being "
                        "either ticket-based (1) or claim-based (2) and "
                        "<seconds> being the number of seconds the demo runs "
                        "(it runs forever if this is not specified).\n\n");
                exit (1);
                break;
            }
        }
#endif /* _WRS_KERNEL */

    /* There is as much forks as there are philosophers */

    forksAttr.numForks = philosAttr.numPhil;

    /* 
     * Print some instructions to make it a bit easier to end the demo if left
     * to run forever.
     */

    if (demoDuration == 0)
        {
        taskDelay (sysClkRateGet());
#ifdef _WRS_KERNEL
        printf ("\nResume the main task (ID = %p) to end the demo\n\n",
        taskIdSelf());
#else
        printf ("\nResume the RTP's initial task to end the demo.\n\n");
#endif /* _WRS_KERNEL */
        taskDelay (sysClkRateGet() * 3);
        }

    if (solution == CLAIM_BASED)
        printf ("\nRunning claim-based solution.\n\n");
    else
        printf ("\nRunning ticket-based solution.\n\n");
    
    /* Create output message semaphore */

    semOutId = semBCreate (SEM_Q_FIFO, SEM_FULL);

    /* Create synchronization semaphore */

    philosAttr.syncSemId = semBCreate (SEM_Q_FIFO, SEM_EMPTY);

    /* Create the various arrays used in this implementation */

    philosAttr.pPhil    = (TASK_ID *) calloc ((size_t)philosAttr.numPhil,
					      sizeof (TASK_ID));
    philosAttr.iAmReady = (BOOL *) calloc ((size_t)philosAttr.numPhil,
                                           sizeof (BOOL));
    stats.foodIntake    = (unsigned int *) calloc ((size_t)philosAttr.numPhil,
                                                   sizeof (unsigned int));
    forksAttr.forks     = (struct fork *)  calloc ((size_t)philosAttr.numPhil,
                                                   sizeof (A_FORK));
    stats.goHungry      = (unsigned int *) calloc ((size_t)philosAttr.numPhil,
                                                   sizeof (unsigned int));

    /* Initialize the owner of the forks */

    for (i = 0; i < philosAttr.numPhil; i++)
        forksAttr.forks[i].philosopher = NOBODY;

    /* Create mutex semaphore protecting the critical sections */

    if ((forksAttr.forkMutex = semMCreate (SEM_Q_FIFO)) == NULL)
        {
        printf ("Cannot create mutex semaphore for forks.\n");
        exit (1);
        }

    /* Create philosopher tasks */

    for (i = 0; i < philosAttr.numPhil; i++)
        {
        if ((philosAttr.pPhil[i] = taskSpawn ("tPhil", 200, 0, 0x1000,
                                              (FUNCPTR) philosopherRun, 
                                              i,
                                              solution, 
                                              (_Vx_usr_arg_t) &philosAttr,
                                              (_Vx_usr_arg_t) &forksAttr, 
                                              (_Vx_usr_arg_t) &stats,
                                              0L,0L,0L,0L,0L)) == TASK_ID_ERROR)
            {
            printf ("Cannot create task for philosopher %d.\n", i + 1);
            exit (1);
            }
        }

    /*
     * Wait for all the philosopher tasks to be pending on the synchronization
     * semaphore.
     */

    while (philosophersNotReady)
        {
        philosophersNotReady = FALSE;

        for (i = 0; i < philosAttr.numPhil; i++)
            {
            if (philosAttr.iAmReady[i] == FALSE)
                philosophersNotReady = TRUE;
            }

        taskDelay (10);
        }

    /* Let the philosophers start their meal */

    semFlush (philosAttr.syncSemId);

    /* Let the demo run as much as one wants */

    if (demoDuration == 0)
        taskSuspend (0);
    else
        taskDelay (sysClkRateGet() * demoDuration);

    /* Then kill the philosopher tasks, print statistics and exit */

    for (i = 0; i < philosAttr.numPhil; i++)
        taskDelete (philosAttr.pPhil[i]);

    for (i = 0; i < philosAttr.numPhil; i++)
        printf ("\nPhilosopher %d has eaten %d times and waited %d times.\n",
                i + 1, stats.foodIntake[i], stats.goHungry[i]);

    /* Free resources */

    semDelete (philosAttr.syncSemId);
    free (philosAttr.pPhil);
    free (philosAttr.iAmReady);
    free (stats.foodIntake);
    free (stats.goHungry);
    semDelete (forksAttr.forkMutex);
    free (forksAttr.forks);
    semDelete (semOutId);
    semOutId = NULL;

#ifndef _WRS_KERNEL
    exit (0);
#endif /* _WRS_KERNEL */
    }

/*******************************************************************************
*
* philosopherRun - A philosopher task's run routine
* 
* This routine is the starting point for each of the philosopher task.
*
* RETURNS: N/A
* ERRNO: N/A
*
* \NOMANUAL
*/

void philosopherRun
    (
    int                 myNum,      /* This philosopher's order number */
    int                 solution,   /* Solution to execute */
    PHILOSOPHERS_ATTR * pPhilsAttr, /* Philosophers attributes */
    FORKS_ATTR *        pForksAttr, /* Forks attributes*/
    STATS *             pStats      /* statistics data */
    )
    {
    char cpuNumStr[20];
    pPhilsAttr->iAmReady[myNum] = TRUE;

    /* Philosopher waits till every one is ready */

    semTake (pPhilsAttr->syncSemId, WAIT_FOREVER);

    /* Now start thinking and eating */

    while (TRUE)
        {
#ifdef _WRS_KERNEL
#ifdef _WRS_CONFIG_SMP        
        sprintf (cpuNumStr, "(CPU%d)", vxCpuIndexGet()); 
#else        
        strcpy (cpuNumStr, "");
#endif /* _WRS_CONFIG_SMP */
#else
        strcpy (cpuNumStr, "");
#endif /* _WRS_KERNEL */

        PRINT_LOCK_ACQUIRE;
        printf ("Philosopher %d is thinking...%s\n", myNum + 1, cpuNumStr);
        PRINT_LOCK_RELEASE;

        taskDelay (sysClkRateGet() * (myNum + 1));

        PRINT_LOCK_ACQUIRE;
        printf ("Philosopher %d wants to eat.%s\n", myNum + 1, cpuNumStr);
        PRINT_LOCK_RELEASE;
        
        /*
         * If the forks cannot be acquired this implementation simply does
         * an active wait and checks again for the fork's availability after a
         * second of delay.
         */

        while (forksGet (myNum, solution, pForksAttr, cpuNumStr) == FALSE)
            {
            if (solution == TICKET_BASED)
                {
                PRINT_LOCK_ACQUIRE;
                printf ("Philosopher %d must wait for forks.%s\n", myNum + 1,
			cpuNumStr);
                PRINT_LOCK_RELEASE;
                }

            pStats->goHungry[myNum]++;
            taskDelay (sysClkRateGet());
            }

        PRINT_LOCK_ACQUIRE;
        printf ("Philosopher %d is eating...%s\n", myNum + 1, cpuNumStr);
        PRINT_LOCK_RELEASE;

        pStats->foodIntake[myNum]++;
        taskDelay (sysClkRateGet());

        forksPut (myNum, solution, pForksAttr, cpuNumStr);
        }
    }

/*******************************************************************************
* 
* forksGet - Get the right-hand and left-hand pair of forks
* 
* This routine attempts to acquire the forks a philosopher needs to be able to
* eat. Forks are always acquired in pair, i.e. if only one of the forks is
* available then no fork is acquired.
* 
* RETURNS: TRUE if both forks are acquired, FALSE otherwise.
* ERRNO: N/A
*
* \NOMANUAL
*/

BOOL forksGet
    (
    int             myNum,      /* Philosopher's order number */
    int             solution,   /* Solution to execute */
    FORKS_ATTR *    pForksAttr, /* Forks attributes*/
    char *          cpuNumStr   /* CPU index info*/
    )
    {
    BOOL forksAcquired  = FALSE;
    BOOL passTurn       = FALSE;
    int  rightFork      = myNum;
    int  leftFork       = (myNum + 1) % pForksAttr->numForks;

    /* Acquire the mutex protecting the critical section */

    semTake (pForksAttr->forkMutex, WAIT_FOREVER);

    if (solution == TICKET_BASED)
        {
        /* Check whether it is this philosopher's turn to get the forks */

        if (((pForksAttr->forks[rightFork].philosopher == myNum) ||
             (pForksAttr->forks[rightFork].philosopher == NOBODY)) &&
            ((pForksAttr->forks[leftFork].philosopher  == myNum) ||
             (pForksAttr->forks[leftFork].philosopher  == NOBODY)))
            forksAcquired = TRUE;
        }
    else /* Claim-based solution */
        {
        /* 
         * A fork can be acquired only if it is not in use and if no one else
         * has claimed it. If the fork is in use already then the philosopher
         * simply claims it and waits for it to be available. If the fork is
         * available but has already been claimed then the philosopher must
         * wait for his/her turn.
         * 
         * Note: if we were to randomize the think time of the philosophers it
         *       might be more efficient for a philosopher to claim the fork
         *       when the fork is available but has been claimed by the other
         *       philosopher (i.e. implement a two-slot queue for the fork).
         */

        if (pForksAttr->forks[rightFork].status == FORK_IN_USE)
            {
            pForksAttr->forks[rightFork].philosopher = myNum;
            passTurn = TRUE;
            PRINT_LOCK_ACQUIRE;
            printf ("Philosopher %d requested usage of the fork %d.%s\n",
                    myNum + 1, rightFork + 1, cpuNumStr);
            PRINT_LOCK_RELEASE;
            }
        else
            if ((pForksAttr->forks[rightFork].philosopher != myNum) &&
                (pForksAttr->forks[rightFork].philosopher != NOBODY))
                {
                passTurn = TRUE;

                PRINT_LOCK_ACQUIRE;
                printf ("Philosopher %d must wait to use the fork %d.%s\n",
                        myNum + 1, rightFork + 1, cpuNumStr);
                PRINT_LOCK_RELEASE;
                }

        if (pForksAttr->forks[leftFork].status == FORK_IN_USE)
            {
            pForksAttr->forks[leftFork].philosopher = myNum;
            passTurn = TRUE;

            PRINT_LOCK_ACQUIRE;
            printf ("Philosopher %d requested usage of the fork %d.%s\n",
                    myNum + 1, leftFork + 1, cpuNumStr);
            PRINT_LOCK_RELEASE;
            }
        else
            if ((pForksAttr->forks[leftFork].philosopher != myNum) &
                (pForksAttr->forks[leftFork].philosopher != NOBODY))
                {
                passTurn = TRUE;

                PRINT_LOCK_ACQUIRE;
                printf ("Philosopher %d must wait to use the fork %d.%s\n",
                        myNum + 1, leftFork + 1, cpuNumStr);
                PRINT_LOCK_RELEASE;
                }

        /* 
         * if both forks are available and claimed by no one else then the
         * philosopher may acquire them.
         */

        if (!passTurn)
            {
            pForksAttr->forks[rightFork].status      = FORK_IN_USE;
            pForksAttr->forks[leftFork].status       = FORK_IN_USE;
            pForksAttr->forks[rightFork].philosopher = NOBODY;
            pForksAttr->forks[leftFork].philosopher  = NOBODY;

            forksAcquired = TRUE;
            }
        }

    /* Release the mutex protecting the critical section */

    semGive (pForksAttr->forkMutex);

    return forksAcquired;
    }

/*******************************************************************************
* 
* forksPut - Put down the right-hand and left-hand pair of forks
* 
* This routine simply releases the forks a philosopher was using to eat.
* 
* RETURNS: N/A
* ERRNO: N/A
*
* \NOMANUAL
*/

void forksPut
    (
    int             myNum,      /* Philosopher's order number */
    int             solution,   /* Solution to execute */
    FORKS_ATTR *    pForksAttr, /* Forks attributes*/
    char *          cpuNumStr   /* CPU index info*/
    )
    {
    int rightFork = myNum;
    int leftFork  = (myNum + 1) % pForksAttr->numForks;

    /* Acquire the mutex protecting the critical section */

    semTake (pForksAttr->forkMutex, WAIT_FOREVER);

    if (solution == TICKET_BASED)
        {
        /*
         * The forks are handed over to the philosophers on the immediate right
         * and immediate left of the philosopher that is done eating.
         */

        pForksAttr->forks[rightFork].philosopher = (rightFork ? rightFork - 1 :
                                                    (pForksAttr->numForks - 1));
        pForksAttr->forks[leftFork].philosopher = leftFork;

        PRINT_LOCK_ACQUIRE;
        printf ("Philosopher %d gave fork %d to philosopher %d and fork %d to "
                "philosopher %d.%s\n", myNum + 1, rightFork + 1,
                pForksAttr->forks[myNum].philosopher + 1, leftFork + 1,
                pForksAttr->forks[leftFork].philosopher + 1, cpuNumStr);
        PRINT_LOCK_RELEASE;
        }
        else	/* Claim-based solution */
            {
            /* The forks are simply flagged as available */

            pForksAttr->forks[rightFork].status = FORK_AVAILABLE;
            pForksAttr->forks[leftFork].status  = FORK_AVAILABLE;
            }

    /* Release the mutex protecting the critical section */

    semGive (pForksAttr->forkMutex);
    }


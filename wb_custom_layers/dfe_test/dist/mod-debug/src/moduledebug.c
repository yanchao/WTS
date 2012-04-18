/* moduledebug.c - a sample test for linux kernel modules */

/* Copyright 1984-2004 Wind River Systems, Inc. */

/*
   modification history
   --------------------
   01a,12mar04,tbu,ms   written.
   01v,04jun04,eab      modified for tutorial.
   */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>

/* defines */

#define DRIVER_AUTHOR "Wind River Workbench Engineers"
#define DRIVER_DESC   "Module Debug Sample"

#define PRINTF(args...) printk(KERN_ALERT "" args);

/* globals */

char *globalData="GLOBAL";
int count = 0;
char globalChar = 'X';
int *globalPtr = &count;
char *globalCharPtr = &globalChar;
float globalFloat = 1.13;

/* locals */

static char *privateData="PRIVATE";
static wait_queue_head_t wait;

static DECLARE_COMPLETION(evt_dead);
static pid_t pid;

static int moduleTestExiting = 0;

/* forward declarations */

static int  initModuleTest(void);
static void exitModuleTest(void);

static void moduleTestLoop(void);

extern void putABreakPointHere (void);


/* functions declarations */

/******************************************************************************
 *
 * initModuleTest - the module init function
 *
 * This initializes our module and launches a thread.
 *
 * RETURNS: 0 (OK) always
 *
 * NOMANUAL
 */

static int  initModuleTest ( void ) {
    PRINTF("Welcome to %s !\n", DRIVER_DESC);
    PRINTF("privateData is at 0x%p. globalData is at 0x%p\n",privateData, globalData);
    PRINTF("Address of putABreakPointHere is 0x%p.\n",putABreakPointHere);

    init_waitqueue_head (&wait);

    pid =  kernel_thread ((int (*)(void*)) moduleTestLoop, (void*) 0, CLONE_SIGHAND);

    PRINTF("Module %s loaded.  initModuleTest exiting.\n",DRIVER_DESC);

    return 0;
}


/******************************************************************************
 *
 * exitModuleTest - the module exit function
 *
 * This performs our module cleanup and kills the thread.
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void exitModuleTest ( void ) {
    struct pid *pid_ptr;

    PRINTF("Killing module thread ...\n");

    moduleTestExiting = 1;
    pid_ptr = find_get_pid(pid);
    if (!kill_pid (pid_ptr,SIGTERM,1)) {
        PRINTF("Unable to kill thread. Waiting for completion !\n");
        wake_up (&wait);	
        wait_for_completion(&evt_dead);
    }
    PRINTF("Bye bye, %s !\n",DRIVER_DESC);	    
}

/******************************************************************************
 *
 * moduleTestLoop - the module thread entry point
 *
 * This calls the function where we want to plant a breakpoint, in
 * a loop with timeout
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void moduleTestLoop ( void ) {
    int localCount = 0;

    while (1) {
        putABreakPointHere();
        /* Waits 1 sec */
        sleep_on_timeout(&wait,500);
        PRINTF("moduleTest thread woke up. Global is %s !\n", globalData);
        localCount++;
        count += 2;

        if (moduleTestExiting) {
            PRINTF("localCount = %d; count = %d; moduleTest thread exiting!\n", localCount, count);
            complete_and_exit(&evt_dead,0);
        }
    }
}

EXPORT_SYMBOL(count);

/* module declarations */

module_init(initModuleTest);
module_exit(exitModuleTest);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

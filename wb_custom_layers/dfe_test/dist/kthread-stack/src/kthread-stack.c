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
#include "stack_header.h"

/* defines */

#define DRIVER_AUTHOR "Wind River Workbench Engineers"
#define DRIVER_DESC   "Module Debug Sample"

#define PRINTF(args...) printk(KERN_ALERT "" args);

/* globals */

INNER_STRUCT_TYPE1  global_s1; 
char *globalData="GLOBAL";
int count = 0;
char globalChar = 'X';
int *globalPtr = &count;
char *globalCharPtr = &globalChar;
float globalFloat = 1.13;

char counter0 = 0;
short counter1 = 0;
int counter2 = 0;
long counter3 = 0;
long long counter4 = 0;


/* locals */

static char *privateData="PRIVATE";
static wait_queue_head_t wait;

static DECLARE_COMPLETION(evt_dead);
static pid_t pid;

static int dfe_exit = 0;

/* forward declarations */

static int  init_dfe(void);
static void exit_dfe(void);

static void dfe_run(void);

extern void sample_stack (void);
extern void mid_stack (void);


/* functions declarations */

/******************************************************************************
 *
 * init_dfe - the kthread init function
 *
 * This initializes our module and launches a thread.
 *
 * RETURNS: 0 (OK) always
 *
 * NOMANUAL
 */

static int  init_dfe ( void ) {
    PRINTF("Welcome to %s !\n", DRIVER_DESC);
    PRINTF("privateData is at 0x%p. globalData is at 0x%p\n",privateData, globalData);

    init_waitqueue_head (&wait);

    pid =  kernel_thread ((int (*)(void*)) dfe_run, (void*) 0, CLONE_SIGHAND);

    PRINTF("Module %s loaded.  init_dfe exiting.\n",DRIVER_DESC);

    return 0;
}


/******************************************************************************
 *
 * exit_dfe - the module exit function
 *
 * This performs our module cleanup and kills the thread.
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void exit_dfe ( void ) {
    struct pid *pid_ptr;

    PRINTF("Killing module thread ...\n");

    dfe_exit = 1;
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
 * dfe_run - the module thread entry point
 *
 * This calls the function where we want to plant a breakpoint, in
 * a loop with timeout
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void dfe_run ( void ) {
    int localCount = 0;

    while (1) {

        simple_stack1();
        /* Waits 1 sec */
        sleep_on_timeout(&wait,100);
        PRINTF("moduleTest thread woke up. Global is %s !\n", globalData);
        localCount++;
        count += 2;

        mid_stack1();
        if (dfe_exit) {
            PRINTF("localCount = %d; count = %d; kthread exiting!\n", localCount, count);
            complete_and_exit(&evt_dead,0);
        }
    }
}

EXPORT_SYMBOL(count);

/* module declarations */

module_init(init_dfe);
module_exit(exit_dfe);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

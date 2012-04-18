/*
 * =====================================================================================
 *
 *       Filename:  mod_debug.c
 *
 *    Description:  for DFE WR Linux kernel testing
 *
 *        Version:  1.0
 *        Created:  06/23/2011 03:12:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
 *        Company:  WindRiver System
 *
 * =====================================================================================
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
int dfe_count = 0;
int *globalPtr = &dfe_count;
float globalFloat = 1.13;

/* locals */

static wait_queue_head_t wait;

static DECLARE_COMPLETION(evt_dead);
static pid_t pid;

static int dfe_gone = 0;

/* forward declarations */

static int  wb_dfe_init(void);
static void wb_dfe_exit(void);

static void mod_thread(void);

extern void mid_stack1 (void);


/* functions declarations */

/******************************************************************************
 *
 * wb_dfe_init - the module init function
 *
 * This initializes our module and launches a thread.
 *
 * RETURNS: 0 (OK) always
 *
 * NOMANUAL
 */

static int  wb_dfe_init ( void ) 
{
    PRINTF("Welcome to %s !\n", DRIVER_DESC);

    init_waitqueue_head (&wait);

    pid =  kernel_thread ((int (*)(void*)) mod_thread, (void*) 0, CLONE_SIGHAND);

    PRINTF("Module %s loaded.  wb_dfe_init exiting.\n",DRIVER_DESC);

    return 0;
}


/******************************************************************************
 *
 * wb_dfe_exit - the module exit function
 *
 * This performs our module cleanup and kills the thread.
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void wb_dfe_exit ( void ) 
{
    struct pid *pid_ptr;

    PRINTF("Killing module thread ...\n");

    dfe_gone = 1;
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
 * mod_thread - the module thread entry point
 *
 * This calls the function where we want to plant a breakpoint, in
 * a loop with timeout
 *
 * RETURNS: N/A
 *
 * NOMANUAL
 */

static void mod_thread ( void ) 
{
    int dfe_local = 0;

    while (1) {
        mid_stack1();
        /* Waits 1 sec */
        sleep_on_timeout(&wait,500);
        PRINTF("module thread woke up. Global is %s !\n", globalData);
        dfe_local++;
        dfe_count += 2;

        if (dfe_gone) {
            PRINTF("dfe_local = %d; dfe_count = %d; module thread exiting!\n", dfe_local,dfe_count);
            complete_and_exit(&evt_dead,0);
        }
    }
}

EXPORT_SYMBOL(dfe_count);

/* module declarations */

module_init(wb_dfe_init);
module_exit(wb_dfe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

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

#define KERNEL_THREAD_NUMBERS 32

/* globals */

static struct task_struct * pid[KERNEL_THREAD_NUMBERS]; 

char *globalData="GLOBAL";
atomic_t dfe_count = ATOMIC_INIT(0);
float globalFloat = 1.13;

/* locals */

static int dfe_nr_threads = 0;

/* forward declarations */

static int  wb_dfe_init(void);
static void wb_dfe_exit(void);

static int mod_thread(void *i);

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
    int i;
    unsigned int nr_online_cpus = num_online_cpus();

    PRINTF("Welcome to %s !\n", DRIVER_DESC);
    PRINTF("number of online cpus = %d \n", nr_online_cpus);

    dfe_nr_threads = 2 * nr_online_cpus;
    if (dfe_nr_threads > KERNEL_THREAD_NUMBERS) {
        dfe_nr_threads = KERNEL_THREAD_NUMBERS;
    }
    for (i = 0; i < dfe_nr_threads; i++) {
        pid[i] = NULL;
    }

    for (i = 0; i < dfe_nr_threads; i++) {
        pid[i] = kthread_create(mod_thread, (void *)i, "dfe_%d_alive",i);
		if (pid[i]) {
			wake_up_process(pid[i]);
		}
    }

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
    int i ;
    PRINTF("Killing module thread ...\n");

    for (i = 0; i < dfe_nr_threads; i++) {
		kthread_stop(pid[i]);
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

static int mod_thread ( void *i ) 
{
    int dfe_local = 0;
    int th_id = (int *)i; 
    int seconds = (th_id + 1) * 1000; 
    unsigned int new_core_id = raw_smp_processor_id();
    unsigned int old_core_id = new_core_id;

	/* Request delivery of SIGKILL */
	allow_signal(SIGKILL);

    while (1) {
		if (kthread_should_stop()) {
            PRINTF("dfe_local = %d; dfe_count = %d; module thread exiting!\n", dfe_local,atomic_read(&dfe_count));
			break;
        }

        mid_stack1();
        /* Waits 1 sec */
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(seconds);
        /*-----------------------------------------------------------------------------
         *  add thread transfer code
         *  first ,update new_core_id
         *-----------------------------------------------------------------------------*/
        new_core_id = raw_smp_processor_id();
        if (new_core_id != old_core_id) {
            PRINTF ("kernel thread %ld is transfered from core %d to core %d.\n", th_id, old_core_id, new_core_id);
        }
        /*  update old_core_id to new one */
        old_core_id = new_core_id;

        PRINTF("kernel thread %d  woke up on core %d . thread id is %d !\n", th_id,raw_smp_processor_id(),pid[th_id]->pid); 
        dfe_local++;
        atomic_add(2,&dfe_count);
    }
}

EXPORT_SYMBOL(dfe_count);
EXPORT_SYMBOL(dfe_nr_threads);

/* module declarations */

module_init(wb_dfe_init);
module_exit(wb_dfe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

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
#include <linux/kthread.h>
#include <linux/threads.h>
#include <linux/cpuset.h>
#include <linux/sched.h>

/* defines */

#define DRIVER_AUTHOR "Wind River Workbench Engineers"
#define DRIVER_DESC   "Module Debug Sample"

#define PRINTF(args...) printk(KERN_ALERT "" args);

#define KERNEL_THREAD_NUMBERS 32

/* globals */

static pid_t pid[KERNEL_THREAD_NUMBERS]; 
static int   killed[KERNEL_THREAD_NUMBERS];

char *globalData="GLOBAL";
atomic_t dfe_count = ATOMIC_INIT(0);
float globalFloat = 1.13;

/* locals */

static wait_queue_head_t wait;

static DECLARE_COMPLETION(evt_dead);

static int dfe_gone = 0;
static int dfe_nr_threads = 0;

/* forward declarations */

static int  wb_dfe_init(void);
static void wb_dfe_exit(void);

static void mod_thread(int i);

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

    init_waitqueue_head (&wait);

    for (i = 0; i < dfe_nr_threads; i++) {
        pid[i] = kernel_thread((int(*)(void*)) mod_thread, (void*) i,
                CLONE_SIGHAND);
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
    struct pid *pid_ptr;
    int pid_index = 0;
    int i ;
    PRINTF("Killing module thread ...\n");
    for (i = 0; i < dfe_nr_threads; i++) {
        killed[i] = 0;
    }
    dfe_gone = 1;
    while (pid_index < dfe_nr_threads) {
        for (i = 0; i < dfe_nr_threads; i++) {
            if (0 == killed[i]) {
                pid_ptr = find_get_pid(pid[i]);
                PRINTF("we try to kill kernel thread %d ,thread id is %d !\n", i,pid[i]);
                if (!kill_pid(pid_ptr, SIGTERM, 1)) {
                    PRINTF("Unable to kill thread. Waiting for completion !\n");
                    wake_up(&wait);
                    wait_for_completion(&evt_dead);
                }
                PRINTF("~~~~~kernel thread %d ,thread id  %d was killed !~~~~~\n", i,pid[i]);
                killed[i] = 1;
                ++pid_index;
            }
        }
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

static void mod_thread ( int i ) 
{
    int dfe_local = 0;
    unsigned int th_id = (unsigned int )i; 
    int seconds = (th_id + 1) * 1000; 
    char dfe_str[14];
	int retval;

    unsigned int new_core_id = raw_smp_processor_id();
    unsigned int old_core_id = new_core_id;

    unsigned int dfe_cpu = th_id / 2;
    unsigned long mask = 1UL << dfe_cpu;
	cpumask_t cpus_allowed, new_mask;
    struct cpumask dfe_mask;
    struct task_struct *tsk;

    sprintf(dfe_str,"dfe_%d_alive",(int *)i);

    daemonize(dfe_str);

	/* Request delivery of SIGKILL */
	allow_signal(SIGKILL);

    /* affinity begin */
    cpumask_clear(&dfe_mask);
    cpumask_set_cpu(dfe_cpu,&dfe_mask);

    tsk = current;
    get_task_struct(tsk);
    PRINTF ("kthread %d, before set cpu affinity:cpus_allowed = %ld\n",th_id, tsk->cpus_allowed);
    cpumask_and(&new_mask, &dfe_mask, cpu_possible_mask);
    PRINTF ("kthread %d ,new_mask =  %ld .\n",th_id, new_mask);
    cpumask_copy(&tsk->cpus_allowed,&new_mask);
    PRINTF ("kthread %d, after set cpu affinity:cpus_allowed = %ld\n",th_id, tsk->cpus_allowed);
    /*put_task_struct(tsk);*/

    while (1) {
        mid_stack1();
        /* Waits 1 sec */
        sleep_on_timeout(&wait,seconds);

        /*-----------------------------------------------------------------------------
         *  add thread transfer code
         *  first ,update new_core_id
         *-----------------------------------------------------------------------------*/
        new_core_id = raw_smp_processor_id();
        if (new_core_id != old_core_id) {
            PRINTF ("kthread %ld (cpus_allowed = %ld) is transfered from core %d to core %d.\n", th_id,tsk->cpus_allowed,old_core_id, new_core_id);
        }
        /*  update old_core_id to new one */
        old_core_id = new_core_id;

        PRINTF("kthread %d  woke up on core %d . thread id is %d !\n", th_id,raw_smp_processor_id(),pid[th_id]); 
        dfe_local++;
        atomic_add(2,&dfe_count);

        if (dfe_gone) {
            PRINTF("dfe_local = %d; dfe_count = %d; kthread exiting!\n", dfe_local,atomic_read(&dfe_count));
            complete_and_exit(&evt_dead,0);
        }
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

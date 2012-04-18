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
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>

/* defines */

#define DRIVER_AUTHOR "Wind River Workbench Engineers"
#define DRIVER_DESC   "Module Debug Sample"

#define PRINTF(args...) printk(KERN_ALERT "" args);

/* globals */

static struct workqueue_struct *dfe_wq;

typedef struct {
    struct work_struct dfe_work;
    char *globalData;
    long dfe_count;
    float globalFloat;
} dfe_work_t;
dfe_work_t *dfe_kernel1,*dfe_kernel2;

/* forward declarations */

static int  wb_dfe_init(void);
static void wb_dfe_exit(void);

static void dfe_wq_func(struct work_struct *work);

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
    int ret;
    unsigned int nr_online_cpus = num_online_cpus();

    PRINTF("Welcome to %s !\n", DRIVER_DESC);
    PRINTF("number of online cpus = %d \n", nr_online_cpus);

    dfe_wq = create_workqueue("dfe_wq");
    if ( ! dfe_wq) {
        PRINTF("dfe_wq create FAILED");
        return -1;
    }

    dfe_kernel1 = (dfe_work_t *)kmalloc(sizeof(dfe_work_t),GFP_KERNEL) ;
    if (dfe_kernel1) {
        INIT_WORK( (struct work_struct *)dfe_kernel1,dfe_wq_func);
        dfe_kernel1->globalData = "GLOBAL";
        dfe_kernel1->dfe_count = 0;
        dfe_kernel1->globalFloat = 1.13;
        ret = queue_work(dfe_wq,(struct work_struct*)dfe_kernel1);
    }

    dfe_kernel2 = (dfe_work_t *)kmalloc(sizeof(dfe_work_t),GFP_KERNEL) ;
    if (dfe_kernel2) {
        INIT_WORK( (struct work_struct *)dfe_kernel2,dfe_wq_func);
        dfe_kernel2->globalData = "GLOBAL";
        dfe_kernel2->dfe_count = 0;
        dfe_kernel2->globalFloat = 1.13;
        ret = queue_work(dfe_wq,(struct work_struct *)dfe_kernel2);
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
    kfree((void *) dfe_kernel1);
    kfree((void *) dfe_kernel2);
    flush_workqueue(dfe_wq);
    destroy_workqueue(dfe_wq);

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

static void dfe_wq_func( struct work_struct *work ) 
{
    dfe_work_t *dfe_work = (dfe_work_t *)work;
    int dfe_local = 0;

    mid_stack1();
    dfe_local++;
    dfe_work->dfe_count += 2;
    PRINTF("dfe_local = %d; dfe_work->dfe_count = %d; dfe work exiting!\n", dfe_local,dfe_work->dfe_count);
}

/* module declarations */

module_init(wb_dfe_init);
module_exit(wb_dfe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

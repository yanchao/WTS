/*
 * kgdbTest.c - kgdb test sample module
 *
 * Copyright (c) 2010 Wind River Systems, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

/* includes */

#ifdef MODVERSIONS
#include <config/modversions.h>
#endif				

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/major.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <linux/bitops.h>
#include <linux/spinlock.h>
//#include <asm/semaphore.h>
#include <linux/semaphore.h>
#include <linux/init.h>

#ifdef kill_proc

// for kernel version < 2.6.27

typedef int compat_pid;
#defined compat_kill_pid(pid, sig, flag) kill_proc(pid, sig, flag)

#else

typedef struct pid * compat_pid;
#define compat_kill_pid(pid, sig, flag) kill_pid(pid, sig, flag)

#endif

//extern int compat_kill_pid(pid_t, int, int);

/* defines */

#define DEBUG_IT
#ifdef DEBUG_IT
#define PRINTK(args...) printk (KERN_ALERT args)
#else
#define PRINTK(args...)
#endif /* DEBUG_IT */

/* globals */

#ifdef	MODULE
MODULE_AUTHOR ("Wind River");
MODULE_DESCRIPTION ("Sample Test");

static int __init kgdbTest_init (void);
static void __exit kgdbTest_cleanup_module (void);

module_init (kgdbTest_init);
module_exit (kgdbTest_cleanup_module);
#endif	/* MODULE */

#ifdef	MODULE_LICENSE
MODULE_LICENSE ("Dual BSD/GPL");
#endif	/* MODULE_LICENSE */

wait_queue_head_t	waitQueue;

DECLARE_COMPLETION (evtDead);

/* typedefs */

typedef int (*KTHREAD_FUNCPTR) (void * arg);

/* locals */

int			terminating = 0;
static compat_pid	thread_pid;

/******************************************************************************
*
* kgdbTest_func4 -
*/

volatile int kgdbTest_func4 (void)
    {
    return 100;
    }

/******************************************************************************
*
* kgdbTest_func3 -
*/

int kgdbTest_func3 (int j)
    {
    volatile int    i;
    static int      k = 0;
    
    i = kgdbTest_func4 ();
    i += (5 * (i * 200) - 33);
    k++;
    return i;
    }

/******************************************************************************
*
* kgdbTest_func2 -
*/

void kgdbTest_func2 (void)
    {
    int	dummy1 = 1;
    int	dummy2;
    int	dummy3;

    dummy1 = kgdbTest_func3 (2) / 2;
    dummy2 = kgdbTest_func3 (3) / 2;
    dummy3 = kgdbTest_func3 (4) / 2;
    dummy1 += dummy2;
    dummy2 += dummy1;
    dummy3 = dummy1 + dummy2;
    PRINTK ("dummy1 %d dummy2 %d dummy3 %d\n", dummy1, dummy2, dummy3);
    }

/******************************************************************************
*
* kgdbTest_func1 -
*/

void kgdbTest_func1 (char * pA)
    {
    PRINTK ("%s", pA);
    kgdbTest_func2 ();
    }

/******************************************************************************
*
* kgdbTest_thread -
*/

static void kgdbTest_thread (void)
    {
    for (;;)
    	{
	sleep_on_timeout (&waitQueue,2000); /* 2 secs */
	kgdbTest_func1 ("kgdbTest's alive\n");

	if (terminating)
	    {
	    PRINTK ("thread exiting ...\n");
	    complete_and_exit (&evtDead, 0);
	    }
	}
    }

/******************************************************************************
*
* kgdbTest_init -
*/

static int __init kgdbTest_init (void)
    {
    init_waitqueue_head (&waitQueue);

    thread_pid = (compat_pid) kernel_thread ((KTHREAD_FUNCPTR) kgdbTest_thread,
			    	(void *) 0,
			    	CLONE_SIGHAND);

    PRINTK ("%s done (pid %d created)\n", __FUNCTION__, (int) thread_pid);
    return 0;
    }
    
/******************************************************************************
*
* kgdbTest_cleanup_module -
*/

static void __exit kgdbTest_cleanup_module (void)
    {
    terminating = 1;
    
    if (!compat_kill_pid (thread_pid, SIGTERM, 1))
    	{
	PRINTK ("Waiting for completion of thread ...\n");
	wake_up (&waitQueue);	
	wait_for_completion (&evtDead);
	}
	
    PRINTK ("%s done !\n", __FUNCTION__);
    }

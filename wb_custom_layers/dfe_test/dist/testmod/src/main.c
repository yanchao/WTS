/* main.c: for "testmod" sample kernel module */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,10feb06,Alex deVries  authored
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "timer.h"
#include "settings.h"

unsigned int testmod_timeout;
module_param_named(timeout, testmod_timeout, int, 0);



/*
 * int testmod_init()
 * This is the function called when the module is loaded.
 */

static int __init
testmod_init(void)
{
	printk(KERN_INFO "Loading testmod\n");
	printk(KERN_INFO "timeout is %u\n",testmod_timeout);
	create_sysctl_files(); 
	create_timers();
	return 0;
}

/*
 * int testmod_exit()
 * This is the function called when the module is unloaded.
 */

static void __exit
testmod_exit(void)
{
	printk(KERN_INFO "Unloaded testmod\n");
	destroy_sysctl_files();
	destroy_timers();

}

module_init(testmod_init);
module_exit(testmod_exit);

MODULE_DESCRIPTION("Wind River test module");
MODULE_LICENSE("GPL");

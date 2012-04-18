#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/sysctl.h>
#include <linux/device.h>
#include <linux/proc_fs.h>

#include "timer.h"
#include "testmod.h"

static struct ctl_table_header * testmod_sysctl_header;
static int testmod_sysctl_handler(ctl_table *ctl, int write, 
	void __user *buffer, size_t *lenp, loff_t *ppos);

/*
 * testmod_table_entries[] -
 * This table connects the module's callback handler
 */

static ctl_table testmod_table_entries[] = {
	{
		.procname = "iloveottawa",
		.data = &testmod_timeout,
		.maxlen = sizeof(unsigned int),
		.mode = 0600, 
		.proc_handler = &testmod_sysctl_handler,
	},
	{
		.procname = "timeout",
		.data = &testmod_timeout,
		.maxlen = sizeof(unsigned int),
		.mode = 0644, 
		.proc_handler = &testmod_sysctl_handler,
	},
	{
		.procname = NULL
	}
};

/*
 * testmod_table[] - 
 * This table connects the module's sysctl to the handler list
 */

static ctl_table testmod_table[] = {
	{
		.procname = "testmod",
		.maxlen = 0,
		.mode = 0555,
		.child = testmod_table_entries,
	},
	{
		.procname = NULL
	}
};

/*
 * testmod_root_table[] -
 * This table is the module's root sysctl table
 */

static ctl_table testmod_root_table[] = {
	{
		.procname = "dev",
		.maxlen = 0,
		.mode = 0555,
		.child = testmod_table,
	},
	{
		.procname = NULL
	}
};

/*
 * int testmod_sysctl_handler() -
 * This is the sysctl call back handler
 */

static int testmod_sysctl_handler(ctl_table *ctl, int write, 
	void __user *buffer, size_t *lenp, loff_t *ppos)
{

	int ret = 0;

	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	printk(KERN_INFO "Updating\n");

	update_timer();

	return ret;

}

/*
 * void create_sysctl_files() -
 * This registers this module with sysctl
 */

void create_sysctl_files(void)
{
	printk(KERN_INFO "Registering\n");

	testmod_sysctl_header = register_sysctl_table(testmod_root_table);

}


/*
 * void destroy_sysctl_files() -
 * This unregisters this module from sysctl
 */

void destroy_sysctl_files(void) {
	unregister_sysctl_table(testmod_sysctl_header);
}

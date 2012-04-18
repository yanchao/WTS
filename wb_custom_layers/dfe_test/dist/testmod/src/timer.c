#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include "testmod.h"

static struct timer_list testmod_timer_struct;
static unsigned int count = 0;

#define future_in_ms(n) (jiffies+(testmod_timeout * HZ ) / 1000)

/* 
 * testmod_timer() - This is the timer callback.
 */

static void testmod_timer(unsigned long dev_addr)
{
	count++;
	printk(KERN_INFO "Timeout #%u!\n",count);
	testmod_timer_struct.expires = future_in_ms(testmod_timeout);
	add_timer(&testmod_timer_struct);
}

/*
 * create_timers(void) - This creates the programmable timer 
 */

void create_timers(void) {

	printk(KERN_INFO "Setting up timers\n");

	/* Initialize the timer */
	init_timer(&testmod_timer_struct);

	/* Set up the timer callback */
	testmod_timer_struct.function = &testmod_timer;

	/* Set up the timeout */
	testmod_timer_struct.expires = future_in_ms(testmod_timeout);

	/* Add the timer */
	add_timer(&testmod_timer_struct);
}

/* 
 * update_timer(void) - this updates the timer with a new timeout value 
 *         stored in testmod_timeout.
 */

void update_timer(void) {
	testmod_timer_struct.expires = future_in_ms(testmod_timeout);
}

/*
 * destroy_timers() - frees up and cancels existing timers.
 */

void destroy_timers(void) {

	printk(KERN_INFO "Destroying timers\n");
	del_timer(&testmod_timer_struct);
}

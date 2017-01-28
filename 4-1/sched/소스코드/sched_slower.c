#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/sched.h>

int slower_pid;

asmlinkage int sys_sched_slower(int input_pid) {

	slower_pid = input_pid;

	printk("%d was collected in Sched_slower\n",slower_pid);

	return slower_pid;
}

EXPORT_SYMBOL(slower_pid);

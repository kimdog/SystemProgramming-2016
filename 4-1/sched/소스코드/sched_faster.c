#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/sched.h>

int faster_pid;

asmlinkage int sys_sched_faster(int input_pid) {

	faster_pid = input_pid;

	printk("%d was collected in Sched_faster\n",faster_pid);

	return faster_pid;
}

EXPORT_SYMBOL(faster_pid);

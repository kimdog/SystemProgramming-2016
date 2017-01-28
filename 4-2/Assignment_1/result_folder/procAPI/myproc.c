//#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/init.h>
#include "/home/osta/SP_01/QUEUE/myqueue_usr.h"
#include <linux/proc_fs.h>

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"

extern MY_KIM_QUEUE* kim_myQueue;
extern bool my_enqueue(MY_KIM_QUEUE* queue, char* fs_name,
        unsigned long long sec_num, struct timespec cur_time);
extern void my_print_queue(MY_KIM_QUEUE* queue);

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Simple module Open!!\n");

    return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer,
        size_t count, loff_t *ppos)
{
    printk(KERN_INFO "Simple module Write!!\n");
    if(kim_myQueue != NULL) {
        my_print_queue(kim_myQueue);
    }

    return count;
}

static const struct file_operations myproc_fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .write = my_write,
};

static int __init simple_init(void)
{
    printk(KERN_INFO "Simple Module init!!\n");

    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);
   
    return 0;
}

static void __exit simple_exit(void)
{
    printk(KERN_INFO "Simple Module Exit!!\n");

    return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("KOREA University");
MODULE_DESCRIPTION("It's simple!!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");


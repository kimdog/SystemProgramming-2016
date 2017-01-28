 #include "/home/osta/SP_01/QUEUE/myqueue_usr.h"


extern MY_KIM_QUEUE* kim_myQueue;
extern MY_KIM_QUEUE* my_createQueue(void);
extern bool my_enqueue(MY_KIM_QUEUE* queue, char* fs_name, unsigned long long sec_num,
        struct timespec cur_time);
extern void my_print_queue(MY_KIM_QUEUE* queue);
extern MY_KIM_QUEUE* my_destroyQueue(MY_KIM_QUEUE* queue);
extern bool is_myQueue_exist;


static int __init simple_init(void)
{
    printk(KERN_INFO "My QUEUE Module init!!\n");
 
    kim_myQueue = my_createQueue();
    printk("success create kim_myQueue\n");
    is_myQueue_exist = true;
   
    return 0;
}

static void __exit simple_exit(void)
{
    printk(KERN_INFO "My QUEUE Module Exit!!\n");
    if(kim_myQueue) {
        is_myQueue_exist = false;
        my_destroyQueue(kim_myQueue);
        printk("Destroy kim_myQueue\n");
    }

    return;
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("KOREA University");
MODULE_DESCRIPTION("It's my QUEUE!!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");


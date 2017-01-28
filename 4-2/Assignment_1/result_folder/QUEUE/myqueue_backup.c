#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>


typedef struct my_kim_queue_node {
    char fs_name[20];   // file system name
    int sec_num;  // sector number
    char cur_time[20];  // current time

    struct my_kim_queue_node* next;
} MY_KIM_QUEUE_NODE;

typedef struct {
    MY_KIM_QUEUE_NODE* front;
    MY_KIM_QUEUE_NODE* rear;
    int count;
} MY_KIM_QUEUE;

MY_KIM_QUEUE* kim_myQueue;

MY_KIM_QUEUE* my_createQueue(void) {
    MY_KIM_QUEUE* queue;

    queue = (MY_KIM_QUEUE*)vmalloc(sizeof(MY_KIM_QUEUE));
    if(queue) {
        queue->front    =NULL;
        queue->rear     =NULL;
        queue->count    =0;
    }

    return queue;
}

bool my_enqueue (MY_KIM_QUEUE* queue, char* fs_name, int sec_num, char* cur_time) {
    if(queue->count < 30000) {
    MY_KIM_QUEUE_NODE* newPtr;

    if(!(newPtr = (MY_KIM_QUEUE_NODE*)vmalloc(sizeof(MY_KIM_QUEUE_NODE))))
        return false;

    strcpy(newPtr->fs_name, fs_name);
    newPtr->sec_num = sec_num;
    strcpy(newPtr->cur_time, cur_time);
    newPtr->next    = NULL;

    if(queue->count == 0)
        queue->front = newPtr;
    else
        queue->rear->next = newPtr;

    (queue->count)++;
    queue->rear = newPtr;
    return true;
    }
    else
        return false;
}
EXPORT_SYMBOL(my_enqueue);

void my_print_queue(MY_KIM_QUEUE* queue) {
    MY_KIM_QUEUE_NODE* newPtr;
    newPtr = queue->front;
    while(newPtr != NULL) {
        printk("%s\t%d\t%s", newPtr->fs_name, 
                newPtr->sec_num, newPtr->cur_time);
        // cur_time has '\n'
        newPtr = newPtr->next;
    }
}
EXPORT_SYMBOL(my_print_queue);

MY_KIM_QUEUE* my_destroyQueue(MY_KIM_QUEUE* queue) {
    MY_KIM_QUEUE_NODE* deletePtr;

    if(queue) {
        while(queue->front != NULL) {
         
            deletePtr = queue->front;
            queue->front = queue->front->next;
            vfree(deletePtr);
        }
        vfree(queue);
    }
    return NULL;
}

static int __init simple_init(void)
{
    printk(KERN_INFO "My QUEUE Module init!!\n");
  
    kim_myQueue = my_createQueue();
    printk("success create kim_myQueue\n");
    return 0;
}

static void __exit simple_exit(void)
{
    printk(KERN_INFO "My QUEUE Module Exit!!\n");
    my_destroyQueue(kim_myQueue);
    printk("Destroy kim_myQueue\n");

    return;
}

module_init(simple_init);
module_exit(simple_exit);

EXPORT_SYMBOL(kim_myQueue);

MODULE_AUTHOR("KOREA University");
MODULE_DESCRIPTION("It's my QUEUE!!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");


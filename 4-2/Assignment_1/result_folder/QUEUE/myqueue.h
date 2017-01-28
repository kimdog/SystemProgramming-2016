#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/time.h>

typedef struct my_kim_queue_node {
    char fs_name[20];   // file system name
    unsigned long long sec_num;  // sector number
    struct timespec cur_time;  // current time

    struct my_kim_queue_node* next;
} MY_KIM_QUEUE_NODE;

typedef struct {
    MY_KIM_QUEUE_NODE* front;
    MY_KIM_QUEUE_NODE* rear;
    int count;
} MY_KIM_QUEUE;

MY_KIM_QUEUE* kim_myQueue = NULL;
bool is_myQueue_exist = false;

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
EXPORT_SYMBOL(my_createQueue);

bool my_enqueue (MY_KIM_QUEUE* queue, char* fs_name, unsigned long long sec_num, struct timespec cur_time) {
    if(queue->count < 50000) {
        MY_KIM_QUEUE_NODE* newPtr;

        if(!(newPtr = (MY_KIM_QUEUE_NODE*)vmalloc(sizeof(MY_KIM_QUEUE_NODE))))
            return false;

        strcpy(newPtr->fs_name, fs_name);

        newPtr->sec_num = sec_num;
        newPtr->cur_time = cur_time;
        newPtr->next    = NULL;

        if(queue->count == 0)
             queue->front = newPtr;
        else
             queue->rear->next = newPtr;

         (queue->count)++;
          queue->rear = newPtr;
 
          return true;
    }
    
    else {
        // if queue->rear->next == NULL, connect front to rear
        if(!queue->rear->next)
            queue->rear->next = queue->front;

        queue->rear = queue->rear->next;
        // if more than queue, push back front
        if((queue->rear) == (queue->front))
            queue->front = queue->front->next;

        strcpy(queue->rear->fs_name, fs_name);
        queue->rear->sec_num = sec_num;
        queue->rear->cur_time = cur_time;

        return true;
    }
}
EXPORT_SYMBOL(my_enqueue);

void my_print_queue(MY_KIM_QUEUE* queue) {
   
    struct tm result;

    while((queue->front)) {
        // if null, break;

        time_to_tm(queue->front->cur_time.tv_sec, 0, &result);

        printk("%s\t%Lu\t %02d:%02d:%02d.%ld\n", queue->front->fs_name, 
                queue->front->sec_num, result.tm_hour, result.tm_min, 
                result.tm_sec, (queue->front->cur_time.tv_nsec)/1000);
        
        // reset current Node
        strcpy(queue->front->fs_name,"");
        queue->front->sec_num = 0;
        queue->front->cur_time.tv_sec = 0;
        queue->front->cur_time.tv_nsec = 0;

        // if printed rear, break
        if((queue->front) == (queue->rear))
            break;

        // else next step.
        queue->front = queue->front->next;
    }
}
EXPORT_SYMBOL(my_print_queue);

MY_KIM_QUEUE* my_destroyQueue(MY_KIM_QUEUE* queue) {
    MY_KIM_QUEUE_NODE* deletePtr;

    if(queue) {
        while(queue->count != 0) {
         
            deletePtr = queue->front;
            queue->front = queue->front->next;
            vfree(deletePtr);

            queue->count--;
        }
        vfree(queue);
        queue = NULL;
    }
    return NULL;
}
EXPORT_SYMBOL(my_destroyQueue);

EXPORT_SYMBOL(kim_myQueue);
EXPORT_SYMBOL(is_myQueue_exist);

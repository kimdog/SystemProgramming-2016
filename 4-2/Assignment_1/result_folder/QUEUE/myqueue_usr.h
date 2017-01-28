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


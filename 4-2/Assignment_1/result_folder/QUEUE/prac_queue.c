#include <stdio.h>
#include <linux/kernel.h>
#include </usr/src/linux-4.4.1/include/linux/module.h>

extern myQueue;
extern my_print_queue;
extern my_enqueue;

int main() {

    if(my_enqueue(1)) {
        my_print_queue;
    }

    return 0;
}

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

#include <linux/ip.h>	// ip header
#include <linux/tcp.h>	// tcp header

#define PROC_DIRNAME "myproc"
#define PROC_FILENAME "myproc"

#define PORT1 1111
#define PORT2 2222
#define PORT3 3333
#define PORT4 4444
#define PORT5 5555

unsigned int packet_limit;
unsigned int sport;	// source port
unsigned int dport;	// destination port

unsigned int server_port[5];	// each server port.
unsigned int data_len[5];	// data lenth for each port


static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static unsigned int my_hook_fn(void *priv,
	struct sk_buff *skb, const struct nf_hook_state *state) {


    struct iphdr  *ih; // ip header
    struct tcphdr *th; // tcp header

    if(!skb)
	return NF_DROP;

    ih = ip_hdr(skb);

    if(!ih)
	return NF_DROP;

    // if TCP, go on
    if(ih->protocol == IPPROTO_TCP) {

	int i;

	th = tcp_hdr(skb);
	// get source port and destination port
	sport = htons((unsigned short int) th->source);
	dport = htons((unsigned short int) th->dest);

	for(i=0; i<5; i++) {
	    if(sport == server_port[i]) {
		// print previous information.
		printk(KERN_INFO "##SPORT : %u, ##Length : %u\n",
			server_port[i], data_len[i]);

		// DROP packet, if over limit
		if(data_len[i] > packet_limit)
		    return NF_DROP;

		// skb->len = skb->tail - skb->data
		data_len[i] += skb->len;
	    }
	}
    }

    return NF_ACCEPT;
    /*
    // packet drop

    printk(KERN_INFO "packet dropped\n");
    return NF_DROP;	// drop the packet
    */
}

static struct nf_hook_ops my_nf_ops = {
    .hook = my_hook_fn,	// call my hook function
    .hooknum = NF_INET_PRE_ROUTING, // also, 'NF_INET_LOCAL_IN' is ok.
    .pf = PF_INET,	// IPv4
    .priority = NF_IP_PRI_FIRST, // set highest priority
};


static ssize_t my_write(struct file *file, const char __user *user_buffer,
	size_t count, loff_t *ppos) {

    printk(KERN_INFO "SImple module write!\n");

    // update packet_limit by writing.
    packet_limit = simple_strtol(user_buffer, NULL, 10);

    printk(KERN_INFO "##packet limit : %u\n", packet_limit);

    return count;
}

static const struct file_operations myproc_fops = {
    .owner = THIS_MODULE,
    .write = my_write,
};

static int __init simple_init(void) {

    // initiate port and each port data lenght.
    int i;
    for (i = 0; i < 5; i++) {
	data_len[i] = 0;
    }
    server_port[0] = PORT1;
    server_port[1] = PORT2;
    server_port[2] = PORT3;
    server_port[3] = PORT4;
    server_port[4] = PORT5;

    printk(KERN_INFO "SIMple module init!\n");

    // initiate packet_limit.
    packet_limit = 10000;
    printk(KERN_INFO "##packet limit = %u\n", packet_limit); 

    // make proc dir/file
    proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
    proc_file = proc_create(PROC_FILENAME, 0600, proc_dir, &myproc_fops);
    printk(KERN_INFO "##Proc file make success\n");

    // register hook function
    nf_register_hook(&my_nf_ops);
    printk(KERN_INFO "##nf_hook registered\n");

    return 0;
}

static void __exit simple_exit(void) {
    printk(KERN_INFO "simple module exit!\n");
    remove_proc_entry(PROC_FILENAME, proc_dir); // remove file
    remove_proc_entry(PROC_DIRNAME, NULL);	// remove directory
    nf_unregister_hook(&my_nf_ops);
}
module_init(simple_init);
module_exit(simple_exit);

MODULE_AUTHOR("KOREA UNIV.");
MODULE_DESCRIPTION("It's simple!");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");

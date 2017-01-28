#include <linux/kernel.h>
#include <pthread.h>    // use thread
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h>	// connect ... functions for socket 
#include <sys/types.h>	// typedef
#include <arpa/inet.h>	// inet_addr()
#include <time.h>
#include <sys/time.h>

#define BUFFER_LEN 65536
#define BUFF_SIZE 65536
#define SERV_IP_ADDR "192.168.56.103" // SERVER IP address
#define PORT1 1111	// each SERVER PORTS
#define PORT2 2222
#define PORT3 3333
#define PORT4 4444
#define PORT5 5555
#define NUM_PORTS 5	// number of ports 

void *f_socket(void*);	// each socket thread function

int main() {

  pthread_t sock_th[NUM_PORTS]; // each socket thread ID
//void *status;

  int err;		// check error
  int port[NUM_PORTS];	// port number
  int i;

  port[0] = PORT1;
  port[1] = PORT2;
  port[2] = PORT3;
  port[3] = PORT4;
  port[4] = PORT5;
  

  // start thread, by function f_socket with throw value port number
  for(i=0; i<NUM_PORTS; i++) {
      err = pthread_create(&sock_th[i], NULL, f_socket, (void*)&port[i]);

      // error check
      if (err != 0) {
	  printf("port: %d thread create error\n", port[i]);
      }
  }


  // join each thread.
  // : main function will not finished before jointed thread finished
  for(i=0; i<NUM_PORTS; i++) {
      // return value is NULL, it can be &status.
      err = pthread_join(sock_th[i], NULL);

      if (err != 0) {
	  printf("port: %d thread join error\n", port[i]);
      }
  }

  printf("main function end\n");

  return 0;
}


void *f_socket(void* arg) {
    int port = *((int*) arg);	     // receive server port number
    printf("port :: %d\n", port);
//  pthread_t id = pthread_self();
//  printf("thread ID :: %lu\n", id);


    // declair socket data structure

    int client_socket;		     // client socket id
    struct sockaddr_in server_addr;  // server address
    char buff[BUFF_SIZE+5];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // if fail, return -1
    if(client_socket == -1)
    {
	printf("socket create FAIL\n");
	pthread_exit(NULL);
    }

    // declair connect file descriptor

    // memset - reset server address 0
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;	// IPv4
    server_addr.sin_port = htons(port);	// SERVER PORT
    server_addr.sin_addr.s_addr = inet_addr(SERV_IP_ADDR); // SERVER IP


    // connect to SERVER with client socket
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
	printf("connect FAIL\n");
	pthread_exit(NULL);
    }


    // create output file
    char outputfile[256];
    char portnum[6];

    // outputfile = "./port.txt"
    strcpy(outputfile, "./");
    sprintf(portnum, "%d", port);
    strcat(outputfile, portnum);
    strcat(outputfile, ".txt");

    // open file
    FILE* fp;
    fp = fopen(outputfile, "wt+");

    // declare time structure
    struct timeval tv;
    struct timezone tz;
    struct tm *tm;

    // infinite LOOP !!
    while(1) {
	// read Message from client_socket
        char buffer[BUFFER_LEN];
        int read_size = read(client_socket, buffer, BUFFER_LEN);
	buffer[read_size] = '\0';

	// get current time
	gettimeofday(&tv, &tz);
	tm = localtime(&tv.tv_sec);

	// print "h:m:s.ms msgLen contents"
        fprintf(fp, "%d:%02d:%02d.%ld  %d  %s\n", 
		tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec,
		read_size, buffer);
      
	// show finish one LOOP
	//printf("port::%d::read\n", port);
	
	// wait 1 second
	sleep(1);

    }

    fclose(fp);
    close(client_socket);

    return NULL;
}

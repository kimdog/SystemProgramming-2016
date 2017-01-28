#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef int BOOL;

#define TRUE 1
#define FALSE 0

// information form
typedef struct node {

	struct node* next;	// next Process in the List
	char name[30];		// name of process
	int pid;		// pid
	int vmSize;		// size of Virtual Memory	 : Page * 4kB
	int vmRss;		// resident set size		 : Page * 4kB
	int vmHwm;		// Working Set Size (= Peak RSS) : Page * 4kB
	int shared;		// shared memory size 		 : number of Pages
	int checkWSS;		// Check 1 second '*' if WSS is changed	  
} Process;

// list of information
typedef struct {
	int count;		// number of Processes.
	Process *head;		// head process
	Process *rear;		// rear process
	Process *pos;		// current process
} List_process;

// add process on List
int add_process(List_process* list, Process* item) {

	Process* newPtr;

	if(!(newPtr = (Process*)malloc(sizeof(Process))))
		return 0;

	newPtr->next   = NULL;
	strcpy(newPtr->name,item->name);
	newPtr->pid    = item->pid;
	newPtr->vmSize = item->vmSize;
	newPtr->vmRss  = item->vmRss;
	newPtr->vmHwm  = item->vmHwm;
	newPtr->shared = item->shared;
	newPtr->checkWSS = 0;

	if (list->count == 0)
		list->head = newPtr;
	else
		list->rear->next = newPtr;

	(list->count)++;
	list->rear = newPtr;
	
	return 1;
}

int main() {

	//==========================================================
	// find max PID
	//==========================================================

	int max_PID;		// max_PID
	FILE *file_max;		// max PID file

	file_max = fopen("/proc/sys/kernel/pid_max","rb");

	char buf_max[256];
	fgets(buf_max,sizeof(buf_max),file_max);
	max_PID = atoi(buf_max);		// change type string to int

	printf("#### max PID : %d\n",max_PID);

	//==========================================================
	// for 0 to max PID
	// if file of index does not exist, pass
	// else get information of file
	// PID, name, vmSize, vmRSS, vmHWM, shared
	//==========================================================

	//==============================================================
	// create process list
	List_process* list = (List_process*)malloc(sizeof(List_process));	

	if(list)
	{
		list->count	= 0;
		list->rear	= NULL;
		list->head	= NULL;
		list->pos	= NULL;
	}
	//==============================================================

	int k=0;		// how many times to Update. time = k*0.5 seconds
	while(1) {

	if(k==150) break;	// if 150 * 0.5 seconds, stop!
//	if(k==3) break;
	printf("\nPID\tName\t\t\tVirtual\tPhysical shared\tWSS\n");

//	BOOL isUpdate = FALSE;			// is vmHWM updated?
	int total_num = 0;			// how many program
	int i;					//index of for Loop
	for(i=0;i<max_PID;i++) {
	
		char filename[30]; 	// file name of each file
		FILE *f;		// each file
	
		sprintf(filename,"/proc/%d/status",i); 	// set file name	
		f = fopen(filename,"rb");		// open file
//		f = fopen("/proc/1/status","rb");
		if(f==NULL)
		{
			// nothing to happen
		}
		else
		{
			//=====================================================
			// take pre_process(last 0.5 seconds) information
			//=====================================================
			Process* pre_process = (Process*)malloc(sizeof(Process));
			
			// initialize 	
			strcpy(pre_process->name,"");
			pre_process->pid      = -1;
			pre_process->vmSize   = -1;
			pre_process->vmRss    = -1;
			pre_process->vmHwm    = -1;
			pre_process->shared   = -1;
			pre_process->checkWSS =  0;

			// if list have items, check which is previous information
			BOOL isExist = FALSE;		// is index exist in list?
			if((list->count)!=0) {
				list->pos = list->head;	// set position = head.
				int j;
				for(j=0;j<(list->count);j++) {
					// if find i == pid in list take information
					if((list->pos->pid)==i) {
						strcpy(pre_process->name,list->pos->name);
						pre_process->pid    = list->pos->pid;
						pre_process->vmSize = list->pos->vmSize;
						pre_process->vmRss  = list->pos->vmRss;
						pre_process->vmHwm  = list->pos->vmHwm;
						pre_process->shared = list->pos->shared;
						pre_process->checkWSS = list->pos->checkWSS;
						isExist = TRUE;
					}
					// next step.
					if(isExist) break; // stop for load current position
					list->pos = list->pos->next;
				}
			}
			// end take previous information
			//=========================================================


			// create form of information
			Process* process = (Process*) malloc(sizeof(Process));

			char buf[256];
			char *tok = NULL;	

			while(fgets(buf,sizeof(buf),f) != NULL) {
				
				tok = strtok(buf,":| |\t|\n");
				while(tok != NULL)
				{
					// is it Name information?
					if(strcmp(tok,"Name")==0) {
						// get name
						tok = strtok(NULL,":| |\t|\n");
						strcpy(process->name,tok);
						tok = strtok(NULL,":| |\t|\n");
					}

					// is it PID information?
					else if(strcmp(tok,"Pid")==0) {
						// get converted int pid
						tok = strtok(NULL,":| |\t|\n");
						process->pid = atoi(tok); 
						tok = strtok(NULL,":| |\t|\n");
					}

					// is it vmSize information?
					else if(strcmp(tok,"VmSize")==0) {
						// get converted int vmSize
						tok = strtok(NULL,":| |\t|\n");
						process->vmSize = atoi(tok); 
						tok = strtok(NULL,":| |\t|\n");
					}

					// is it vmRss information?
					else if(strcmp(tok,"VmRSS")==0) {
						// get converted int vmRss
						tok = strtok(NULL,":| |\t|\n");
						process->vmRss = atoi(tok); 
						tok = strtok(NULL,":| |\t|\n");
					}

					// is it vmHWM information?
					else if(strcmp(tok,"VmHWM")==0) {
						// get converted int vmHwm
						tok = strtok(NULL,":| |\t|\n");
						process->vmHwm = atoi(tok); 
						tok = strtok(NULL,":| |\t|\n");
					}


					// just pass to next token
					else {
						tok = strtok(NULL,":| |\t|\n");
					}
				}
			//	printf("%s", buf);
			}

			fclose(f);	// close status file

			// ====================================================
			// file of index i already checked exist
			// so not check, just open statm for get shared memory
			// ====================================================


			sprintf(filename,"/proc/%d/statm",i); 	// set file name	
			f = fopen(filename,"rb");		// open statm file
		
			fgets(buf,sizeof(buf),f);		// get one line
			tok = strtok(buf,":| |\t|\n");		// pass pages of vmSize
			tok = strtok(NULL,":| |\t|\n");		// pass pages of vmRSS
			tok = strtok(NULL,":| |\t|\n");		// get shared memory page
			process->shared = atoi(tok)*4;		// page * 4 = size
			// this value is not total size, just the number of pages.
			// For getting the size of shared memory
			// we should multiply 4kB .

			fclose(f);	// close statm file

			//========================================================
			// if there is no index i information in list, add.
			process->next = NULL;
			process->checkWSS = 0;
			if(!isExist) {
				// add process on list
				add_process(list,process);
				list->pos = list->rear;
			} else {
				//update current pos
				list->pos->vmSize = process->vmSize;
				list->pos->vmRss  = process->vmRss;
				list->pos->vmHwm  = process->vmHwm;
				list->pos->shared = process->shared;
				// count 0.5 sec
				if((pre_process->checkWSS) == 0) {
					list->pos->checkWSS = 0;
				} else {
					list->pos->checkWSS = (pre_process->checkWSS) - 1;
				}
			}

			// ==========================================================
			// if previous vmHWM == current vmHWM : check WSS * 1sec
			// else nothing to happen.

			if((pre_process->vmHwm) == (process->vmHwm)) {
				// Nothing to happen
			}
			else {
				// 0.5 sec *  2 times = 1 sec
				// each loop : -1
				list->pos->checkWSS = 2;
			}

			// ==========================================================
			// NOw , We get current information : process
			// 	and	previous information : pre_process
			// ==========================================================
			// print information
			// i will show size of pages,
			// so i should all data devided by 4.
			// ==========================================================

			int WSS = (list->pos->checkWSS); // if >0 , check '*'
			// if length of name > 7 2 tabs
			if(strlen(process->name)>7) {
				if(WSS == 0) {
				printf("%d\t%s\t\t%d\t%d\t%d\n",process->pid,process->name,(process->vmSize)/4,(process->vmRss)/4,(process->shared)/4);
				} else {
				printf("%d\t%s\t\t%d\t%d\t%d\t*\n",process->pid,process->name,(process->vmSize)/4,(process->vmRss)/4,(process->shared)/4);
				}
			}
			
			// else 3 tabs
			else {
				if(WSS == 0) {
				printf("%d\t%s\t\t\t%d\t%d\t%d\n",process->pid,process->name,(process->vmSize)/4,(process->vmRss)/4,(process->shared)/4);
				} else {
				printf("%d\t%s\t\t\t%d\t%d\t%d\t*\n",process->pid,process->name,(process->vmSize)/4,(process->vmRss)/4,(process->shared)/4);
				}
			}

			
//			free(pre_process);	// free malloc pre_process
//			free(process);		// free malloc process
			total_num++;		// increase number of process
		}

	}

	// print total number of process and Time.
	printf("\n#### Time : %f seconds\n",k*0.5);
	printf("#### Total number of Process : %d\n\n",total_num);
/*
	int l;
	printf("##test ::");
	list->pos = list->head;
	for(l=0;l<(list->count);l++) {
		printf("%d",list->pos->pid);
		list->pos = list->pos->next;
	}
*/
	k++;				// increase 0.5 time
//	not sleep this time, it will be another sleep between get previous and get current

	usleep(500000);			// 0.5 seconds sleep
	}// end time while.

	free(list);	// free malloc list
	return 1;	
}

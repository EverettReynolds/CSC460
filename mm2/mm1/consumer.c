// Everett Reynolds
// MM1 Consumer



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <time.h>
#include <sys/sem.h>
#include <string.h>
#include <math.h>
#include <sys/ipc.h>
#include <stdbool.h>
#include <unistd.h>


#define mutex 0
#define full 1
#define empty 2
#define left shmem[buffSize]
#define right shmem[buffSize+1]

typedef struct jobStruct{
        char id;
        int pid;
        int size;
        int time;
        int state;
}job;

int main(int argc, char *argv[])
{



        FILE *fopen(), *fp; // Initializing Everything under The Sun including Vars, Shared Memory, and Semaphores
	job* shmem;
	job* currJob;
	char* shmchar;
	int* numJob;
	int x;
	int rows = atoi(argv[1]);
	int cols = atoi(argv[2]);
	int buffSize = atoi(argv[3]);
	
	if(rows > 20 || cols > 50 || buffSize > 26)
	{
		printf("Error. Invalid Specifications.");
		return -1;
	}
	int shmid = shmget(IPC_PRIVATE, (buffSize)*sizeof(struct jobStruct),0777);
	if(shmid == -1)
	{
		printf("Shared Memory Error.");
		return 0;
	}
	int semid = semget(IPC_PRIVATE,3,0777);
	if(semid == -1)
	{
		printf("Semaphore Error.");
		return 0;
	}
	
	char* ram;
	int currid;
	int ramid = shmget(IPC_PRIVATE, sizeof(char)*rows*cols,0777); // simulates memory manager structure
	int charid = shmget(IPC_PRIVATE, sizeof(char),0777);
	int shmintid = shmget(IPC_PRIVATE, sizeof(int),0777);
	int posid = shmget(IPC_PRIVATE, (buffSize)*sizeof(struct jobStruct),0777);
	int endid = shmget(IPC_PRIVATE, sizeof(int),0777);
	shmem = (struct jobStruct*) shmat(shmid,NULL,SHM_RND);
	shmchar = (char*) shmat(charid,NULL,SHM_RND);
	numJob = (int*) shmat(shmintid,NULL,SHM_RND);
	currJob = (struct jobStruct*) shmat(posid,NULL,SHM_RND);
	ram = (char*) shmat(ramid,NULL,SHM_RND);
	int * endmark = (int*) shmat(endid,NULL,SHM_RND);
	*numJob = 0;
	*shmchar = 'A';
	left.pid = 0;
	right.pid = 0;
	*endmark = 1;
	currid = 0;
	if((fp = fopen("file","r")) == NULL)
        {
        	printf(":( could not open file to read.\n");
                return(0);
        }

	semctl(semid,mutex,SETVAL,1);
	semctl(semid,full,SETVAL,0);
	semctl(semid,empty,SETVAL,buffSize);
	fprintf(fp,"%d\n", shmid);
	fprintf(fp,"%d\n", charid);
	fprintf(fp,"%d\n", semid);
	fprintf(fp,"%d\n", rows);
 	fprintf(fp,"%d\n", cols);
	fprintf(fp,"%d\n", buffSize);
	fprintf(fp,"%d\n", endid);
	fclose(fp);
	
	for(x = 0; x < 2; x++)
	{
		if(fork() != 0) // asked to figure out if more than one process is needed 
		{
			break;
		}
		currid++;
	}	


	for( x<0; x< rows*cols; x++)
	{
		ram[x] = '.'; // set all slots to empty to start with
	}

	if(currid == 0)
	{
		while(*endmark != 0)
		{
			p(full,semid);
			p(mutex,semid);
			currJob[ (*numJob) ] = shmem[left.pid];
			(*numJob)++;
			left.pid = (left.pid+1)%buffSize;
			v(mutex,semid);
			v(empty,semid);
			
		}
	}
	if(currid == 1)
	{
		int y,z;
		int ramPos;
		y = 0;
		while(*endmark != 0)
		{
			for(y = 0; y < *numJob; y++)
			{
				int ramSize = rows * cols;
				ramPos = runCheck(ram,ramSize,currJob[y].size);
				if(ramPos >= 0 && currJob[y].state != -1)
				{
					currJob[y].state = 1;
					for(z = ramPos; z < currJob[y].size; z++)
					{
						ram[z] = currJob[y].id;
					}
				}
				if(currJob[y].time == 0)
				{
					currJob[y].state = 1;
					for(z = 0; z < rows*cols; z++)
					{
						if(ram[z] == currJob[y].id) // if ram = the current job's id, remove that job
						{
							ram[z] = '.';
						}
					}
				}
			}
		}
		
	}
	if(currid == 2)
	{
		int y;
		int ramPos = 0;
		while(*endmark != 0)
		{
			printJobs(currJob,*numJob);
			print(ram,rows,cols);
			for(y= 0; y < *numJob; y++)
			{
				if(currJob[y].state == 1)
				{
					currJob[y].time = currJob[y].time - 1;

				}
			}
		}
		
	}


 
	return 0;


}




p(int s,int sem_id)
{
struct sembuf sops;

sops.sem_num = s;
sops.sem_op = -1;
sops.sem_flg = 0;
if((semop(sem_id, &sops, 1)) == -1) printf("%s", "'P' error\n");
}

v(int s, int sem_id)
{
struct sembuf sops;

sops.sem_num = s;
sops.sem_op = 1;
sops.sem_flg = 0;
if((semop(sem_id, &sops, 1)) == -1) printf("%s","'V' error\n");
}

int isFinished(job currJob[], int numJob)
{

	int x;
	for(x = 0; x < numJob; x++)
	{
		if(currJob[x].state < 0)
		{
			return 0; // 0 means it is waiting for another process to finish or for resources
		}
	}
	return 1; // 1 means it is still running
}


int runCheck(char ram[] , int ramSize, int run)
{
	int x, pos;
	pos = 0;

	for(x = 0; x < ramSize; x++)
	{
		if(ram[x] == ".") // Availiable RAM slot
		{
			pos++;
		}
		else
		{
			pos = 0;
		}
	}
	if( index >= run)
	{
		return x - pos;
	}
	else
	{
		return -1; // The Process was already terminated if = -1
	}
}

void printJobs (job currJob[] , int numJob)
{
	int x;
	printf("ID\tthePID\tSize\tSec");
	for(x = 0; x < numJob; x++)
	{
		printf("%d\t%d\t%d\t%d\n", currJob[x].id, currJob[x].pid, currJob[x].size, currJob[x].time);
	}
	printf("\n");
}
void print(char ram[], int rows, int cols)
{
	int currRow,currCol;
	int ramPos;
	
	for(currRow = 0; currRow < rows; currRow++)
	{
		for(currCol = 0; currCol < cols; currCol++)
		{
			printf("%d", ram[ramPos] );
			ramPos++;
		}
		printf("\n");
	}
	printf("\n");
}

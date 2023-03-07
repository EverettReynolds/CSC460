// Everett Reynolds
// MM1 Shutdown


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
	int status;
}job;

int main(int argc, char *argv[])
{

	FILE *fopen(), *fp;
	int rows, cols, buffSize,shmid,semid,charid,lastid;	

	if((fp = fopen("file","r")) == NULL)
        {
                        printf(":( could not open file to write.\n");
                        return(-1);
        }
	
	fscanf(fp, "%d", &shmid);
	fscanf(fp, "%d", &charid);
	fscanf(fp, "%d", &semid);
	fscanf(fp, "%d", &rows);
	fscanf(fp, "%d", &cols);
	fscanf(fp, "%d", &buffSize);
	fscanf(fp, "%d", &lastid);
	fclose(fp);

	
	job *shmem = (struct jobStruct*) shmat(shmid,NULL,SHM_RND);
	int *flag = (int*) shmat(lastid,NULL,SHM_RND);
	*flag = 0;
	
	if(shmdt(shmem) == -1)
	{
		printf("Memory Detachment Error.");
	}

	 if( (shmctl(shmid,IPC_RMID,NULL)) == -1)
         {

         	printf("Error Removing Shared Memory.");

         }

	 if( (shmctl(lastid,IPC_RMID,NULL)) == -1)
         {

                printf("Error Removing Shared Memory.");

         }


	if(shmdt(flag) == -1)
        {
                printf("Memory Detachment Error.");
        }
	

	 if( (semctl(semid,0,IPC_RMID,0)) == -1)
         {

                printf("Error Removing Semaphores.");

         }


	system("./rmfile");
	system("rm file");
	system("./rmshm");

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



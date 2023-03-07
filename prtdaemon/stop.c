// Everett Reynolds
// Stop Daemon







#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <math.h>
#include <sys/ipc.h>
#include <stdlib.h>


struct daemonVals
{

	int pid;
	char name[30];
	int buff;
	

};

#define MUTEX 1
#define EMPTY 0
#define FULL  2
#define FRONT shmem[3]
#define REAR shmem[4]



int main(int argc, char *argv[])
{


	FILE *fopen(), *fp;
        int shmid,structId, mutexId,fullId,emptyId;
	struct daemonVals *buffStruct;

 		if((fp = fopen("file","w")) == NULL)
                {
                        printf(":( could not open file to write.\n");
                        return(0);
                }
		fscanf(fp, "%d", shmid);
		fscanf(fp, "%d", structId);
		fscanf(fp, "%d", mutexId);
		fscanf(fp, "%d", fullId);
		fscanf(fp, "%d", emptyId);
		fclose(fp);


                        if( (semctl(mutexId,0,IPC_RMID,0)) == -1)
                        {

                                printf("Error Removing Semaphore.");

                        }
	
			if( (semctl(fullId,0,IPC_RMID,0)) == -1)
                        {

                                printf("Error Removing Semaphore.");

                        }


			if( (semctl(emptyId,0,IPC_RMID,0)) == -1)
                        {

                                printf("Error Removing Semaphore.");

                        }

                        if( (shmctl(shmid,IPC_RMID,NULL)) == -1)
                        {

                                printf("Error Removing Shared Memory.");

                        }
			system("./rmshm");
			system("./rmFile");
			system("rm file");









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




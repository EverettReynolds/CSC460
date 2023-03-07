//Everett Reynolds
// MM1 Producer


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
       // fscanf(fp, "%d", &lastid);
        fclose(fp);


        job *shmem = (struct jobStruct*) shmat(shmid,NULL,SHM_RND);
	char* shmlast  = (char*) shmat(lastid,NULL,SHM_RND);

	p(empty,semid);
	p(mutex,semid);

	shmem[right.pid].pid = getpid();
	shmem[right.pid].id = *shmlast;
	shmem[right.pid].size = atoi(argv[1]);
	shmem[right.pid].time = atoi(argv[2]);

	int size = atoi(argv[1]);
	int runTime = atoi(argv[2]);
	int ram = rows*cols;
	if(size > runTime || runTime > 30 || runTime < 2)
	{

		printf("Invalid Size Or Time.");
		return -1;

	}
	
	shmem[right.pid].state = 0;
	(*shmlast) = (*shmlast) + 1;
	right.pid = (right.pid + 1)% buffSize;
	
	v(mutex,semid);
	v(full,semid);

	printf("%d was finished with %d blocks of memory in %d seconds\n", getpid(), atoi(argv[1]), atoi(argv[2]));
	

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


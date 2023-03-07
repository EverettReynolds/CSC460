// Everett Reynolds
// Cheetah Sync
// 3-10-2022
// CSC 460

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	


	/****   Detach and clean-up the shared memory  ******/

	if (shmdt(shmem) == -1 ) printf("shmgm: ERROR in detaching.\n");

	if (firstID == getpid())         // ONLY need one process to do this
	{
	if ((shmctl(shmid, IPC_RMID, NULL)) == -1)
    		printf("ERROR removing shmem.\n");
	if ((semctl(sem_id, 0, IPC_RMID, 0)) == -1)
    	printf("%s", "Parent: ERROR in removing sem\n");



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



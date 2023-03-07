
//Everett Reynolds
// CSC 460
// 2-20-21
// Tortoise Sync

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define TURN shmem[0]

main(int argc, char *argv[])
{
	


	int i , shmid, *shmem,N;
	int firstID = getpid();
	char bet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	

	shmid  =  shmget(IPC_PRIVATE, sizeof(int), 0770);
	
	if (shmid == -1)
    	{
    		printf("Could not get shared memory.\n");
    		return(0);
    	}


/****   Attach to the shared memory  ******/

	shmem = (int *) shmat(shmid, NULL, SHM_RND);


/****  Initialize the shared memory ****/

	TURN = 0;
	
	if(argc == 1)
	{
		N = 1;
	}
	if(N < 1)
	{
		N = 1;
	}
	if(N > 26)
	{
		N = 26;
	}
	else
	{
		N = atoi( argv[1] );
	}
	
	char letters[N];

	for(i = 0; i < N; i++)
	{
		letters[i] = bet[i];
	}
	
	int ID = 0;
	int nextID = 1;
		
	for(i = 0; i < N-1; i++)
	{
		if( fork() == 0 )
		{
			ID = (ID+1)%N;
			nextID = (nextID+1)%N;
		}
		else
		{
			break;
		}
	}

	if( fork() )
	{
		for(i=0; i<N; i++)
		{
			while(TURN == ID);
			printf("%c:%d\n", letters[ID], getpid() );
			TURN = (TURN + 1)%N;
			
		}
	}
	else
	{
		for(i=0; i<N; i++)
		{
			while(TURN == nextID);
			TURN = (TURN+2)%N;
		}
	}
	
	sleep(1);




	if (shmdt(shmem) == -1 ) printf("shmgm: ERROR in detaching.\n");

	if (firstID == getpid())         // ONLY need one process to do this
	if ((shmctl(shmid, IPC_RMID, NULL)) == -1)
    		printf("ERROR removing shmem.\n");

	printf("alldone\n");



}

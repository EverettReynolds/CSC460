// Everett Reynolds
// Bob's Bistro



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <math.h>
#include <sys/ipc.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{



	int x,y, semId, procNum;
	procNum = 0;
        bool con = true;
	int ran;	

	semId = semget(IPC_PRIVATE , 5 , 0777);
	
	if(semId == -1)
	{

		printf("Error Getting Semaphores...\n");
		return(-1);

	}
	
	for(x = 0; x < 5; x++)
	{

		semctl(semId, x , SETVAL, 1);

	}
	
	for(x = 0; x < 4; x++)
	{

		if( fork() == 0)
		{
		
			procNum++;

		}
		else
		{

			break;

		}


	}

	while(con)
	{

		for(x = 0; x < procNum; x++) // Thinking Process
		{
			printf("\t");
		}
		printf("%d THINKING\n", procNum);
		
		
	
		for( x = 0; x < 1000; x++)  // Attempting to Cause Deadlock by Eating up CPU
		{

			for(y = 0; y < 1000; y++)
			{
			
				
			
			}

		}

		 for(x = 0; x < procNum; x++) // Hungry Process
                 {
                        printf("\t");
                 }
		 printf("%d HUNGRY\n", procNum);
		
		 p((procNum+1)%5 , semId); // Left Chopstick
		
		
	  
		p(procNum,semId); // Right Chopstick
		

		for(x = 0; x < procNum; x++) // Eating Process
                 {
                        printf("\t");
                 }
                 printf("%d EATING\n", procNum);
		
		  for( x = 0; x < 1000; x++)  // Attempting to Cause Deadlock by Eating up CPU
                {

                        for(y = 0; y < 1000; y++)
                        {



                        }

                }
		
		 v((procNum+1)%5 , semId); // Left Chopstick
		 v(procNum , semId); // Right Chopstick


		
		
			

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


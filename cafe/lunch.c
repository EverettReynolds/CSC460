// Everett Reynolds
// Cristina's Cafe



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



#define N 5 // Philosopher Num
#define thinking 0 
#define hungry 1
#define eating 2
#define dead 9
#define left (x+1)%N
#define right (x-1)%N



void think(int x , int* proc);
void eat();
void takeForks(int x, int chopId, int* proc );
void putForks(int x, int chopId, int* proc );
void test(int x, int chopId, int* proc);
void markDead(int x, int* proc);



int main(int argc, char *argv[])
{

	int x,semId,clockId, procId,chopId;
	int* clock; // time keeper var
	int* proc; // process keeper var
  //	int* state;	
	int pId = getpid();
	srand(time(NULL));

	 clockId = shmget(IPC_PRIVATE , 100*sizeof(int) , 0777);
	 if(clockId == -1)
	 {

                printf("Error Getting Shared Memory...\n");
                return(-1);

         }

	clock = (int*) shmat(clockId,NULL,SHM_RND);
	

	 procId  = shmget(IPC_PRIVATE , N*sizeof(int) , 0777);
         if(procId == -1)
         {

                printf("Error Getting Shared Memory...\n");
                return(-1);

         }
	
	proc = (int *)shmat(procId,NULL,SHM_RND);
	for(x=0; x<5; x++) // Initialize all processes to thinking by default as mentioned in class
	{

		proc[x] = thinking;

	}
	
	for(x = 0; x < 100; x++)
	{
		clock[x] = x+1;
	}
	
	semId = semget(IPC_PRIVATE , 5 , 0777); // 5 philosophers 

        if(semId == -1)
        {

                printf("Error Getting Semaphores...\n");
                return(-1);

        }

	chopId = semget(IPC_PRIVATE , 5 , 0777); // 5 chopsticks

        if(chopId == -1)
        {

                printf("Error Getting Semaphores...\n");
                return(-1);

        }

	
	for(x=0; x<5; x++) // Default 0 to prevent errors in data 
        {

                semctl(semId,x,SETVAL,0); 
		semctl(chopId,x,SETVAL,0);

        }
	semctl(chopId,5,SETVAL,1);
	semctl(semId,5,SETVAL,1);

	int curr = 0;
	
	for(x = 0; x < 5; x++) // forking a TOTAL of six processes
	{

		if(fork() != 0)
		{
			curr = curr + 1;
		}
		else
		{
			break;
		}

	}	
	
	


	//*clock = 0;

	if(getpid() != pId)
	{
	
		while(*clock < 100)
		{
			
			think(x,proc);
		        proc[x] = hungry;
			takeForks(x,chopId,proc);
			eat();
			putForks(x,chopId,proc);	
	
		}

	}
	else if(pId == getpid())
	{

		while(proc[0] != dead || proc[1] != dead || proc[2] != dead || proc[3] != dead || proc[4] != dead)
		{

			printf("%d. \t", *clock );	
			for(x = 0; x < 5; x++)
			{

				if(*clock > 100)
				{
				
					markDead(x,proc);

				}
				else if(proc[x] == thinking)
				{
				
					printf("%-15s", "thinking");

				}
				else if(proc[x] == hungry)
                                {

                                        printf("%-15s", "hungry");

                                }
				else if(proc[x] == eating)
                                {

                                        printf("%-15s", "eating");

                                }



			}

			*clock = *clock + 1;
			sleep(1);
			printf("\n");
		}
		printf("CleanUp of Shared Memory and Semaphores\n");
		sleep(1);
		if( (shmctl(clockId,IPC_RMID,NULL)) == -1)
		{
			printf("Error Removing Shared Memory..");	
		}
		if( (shmctl(procId,IPC_RMID,NULL)) == -1)
                {
                        printf("Error Removing Shared Memory..");
                }
		if( (semctl(chopId,0,IPC_RMID,0)) == -1)
                {
                        printf("Error Removing Semaphores..");
                }
		if( (semctl(semId,0,IPC_RMID,0)) == -1)
                {
                        printf("Error Removing Semaphores..");
                }



	}
	return 0;

}


void think(int x , int* proc) // Sleep in an interval of 5 - 15

{

	int num;
	num = rand() % 10 + 5;
	sleep(num);
	proc[x] = hungry;

}

void eat() // Sleep in an interval of 1 - 3
{

	int num;
        num = rand() % 2 + 1;
        sleep(num);


}

void takeForks(int x, int chopId, int* proc )
{

	p(5,chopId);
	test(x,chopId,proc);
	v(5,chopId);	
	p(x,chopId);	

}
void putForks(int x,int chopId, int* proc )
{

	p(5,chopId);
        proc[x] = thinking;
        test(left,chopId,proc);
        test(right,chopId,proc);
        v(5,chopId);
        

}
void test(int x, int chopId, int* proc)
{

	if(proc[x] == hungry && proc[left] != eating && proc[right] != eating)
	{
	
		proc[x] = eating;
		v(x,chopId);		
	
	}

}
void markDead(int x, int* proc)
{

	if(proc[x] == thinking)
	{

		proc[x] = dead;
		printf("%-15s", "dead");

	}
	else if(proc[x] == eating)
	{

		 printf("%-15s", "eating");


	}
	else if(proc[x] == hungry)
	{
	
		 printf("%-15s","dead");
		

	}
	else
	{

		 printf("%-15s", "dead");


	}

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





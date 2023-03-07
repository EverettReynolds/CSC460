#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <string.h>
#include <math.h>
#include <sys/ipc.h>

int main(int argc, char *argv[])
{

	FILE *fopen(), *fp;
	int x,y,z,a,semId,numSems,shmid,num;
	int* Coins;
	
	if( argc == 1)
	{
		
		printf("Initializing Memory.");
		
		if((fp = fopen("cryptodata","w")) == NULL)
                {
                        printf(":( could not open cryptodata to write.\n");
                        return(0);
                }

		semId  =  semget(IPC_PRIVATE, 1 , 0777);// ask OS for semaphores
        	if (semId == -1)
        	{
                	printf("Could not get shared memory.\n");
                	return(0);
        	}
		semctl( semId , 0, SETVAL, 1);


	
		shmid  =  shmget(IPC_PRIVATE, 4, 0777);
		if (shmid == -1)
    		{
    			printf("Could not get shared memory.\n");
    			return(-1);
    		}
		semctl( semId , 0, SETVAL, 1);
		
		Coins = (int*) shmat( shmid, NULL, SHM_RND);
		*Coins = 1000;

		fprintf( fp, "%d\n", shmid);
		fprintf( fp, "%d\n", semId);
		fclose(fp);
	
					
	}
	else if (argc > 1)
	{

		int input = atoi(argv[1]);
		int pid = getpid();
		int id = 0;
		
		if(strcmp("coins",argv[1]) == 0)
		{

			if( (fp = fopen("cryptodata", "r")) == NULL)
			{
			
				printf("Program was Not Initialized. Error Opening File.");
				return 0;
			}
			
			fscanf( fp, "%d", &shmid);
                	fclose(fp);
			Coins = (int* ) shmat(shmid,NULL,SHM_RND);
			printf("Coins: %d\n", *Coins);
			
	

		}
		 if(strcmp("cleanup",argv[1]) == 0)
                {

			if( (fp = fopen("cryptodata", "r")) == NULL)
                        {

                                printf("Program was Not Initialized. Error Opening File.");
                                return 0;
                        }
			
                	fscanf( fp, "%d", &shmid);
                        fscanf( fp, "%d", &semId);
                        fclose(fp);
			Coins = (int* ) shmat(shmid,NULL,SHM_RND);
			 printf("Coins: %d\n", *Coins);


			if( (semctl(semId,0,IPC_RMID,0)) == -1)
			{
			
				printf("Error Removing Semaphore.");

			}
			  if( (shmctl(shmid,IPC_RMID,NULL)) == -1)
                        {

                                printf("Error Removing Shared Memory.");

                        }

			
			printf("Program Terminated.");


		
		}
		if(input > 0 && input < 101)
		{
			printf("Simulation Start.");
			if( (fp = fopen("cryptodata", "r")) == NULL)
                        {

                                printf("Program was Not Initialized. Error Opening File.");
                                return 0;
                        }
			 fscanf( fp, "%d", &shmid);
                        fscanf( fp, "%d", &semId);
                        fclose(fp);
			 Coins = (int* ) shmat(shmid,NULL,SHM_RND);
			
			for( x = 0; x < 16; x++)
			{
			
			//	num = (int) pow(2,x);
				for (y = 0; y < x; y++)
				{
				
					num = num * 2;
				
				}
				a = fork();
				if(a == 0)
				{
					if (fork() == 0)
					{
						num = 0 - num;
					}
					break;	
				}
				id++;
				
							
			}
			if(getpid() != pid)
			{
				for( x = 0; x < input; x++)
				{
					if(num < 0) // Child
					{
						for(y = 0; y < input; y++)
						{
							p(0,semId);
							printf("\t%d - %d\n", *Coins, -(num),  (*Coins+num));
							*Coins = Coins - num;
							v(0,semId);
						}
					}
					else
					{
					
						for(z = 0; z < input; z++)
						{
					
							p(0,semId);
                                                        printf("\t%d + %d\n", *Coins, (num),  (*Coins+num));
                                                        *Coins = Coins + num;
                                                        v(0,semId);

				
						}

					}
				} 
			}
						
		
		}
		else
		{

			printf("The Input Is Invalid.\n");
			return 0;

		}	



	}
	else
	{
	
		printf("Too Many Command Arguments.\n" );
		

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











// Everett Reynolds
// CSC 460
// 28-2-2022
// Shared Bulletin Board

#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/shm.h>

struct bbStruct
{
        int id;
        char name[20];
        int favNum;
        char favFood[30];
};


int main(int argc, char** argv)
{

	int shdmemid;
	char * ourName;
	struct bbStruct * shdmem;
	FILE *fopen(), *file;

	
    	if ( (file = fopen("/pub/os/bb/BBID.txt","r")) == NULL)
     	{
     		printf(":( could not open BBID to read.\n");
     		return(0);
     	}
	
	if(argc > 2)
	{
		printf("Bad Input.");
		return 0;

	}
	
	ourName = argv[1];
	
	while( strlen( ourName ) > 20 )
	{
		ourName[strlen(ourName)-1] = '\0';
		
		
	}
	
	fscanf(file, "%d", &shdmemid);
	fclose(file);
	
	shdmem = (struct bbStruct *) shmat(shdmemid, NULL, SHM_RND);

	//shdmem = shdmem + 9;
	
	//strcpy(shdmem -> name, ourName);

	int i=0;
	shdmem = i;
	int count = 18;
        for (i=0;i<count;i++)
		 printf("%2d: %20s| %8d| %30s|\n",i,shdmem -> id,shdmem -> name,shdmem -> favNum,shdmem -> favFood); 
		shdmem++;
	return 0;


}



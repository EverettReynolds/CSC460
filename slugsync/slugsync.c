#include <stdio.h>

int main(int argc, char* argv[])
{

	FILE *fopen(), *f;
	int num,i;
	char bet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	if(argc == 1)
	{
		num = 1;
	}
	else
	{
		num = atoi(argv[1]);
	}
	if(num < 1)
	{

	}
	if(num > 26)
	{
		num = 26;
	}
	if( (f = fopen("file","w") ) == NULL)
	{
		printf( " Could not Open file.\n");
		return(0);
	}
	char letters[num];
	for(i=0; i < num; i++)
	{
		letters[i] = bet[i];
	}
	
	fprintf(f,"%d:",0);
	fclose(f);
	
	int id1 = 0;
	int id2 = 1;
	
	for( i = 0; i < num; i++)
	{
		if(fork() == 0)
		{
			id1 = (id1+1) % num;
			id2 = (id2+1) % num;
		}
		else
		{
			break;
		}
	}

	i = 0;
	int check;
		
	for( i = 0; i < num; i++)
	{
		check = -1;
		while(check != id1)
		{
			if( (f = fopen("file","r") ) == NULL)
			{
				printf(":( could not open file to read.\n");
				return(0);	
			}
			fscanf(f,"%d",&check);
			fclose(f);
		}
	

		printf("%c:%d\n" ,letters[id1],getpid());

	if( (f = fopen("file","w") ) == NULL)
	{
		  printf(":( could not open file to read.\n");
                                return(0);

	}
	
		fprintf(f,"%d",id2);
		fclose(f);
	}	
	return(0);
}

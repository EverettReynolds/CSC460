// Everett Reynolds
// CSC 460
// 1-31-22
// Fork In The Road

#include<stdio.h>

int main(int args, char ** argv)
{

	int i,j,k;
	int res = 0;
	int branch = atoi(argv[1]);
	
	printf("Gen\tPID\tPPID\n");
	
	for(i = 1; i <= branch; i++)
	{
		k = fork();
		if(k == 0)
		{
			res++;
			i = 0;
			branch--;	
		}
	}
	printf("%d\t%d\t%d\n",res,getpid(),getppid());
	sleep(1);
	return(0);
}

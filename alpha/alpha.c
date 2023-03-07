// Everett Reynolds
// CSC 460
// 2-6-2022
// Alpha^Alpha




#include <stdio.h>

int main( int argc, char* argv[] )
{

	int num,i;
	char alpha;
	if(argc == 1)
	{
		num = 0;
	}
	else
	{
		num = atoi( argv[1] );
	}
	if(num < 1)
	{
		num = 1;
	}
	if(num > 26)
	{
		num = 26;
	}
	alpha = 'A';
	for(i = 0; i < num; i++)
	{
		if(fork() == 0)
		{
			alpha++;
		}
	}
	printf( "%c:%d\n" , alpha, getpid() );
	sleep(1);
	return 0;
	

}

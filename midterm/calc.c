#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[])
{
	int a,b;
	if(argc != 4)
	{
		perror("wrong argument");
		exit(1);
	}
	a = atoi(argv[1]);
	b = atoi(argv[3]);

	
	if(strcmp(argv[2],"+")==0)
	{
		printf("%d\n", a+b);
	}
	
	else if(strcmp(argv[2],"-")==0)
	{
		printf("%d\n", a-b);
	}
	
	else if(strcmp(argv[2],"x")==0)
	{
	
		printf("%d\n", a*b);
	}
	
	else if(strcmp(argv[2],"/")==0)
	{
		if(b==0)
		{
			perror("can not divid by zero");
			exit(1);
		}
		printf("%d\n", a/b);
	}

}

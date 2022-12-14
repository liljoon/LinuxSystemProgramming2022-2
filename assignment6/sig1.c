#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//SIGINT 들어오면 처리하는 handler function
void SigIntHandler(int signo)
{
	printf ("Received a SIGINT signal\n");
	printf ("Terminate this process\n");

	exit(0);
}

void main()
{
	/* SIGINT signal handler: SigIntHandler */
	signal(SIGINT, SigIntHandler); // handler function 등록
	
	printf("Press ^C to quit\n");
	while(1) // 만약 다른 signal이 들어왔을 때 끝나지 않고 다시 pause
		pause(); // signal이 올때까지 기다림.
}

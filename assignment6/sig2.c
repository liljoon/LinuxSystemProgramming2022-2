#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void SigUsrHandler(int signo)
{
	if (signo == SIGUSR1) // SIGUSR1 signal이 들어오면 출력
	{
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2) // SIGUSR2 이면 출력
	{
		printf("Received a SIGUSR2 signal\n");
	}
	else // 그 외에는 종료, 여기 main에서는 그 외에 signal이 등록이 안되어서 일어날일 없을듯?
	{
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

void main()
{
	/* SIGUSR1 signal handler: SigUsrHandler */
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) // SIGUSR1 등록
	{
		perror("signal");
		exit(1);
	}
	// SIGUSR2 등록
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	for( ; ; )
		;
		pause(); // signal 들어올때마다 처리 후 다시 pause (process 를 wait상태로 만듦)
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void SigHandler(int signo) // child가 종료되면 비동기적으로 실행됨
{
	printf("A child killed\n");
	exit(0);
}

main()
{
	pid_t	pid;
	int		status;

	signal(SIGCHLD,SigHandler); // CHILD가 종료되면 SIGCHLD를 받게됨, handler 등록
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		printf("I'm a child\n");
		sleep(2); //실행 후 2초 후 종료
	}
	else  {
		while (1) // 종료되지 않고 다른 일을 처리하고 있다고 가정
			;
	}
}

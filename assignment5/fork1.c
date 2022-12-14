#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	int		pid;

	// fork 함수 호출하여 process 생성 후 pid에 반환값 저장
	if ((pid = fork()) == -1)  {
		perror("fork");
		exit(1);
	}
	// pid가 0은 child process
	else if (pid == 0)  {
		/* child */
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	// pid 가 0이 아닌 process id를 갖으면 child process가 저장되고 parent가 실행하는 부분
	else  {
		/* parent */
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}
}

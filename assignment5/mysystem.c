#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int mysystem(char *cmd)
{
	pid_t pid;
	// child process 에서 /bin/sh -c 옵션으로 cmd 를 실행
	if ((pid = fork()) == 0)
	{
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		exit(0);
	}
	// child process 끝날때까지 기다림.
	else
	{
		waitpid(pid, NULL, 0);
	}
}

int main()
{
	mysystem("ls -al"); // 실행 예시
}

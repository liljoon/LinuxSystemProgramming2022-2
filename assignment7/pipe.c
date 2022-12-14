#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_BUF	128

void main()
{
	int		n, fd[2], fd2[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	if (pipe(fd) == -1) // pipe로 fd를 open
	{
		perror("pipe");
		exit(1);
	}
	if (pipe(fd2) == -1) // pipe로 fd2를 open
	{
		perror("pipe");
		exit(1);
	}
	if ((pid = fork()) < 0) // 자식 프로세스 생성
	{
		perror("fork");
		exit(1);
	}
	else if (pid == 0) // child
	{
		close(fd[1]); // fd 출력은 close
		close(fd2[0]); // fd2 읽기 close
		printf("Child : Wait for parent to send data\n");
		// fd[0]으로 입력이 들어올 때 까지 wait
		if ((n = read(fd[0], buf, MAX_BUF)) == -1)
		{
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout); // 개행이 없기에 buffer flush
		write(STDOUT_FILENO, buf, n); // 읽어온 n개 만큼 출력
		strcpy(buf, "Message : child to parent\n");
		write(fd2[1], buf, strlen(buf) + 1);
	}
	else // parent
	{
		close(fd[0]); // fd 입력은 close
		close(fd2[1]); // fd2 출력 close
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		write(fd[1], buf, strlen(buf) + 1); // child로 출력
		if ((n = read(fd2[0], buf, MAX_BUF)) == -1)
		{
			perror("read");
			exit(1);
		}
		write(1, buf, n);
	}
	exit(0);
}

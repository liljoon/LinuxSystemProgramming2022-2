#include <stdio.h>
#include "synclib.h"
#include <stdlib.h>

#define NLOOPS 5

void main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT(); // pipe 두개 초기화
	if ((pid = fork()) < 0)
	{
		perror("fork");
		exit(1);
	}
	else if (pid > 0) // parent
	{
		for (i = 0; i < NLOOPS; i++)
		{
			// 일 처리
			TELL_CHILD(); // child 실행
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); // child 기다림.
		}
	}
	else // child
	{
		for (i = 0; i < NLOOPS; i++)
		{
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT(); // parent한테 신호받으면 실행
			// 일 처리
			TELL_PARENT(); // 일 끝나면 parent 실행
			printf("Child: Tell to parent\n");
		}
	}
}

#include <stdio.h>

#define	MAX_CMD		256

//process fork하여 실행, 함수가 다 완료 안되어도 다음 반복문 실행

void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		if ((pid = fork()) < 0)  {
			perror("fork");
			exit(1);
		}
		else if (pid == 0)  {
			DoCmd(cmd);
			exit(0); // child process는 함수 실행 후 종료 되야함. while문 안돌게
		}
#if 0
		else  {
			wait(NULL);
		}
#endif
	}
}

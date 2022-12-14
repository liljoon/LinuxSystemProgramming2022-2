#include <stdio.h>
#include <pthread.h>

#define	MAX_CMD		256

// thread 생성하여 함수 실행, 함수 다 안끝나도 다음 반복문 실행

void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL); // thread 종료
}

main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) != 0)  {
			perror("pthread_create");
			exit(1);
		}

// 1로 바꾸면 쓰레드 종료까지 기다림
#if 0 
		pthread_join(tid, NULL);
#endif
	}
}

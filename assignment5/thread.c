#include <stdio.h>
#include <pthread.h>

// thread 실행 시킬 함수. 반환값 없음
void
PrintMsg(char *msg)
{
	printf("%s", msg);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;
	char		*msg1 = "Hello, ";
	char		*msg2 = "World!\n";

	// tid1을 통해 PrintMsg 함수 실행, 인자로 msg1을 넣음
	/* Thread ID: tid1, Thread function: PrintMsg, Thread argument: msg1 */
	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// 같은 방법으로 msg2 전달
	if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	
	printf("Threads created: tid=%ld, %ld\n", tid1, tid2);
	
	// thread 종료까지 기다림, 반환값 없음.
	/* Wait for tid1 to exit */
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);
}

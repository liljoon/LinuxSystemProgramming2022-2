#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define	THREAD_MAIN
/*
#define	THREAD_1
#define	THREAD_2
*/

// SIGINT 받았을 때 호출.
void
SigIntHandler(int signo)
{
	// 어떤 쓰레드가 받았는지 출력, 자신의 tid 출력
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate this process\n");

	exit(0); // 종료
}

void
Thread1(void *dummy)
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler); // thread 1번 handler 등록
#endif

	while (1) // 종료되지 않고 대기
		;
}

void
Thread2(void *dummy)
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler); //thread 2번에서도 handler 등록
#endif

	while (1) // 종료되지 않고 대기
		;
}

main()
{

	pthread_t	tid1, tid2; // thread 2개 생성

	if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// thread tid 출력
	printf("Create two threads: tid1=%d, tid2=%d\n", tid1, tid2);
	printf("Main thread: tid=%d\n", pthread_self()); 

// Main thread에서만 딱 한번 실행됨
#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif

	printf("Press ^C to quit\n");

	for ( ; ; ) // signal 들어올 때까지 대기
		pause();
}

// 최종적으로 main thread에서만 signal을 받는 것을 확인.
// handler 안에서 출력되는 tid가 main tid와 동일한 것을 확인할 수 있음.


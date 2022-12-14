#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t	ThreadId[2];

void
SigIntHandler(int signo)
{
	int		i;

	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);
	// 쓰레드 2개 종료 요청
	for (i = 0 ; i < 2 ; i++)  {
		/* Cancel execution of all threads */
		if (pthread_cancel(ThreadId[i]))  {
			perror("pthread_cancel");
			exit(1);
		}
	}
	// 쓰레드 2개가 종료될 때까지 기다림.
	for (i = 0 ; i < 2 ; i++)  {
		if (pthread_join(ThreadId[i], NULL))  {
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void
Thread1(void *dummy)
{
	printf("Thread %d created.....\n", pthread_self());
	// 다른 쓰레드가 종료시킬 수 있도록 설정(default)
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	// 종료할 때 바로 종료안되고 critical section을 벗어나 lock이 풀릴 때 종료 (default)
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

void
Thread2(void *dummy)
{
	printf("Thread %d created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

main()
{
	// thread 2개 생성
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler); // handler 설정

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause();
}

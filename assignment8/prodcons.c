#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

// multi thread에서 전역변수는 shared memory
BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem; // 각 thread 함수에서 semaphore 사용을 해야하므로 전역변수로 생성

// thread 별로 usleep 해주는 함수
void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void
Producer(void *dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
		// 만약 비어있는 칸이 없다면 wait
		if (sem_wait(&EmptySem) != 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		// critical section 보호
		if (sem_wait(&MutexSem) != 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;
	
		// critical section 끝
		if (sem_post(&MutexSem) != 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 채운 개수 +1, signal
		if (sem_post(&FullSem) != 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL); // thread 종료
}

void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  {
		// 채워져있는게 하나도 없으면 wait
		if (sem_wait(&FullSem) != 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//critical section 보호
		if (sem_wait(&MutexSem) != 0)  {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;
		
		// critical section 끝
		if (sem_post(&MutexSem) != 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		// 비어있는 개수 +1 , signal
		if (sem_post(&EmptySem) != 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

void
main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

	// thread 실행 전 semaphore 전역변수 초기화
	if (sem_init(&EmptySem, 0, MAX_BUF) != 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) != 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) != 0)  {
		perror("sem_init");
		exit(1);
	}

	// tid1 생성 Producer, argument는 없음 NULL
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// tid2 생성 Consumer
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// Producer 종료 기다림
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	// Consumer 종료 기다림
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	// 최종 buffer에 몇개 남았는지 . 항상 0개 있어야함
	printf("Main    : %d items in buffer.....\n", Buf.counter);
	
	// semaphore 제거, 다 사용했으므로
	if (sem_destroy(&EmptySem) != 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) != 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) != 0)  {
		perror("sem_destroy");
	}
}

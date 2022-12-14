#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include "prodcons.h"

BoundedBufferType	Buf;

// thread 별로 microsecond만큼 sleep해주는함수.
// 현재 리눅스에서는 쓰레드별로 sleep을 지원함.
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

	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS만큼 buffer에 삽입
		if (Buf.counter == MAX_BUF)  {
			printf("Producer: Buffer full. Waiting.....\n");
			while (Buf.counter == MAX_BUF) // buffer가 꽉찼을 때 busy wait
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		Buf.buf[Buf.in].data = data; //  값 삽입
		Buf.in = (Buf.in + 1) % MAX_BUF; // in update
		Buf.counter++; // shared memory access

		ThreadUsleep(data); // random한 시간동안 sleep
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}

void
Consumer(void *dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS 만큼 값을 뺌.
		if (Buf.counter == 0)  {
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (Buf.counter == 0) // buf가 비어있으면 busy wait
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; // 데이터 가져오기
		Buf.out = (Buf.out + 1) % MAX_BUF; // out update
		Buf.counter--; // shared data access

		ThreadUsleep((rand()%100)*10000); // random 한 시간 sleep
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}

void
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	//Producer thread 생성
	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	//Consumer thread 생성
	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	// producer 종료까지 기다림
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	// consumer 종료까지 기다림
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);
	// 같은 개수를 넣고 같은 개수를 빼서 counter는 항상 0이 되어야하는데
	// synchronization problem 이 발생하면값이 이상해질 수 있음.
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf; // Shared data
pthread_cond_t		NotFull; // condition variable
pthread_cond_t		NotEmpty; // condition variable
pthread_mutex_t		Mutex;	// mutex lock version

void
ThreadUsleep(int usecs) // thread 마다 usleep
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
		if (pthread_mutex_lock(&Mutex) != 0)  { // critical section 보호
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		while (Buf.counter == MAX_BUF)  { // wait하다가 깨어나도 다시 한번 확인을 위한 안전한 코드
			// Full 이 아닐 때 까지 lock 잠시 풀고 wait.
			if (pthread_cond_wait(&NotFull, &Mutex) != 0)  {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		Buf.buf[Buf.in].data = data; // data 삽입
		Buf.in = (Buf.in + 1) % MAX_BUF; // in update
		Buf.counter++; // counter update

		if (pthread_cond_signal(&NotEmpty) != 0)  { // 하나 채웠으므로 NotEmpty 하나 깨움
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		if (pthread_mutex_unlock(&Mutex) != 0)  { // critical section 끝, lock 해제
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep(data); // 랜덤한 시간 sleep
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

	for (i = 0 ; i < NLOOPS ; i++)  {
		if (pthread_mutex_lock(&Mutex) != 0)  { // critical section 보호
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		while (Buf.counter == 0)  { // 만약 비어있다면
			// producer에서 signal 을 보내면 꺠어남, 비어있다면 wait
			if (pthread_cond_wait(&NotEmpty, &Mutex) != 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data; // data 추출
		Buf.out = (Buf.out + 1) % MAX_BUF; // out update
		Buf.counter--; // counter update

		if (pthread_cond_signal(&NotFull) != 0)  { // 하나 뺏으므로 NotFull signal
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}
		if (pthread_mutex_unlock(&Mutex) != 0)  { // critical section 종료
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000); // random시간 sleep
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

	if (pthread_cond_init(&NotFull, NULL) != 0)  { // NotFull condition variabl 초기화, attr은 NULL
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_cond_init(&NotEmpty, NULL) != 0)  { // NotEmpty 초기화
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&Mutex, NULL) != 0)  { // Mutex lock 초기화
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  { // Producer thread생성
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  { // Consumer thread 생성
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  { // Producer 종료 시까지 기다림
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { // Consumer 종료 시까지 기다림
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter); // 결과 0개 남아있어야함.

	if (pthread_cond_destroy(&NotFull) != 0)  { // condition vaiable 삭제
		perror("pthread_cond_destroy");
	}
	if (pthread_cond_destroy(&NotEmpty) != 0)  {
		perror("pthread_cond_destroy");
	}
	if (pthread_mutex_destroy(&Mutex) != 0)  { // Mutex lock 삭제
		perror("pthread_mutex_destroy");
	}
}

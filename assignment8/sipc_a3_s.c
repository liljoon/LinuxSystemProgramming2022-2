#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

static char	pData[1024]; // shared memory 이용
static sem_t	Mutex1, Mutex2; // Mutex는 어쩔 수 없이 전역변수

void server(void *tid) // server thread function, tid 같이 보냄
{
	sem_wait(&Mutex2); // 2에서 signal 보낼 때까지 대기
	//받았으면 출력 및 pData에 데이터 삽입
	printf("server : Received request: %s\n", pData);
	sprintf(pData, "This is a reply from %ld.", pthread_self()); // tid 삽입

	sem_post(&Mutex1); // 다 완료했으므로 1에서 2로 보내는 signal
	printf("server : Replied\n");
}

void client(void *tid) // client thread function
{
	// 바로 shared memory에 data 삽입
	sprintf(pData, "This is a request from %ld.", pthread_self());
	printf("client : Sent a request...\n");
	sem_post(&Mutex2); // 다 데이터 삽입 후 signal
	sem_wait(&Mutex1); // reply 올 때까지 대기
	printf("client : Received reply : %s\n", pData); // Reply오면 출력
}

void main()
{
	pthread_t	tid1, tid2; // thread id 저장

	// semaphore 2개 초기화
	sem_init(&Mutex1, 0, 0);
	sem_init(&Mutex2, 0, 0);

	// pthread 2개 생성
	pthread_create(&tid1, 0, (void *)server, (void*)&tid1);
	pthread_create(&tid2, 0, (void *)client, (void*)&tid2);
	//thread 종료 대기
	pthread_join(tid1, 0);
	pthread_join(tid2, 0);
	// semaphore 삭제
	sem_destroy(&Mutex1);
	sem_destroy(&Mutex2);
}

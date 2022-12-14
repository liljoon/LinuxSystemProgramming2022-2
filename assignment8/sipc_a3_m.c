#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

static char				pData[1024]; // shared memory 이용
static pthread_mutex_t	Mutex; //mutex 전역변수
static pthread_cond_t	cond1, cond2; // 각 방향마다 condition variable
static int				flag; // 0: 초기 , 1: 1->2, 2: 2->1 상태

void server(void *tid) // server thread function, tid 같이 보냄
{
	pthread_mutex_lock(&Mutex); // critical section 보호
	while (flag != 2) // client가 전송 안함 아직
		pthread_cond_wait(&cond1, &Mutex); // cond1에서 기다림
	//받았으면 출력 및 pData에 데이터 삽입
	printf("server : Received request: %s\n", pData);
	sprintf(pData, "This is a reply from %ld.", pthread_self()); // tid 삽입
	flag = 1; // flag update
	pthread_mutex_unlock(&Mutex); // critical section 종료
	pthread_cond_signal(&cond2); // 답장 기다리는 cond2있으면 signal
	printf("server : Replied\n");
}

void client(void *tid) // client thread function
{
	pthread_mutex_lock(&Mutex); // critical section 보호
	// 바로 shared memory에 data 삽입
	sprintf(pData, "This is a request from %ld.", pthread_self());
	printf("client : Sent a request...\n");
	flag = 2; // 전송 flag update
	// 데이터 전송했으니 기다리는 thread 있으면 실행
	pthread_cond_signal(&cond1);
	while (flag != 1) // flag 가 1이 아니면 아직 답장이 없음.
		pthread_cond_wait(&cond2, &Mutex); // cond2에서 wait
	printf("client : Received reply : %s\n", pData); // Reply오면 출력
	pthread_mutex_unlock(&Mutex); // critical section 종료
}

void main()
{
	pthread_t	tid1, tid2; // thread id 저장

	// mutex  init
	pthread_mutex_init(&Mutex, 0);
	// condition variable init
	pthread_cond_init(&cond1, 0);
	pthread_cond_init(&cond2, 0);

	// pthread 2개 생성
	pthread_create(&tid1, 0, (void *)server, (void*)&tid1);
	pthread_create(&tid2, 0, (void *)client, (void*)&tid2);
	//thread 종료 대기
	pthread_join(tid1, 0);
	pthread_join(tid2, 0);
	// mutex, condition variable destroy
	pthread_mutex_destroy(&Mutex);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
}

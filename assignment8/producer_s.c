#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

void
main()
{
	BoundedBufferType	*pBuf;
	int					shmid, i, data;
	int					emptySemid, fullSemid, mutexSemid;

	// shared memory로 bounded buffer를 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// pBuf와 shared memory 연결
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	// 비어있는 개수에 대한 semaphore 초기화
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// 차있는 개수에 대한 semaphore 초기화
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// counter, pBuf등 critical section 보호를 위한 binary semaphore
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	// semaphore 초기화는 consumer에서 해주기 때문에
	// 첫 semWait까지 초기화가 안되었다면0이니까 기다려서 초기화 후 실행되므로 문제없음
	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  {
		if (semWait(emptySemid) < 0)  { // 꽉차있다면 wait로 (busy wait 문제 제거)
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		if (semWait(mutexSemid) < 0)  { // critical section 보호
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		pBuf->buf[pBuf->in].data = data; // data 삽입
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // in update
		pBuf->counter++; // counter update

		if (semPost(mutexSemid) < 0)  { // critical section 끝, signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(fullSemid) < 0)  { // full semaphore 1 증가, signal
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2); // consumer 종료 기다림
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	// semaphore 전부 삭제
	if (semDestroy(emptySemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0)  {
		fprintf(stderr, "semDestroy failure\n");
	}
	// shared memory도 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}

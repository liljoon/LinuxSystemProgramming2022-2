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
	
	// Bounded buffer shared memory open
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// pBuf 와 shared memory 연결
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}
	// semaphore open 
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	// 처음에 다 비어있으므로 MAX_BUF 만큼 초기화
	if (semInitValue(emptySemid, MAX_BUF) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	// fullSemid 0으로 초기화
	if (semInitValue(fullSemid, 0) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	// binary semaphore이므로 1로 초기화
	if (semInitValue(mutexSemid, 1) < 0)  {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999);
	for (i = 0 ; i < NLOOPS ; i++)  {
		// 비어있으면 wait (busy wait 문제 제거)
		if (semWait(fullSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		// critical section 보호
		if (semWait(mutexSemid) < 0)  {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		if (semPost(mutexSemid) < 0)  { // critical section 끝
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		if (semPost(emptySemid) < 0)  { // 비어있는 개수 갱신
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand()%100)*10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}

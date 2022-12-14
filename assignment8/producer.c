#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

// 멀티 프로세스
void
main()
{
	BoundedBufferType	*pBuf;
	int					shmid, i, data;

	// shared memory 생성 및 오픈
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// pBuf와 attach
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	srand(0x8888);
	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS만큼 데이터 삽입
		if (pBuf->counter == MAX_BUF)  {
			printf("Producer: Buffer full. Waiting.....\n");
			while (pBuf->counter == MAX_BUF) // buf가 꽉찼을 경우 busy wait
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand()%100)*10000;
		pBuf->buf[pBuf->in].data = data; // data 삽입
		pBuf->in = (pBuf->in + 1) % MAX_BUF; // in update
		pBuf->counter++; // counter update : shared memory access

		usleep(data); // random 한 시간동안 sleep
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2); // Consumer가 아직 안끝났을 수도 있으므로 sleep
	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	if (shmctl(shmid, IPC_RMID, 0) < 0)  { // shared memory 삭제
		perror("shmctl");
		exit(1);
	}
}

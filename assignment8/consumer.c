#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

void
main()
{
	BoundedBufferType	*pBuf;
	int					shmid, i, data;

	// shared memory 생성 or open
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	// pBuf attach
	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	srand(0x9999);
	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS만큼 데이터 가져오기
		if (pBuf->counter == 0)  {
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (pBuf->counter == 0) // 비어있으면 busy wait
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data; // data 가져오기
		pBuf->out = (pBuf->out + 1) % MAX_BUF; // out update
		pBuf->counter--; // shared memory access

		usleep((rand()%100)*10000); // random 시간동안 sleep
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
	// 마지막에 0개 남아야함.
}

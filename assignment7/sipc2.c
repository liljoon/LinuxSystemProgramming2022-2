#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include <stdlib.h>

int main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;


	/* SHM_KEY, SHM_SIZE, SHM_MODE in shm.h */
	// sipc1 과 동일, shared memory 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1)  {
		perror("shmget");
		exit(1);
	}
	// shared memory 연결
	if ((ptr = shmat(shmid, 0, 0)) == (void *)-1 )  {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr;
	pData = ptr + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1;
	printf("Sent a request.....");

	while ((*pInt) == 1) // sipc1에서 0으로 바꾸면 밑에 코드진행
		;

	printf("Received reply: %s\n", pData); // shared memory 출력

	/* Detach shared memory */
	//연결 해제, physical memory는 sipc1에서 해결
	if (shmdt(ptr) == -1)  {
		perror("shmdt");
		exit(1);
	}
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include <stdlib.h>
#include "semlib.h"

int main()
{
	int		shmid, Mutex1, Mutex2;
	char	*pData;

	/* SHM_KEY, SHM_SIZE, SHM_MODE in shm.h */
	// sipc1 과 동일, shared memory 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1)  {
		perror("shmget");
		exit(1);
	}
	// shared memory 연결
	if ((pData = shmat(shmid, 0, 0)) == (void *)-1 )  {
		perror("shmat");
		exit(1);
	}

	Mutex1 = semInit(SEM_KEY_1);
	Mutex2 = semInit(SEM_KEY_2);
	
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request.....");

	semPost(Mutex2);	
	semWait(Mutex1);

	printf("Received reply: %s\n", pData); // shared memory 출력

	/* Detach shared memory */
	//연결 해제, physical memory는 sipc1에서 해결
	if (shmdt(pData) == -1)  {
		perror("shmdt");
		exit(1);
	}
}

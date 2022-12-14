#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "shm.h"
#include "semlib.h"

void main()
{
	int		shmid, Mutex1, Mutex2;
	char	*pData;
	
	//shared memory 생성 및 연결
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1)
	{
		perror("shmget");
		exit(1);
	}
	if ((pData = (char *)shmat(shmid, 0, 0)) == (char *)-1)
	{
		perror("shmat");
		exit(1);
	}
	// 1에서 2로 보내는 synchronization semaphore
	if((Mutex1 = semInit(SEM_KEY_1)) == -1)
	{
		perror("semInit");
		exit(1);
	}
	// 2에서 1로 보내는 synchornization semaphore
	if((Mutex2 = semInit(SEM_KEY_2)) == -1)
	{
		perror("semInit");
		exit(1);
	}
	// 2에서 1로 signal 기다림.
	if (semWait(Mutex2) == -1)
	{
		perror("semWait");
		exit(1);
	}
	// 데이터 출력 후 다시 반환 데이터 삽입
	printf("Received request: %s\n", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	// 1에서 2로 signal 보냄
	if (semPost(Mutex1) == -1)
	{
		perror("semPost");
		exit(1);
	}
	printf("Replied.\n");
	sleep(1); // 바로 지우지 않고 대기 후 지움
	if (shmdt(pData) == -1)
	{
		perror("shmdt");
		exit(1);
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1) // 절대 잊지 말것
	{
		perror("shmctl");
		exit(1);
	}
	if (semDestroy(Mutex1) == -1) // 이것도 잊지 말것
	{
		perror("semDestoy");
		exit(1);
	}
	if (semDestroy(Mutex2) == -1)
	{
		perror("semDestoy");
		exit(1);
	}
}

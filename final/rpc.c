#include "shm.h"
#include "semlib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int semaphore1, semaphore2;
int shmid;

void processend(int signo)
{
	semDestroy(semaphore1);
	semDestroy(semaphore2);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}

void main()
{
	int *ptr;
	
	signal(SIGINT, processend);
	shmid = shmget(SHM_KEY, sizeof(int), SHM_MODE);
	ptr = shmat(shmid, 0, 0);
	
	semaphore1 = semInit(34 + 0x1000);
	semaphore2 = semInit(34 + 0x2000);
	semInitValue(semaphore1, 0);
	semInitValue(semaphore2, 0);
	while (1)
	{
		semWait(semaphore1);
		int sum = 0;
		for(int i=0;i <= *ptr; i++)
			sum += i;
		*ptr = sum;
		printf("Calc = %d\n", *ptr);
		semPost(semaphore2);
	}
}

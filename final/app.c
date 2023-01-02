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

int semaphore1,semaphore2;
int shmid;
int *ptr;

int Calc(int number)
{
	

	shmid = shmget(SHM_KEY, sizeof(int), SHM_MODE);
	ptr = shmat(shmid, 0 ,0);

	semaphore1 = semInit(34 + 0x1000);
	semaphore2 = semInit(34 + 0x2000);
	*ptr = number;

	semPost(semaphore1);
	semWait(semaphore2);

	return (*ptr);	
}

void main(int argc, char *argv[])
{
	printf("Calc = %d\n", Calc(atoi(argv[1])));
}

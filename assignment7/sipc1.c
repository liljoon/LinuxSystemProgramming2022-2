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
	// 0x9034 주소에, 1024 바이트 만큼, read, write, 없으면 생성
	// shared memory 생성 및 shmid 반환
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1)  {
		perror("shmget");
		exit(1);
	}
	// virtual address 와 shared memory 연결
	if ((ptr = shmat(shmid, 0, 0)) == (void *)-1)  {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr; //int로 형 변환, pInt는 맨 앞4바이트에서
						// 데이터를 썼는지에 대한 flag, 처음엔 0으로 초기화
	//flag 가 1이 될 때까지 무한 루프, spin lock -> 사용 지양
	// synchronization을 위함.
	while ((*pInt) == 0)
		;
	
	pData = ptr + sizeof(int);// flag 건너뛰고 실제 데이터 담을 공간
	printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid()); // 데이터 삽입
	*pInt = 0; // flag를 0으로 만들어서 반대 프로세스에 알림
	printf("Replied.\n");

	sleep(1); // 바로 지우면 상대가 메모리 읽기전에 지울 수 있으므로 sleep

	/* Detach shared memory */
	// 메모리 detach 로 연결 끊음
	if (shmdt(ptr) == -1)  {
		perror("shmdt");
		exit(1);
	}
	// detach를 해도 physical memory는 안 지워지므로 꼭 해야함,Remove ID
	/* Remove shared memory */
	if (shmctl(shmid, IPC_RMID, 0))  {
		perror("shmctl");
		exit(1);
	}
}

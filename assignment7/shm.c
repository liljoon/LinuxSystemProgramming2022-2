#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE]; // 전역변수, static memory

void main()
{
	int		shmid;
	char	*ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  { // heap memory 에 생성
		perror("malloc");
		exit(1);
	}

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  { // shared memory 생성 
	// IPC_PRIVATE : 해당 프로세스만 사용, 프로세스 종료시 자동 삭제
		perror("shmget");
		exit(1);
	}
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  { // shared memory를 virtual address로 연결하여 사용
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);// static memory 주소
	printf("Stack around %p\n", &shmid); // 지역변수로 stack 마지막 근처 주소
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE); // heap 주소
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE); // shared memory주소
	//shared memory 가 stack 과 heap 사이에 존재하는 것을 알 수 있음
	
	if (shmdt(shmptr) < 0)  { // shared memory와 virtual memory 연결 detach
		perror("shmdt");
		exit(1);
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0)  { // physical memory에 생성된 shared memory를 명시적으로 제거 (IPC_PRIVATE이 아닌 경우에는 꼭 해줘야함)
		perror("shmctl");
		exit(1);
	}
}

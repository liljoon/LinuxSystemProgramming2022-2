#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"
#include "shm.h"
#define	NLOOPS	10
#define	SIZE	sizeof(long)
#include <sys/shm.h>
#include <sys/types.h>

int
update(long *ptr)
{
	return ((*ptr)++);
}

void main()
{
	int		fd, i, counter, shmid;
	pid_t	pid;
	caddr_t	area;

	/*  // 기존 실습
	// 항상 0인 파일, 이것을 오픈하여 mmap하면 shared memory가 생김
	if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
	// fork 하여 area를 참조하면 shared memory
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	close(fd);
	*/
	// 추가 실습
	// shared memory 생성 또는 open
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) == -1)
	{
		perror("shmget");
		exit(1);
	}
	area = shmat(shmid, 0, 0); // 메모리 연결
	//
	
	
	TELL_WAIT(); // synchronize 초기화(pipe 이용)
	if ((pid = fork()) < 0)  { // fork
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  { // parent
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			//짝수 경우에만 count하여 값 올림
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter); // 값 출력
			TELL_CHILD(); // child에게 tell하여 시킴
			WAIT_CHILD(); // child가 끝날때까지 기다림
		}
	}
	else  { // child
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT(); // parent가 끝날때 까지 먼저 기다림.
			// 홀 수 경우만 counter , 아닐 경우 에러
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter); // 출력
			TELL_PARENT(); // parent한테 tell해서 처리 시킴.
		}
	}
	shmctl(shmid, IPC_RMID, 0); // 다 사용했으면 shared memory 꼭 삭제
}

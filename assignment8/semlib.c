#include <stdio.h>
#include "semlib.h"
 
// semget으로 semaphore 생성 후 semid 반환
int
semInit(key_t key)
{
    int     semid;
	
	// 입력된 key로 1개의 semaphore로 0600권한, 없으면 생성
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }

    return semid;
}

// semaphore 초기화 함수
int
semInitValue(int semid, int value)
{
	// union 선언
    union semun {
            int     val;
    } semun;

    semun.val = value; // 값 초기화
	//semctl 로 0번째 semaphore한테 SETVAL 명령어를 통해 union값을 통해 초기화
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
		perror("semctl");
        return -1;
    }
 
    return semid;
}
 
// wait function (-1)
int
semWait(int semid)
{
    struct sembuf   semcmd; // 구조체 생성
 
    semcmd.sem_num = 0; // 0번째 semaphore
    semcmd.sem_op = -1; // -1 연산
    semcmd.sem_flg = SEM_UNDO; // 만약 프로세스가 종료되면 자동으로 semaphore 해제
    // 구조체를 통해 semaphore 연산 실행, operation 개수 1개
	if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// Trywait 함수 (-1), semaphore가  0이라면 기다리지 않고 fail
int
semTryWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO; // IPC_NOWAIT 옵션추가하여 fail 처리
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// post, signal 함수  (+1)
int
semPost(int semid)
{
    struct sembuf   semcmd; // 구조체 생성
 
    semcmd.sem_num = 0; // 0번째 semaphore
    semcmd.sem_op = 1; // +1 operation
    semcmd.sem_flg = SEM_UNDO; // 프로세스 종료시 반납
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

// semaphore 값 반환
int
semGetValue(int semid)
{
    union semun {
            int     val;
    } dummy;
 	// 구조체 선언
    return semctl(semid, 0, GETVAL, dummy); // GETVAL 명령어를 통해 값 반환, arg는 필요없으므로 dummy
}

// 다 쓴 semaphore destroy
int
semDestroy(int semid)
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  { // IPC_RMID 명령어로 semaphore 지움
		perror("semctl");
        return -1;
    }
    //close(semid);
 
    return 0;
}

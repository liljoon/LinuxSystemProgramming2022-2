#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// Handler 받기 위한 함수
void SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return ;
}

unsigned int mysleep(unsigned int nsecs)
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler))  { //handler 등록 및 에러처리
		return nsecs;
	}

	/* alarm */
	alarm(nsecs); // alarm 실행하여 nsecs초 후에 SIGALRM 받음

	pause(); // signal 이 들어올 때까지 wait

	return alarm(0); // 남아있는 시간초 출력, 0이어야 정상
}

void main()
{
	printf("Wait for 5 seconds...\n");
	
	mysleep(5);
}

/* 이 프로그램의 문제점은 시간이 다 흐르지 않았는데
다른 시그널이 들어와서 pause를 깨워 종료될 수 있다.
그래서 return 값으로 남은 시간이 0이 아니면 
그러한 상황이 발생한 것을 알 수 있다.*/

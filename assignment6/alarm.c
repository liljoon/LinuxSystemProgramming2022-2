#include <stdio.h>
#include <signal.h>

static unsigned int AlarmSecs;

void SigAlarmHandler(int signo)
{
	alarm(AlarmSecs); // AlarmSecs 초 후에 다시 SIGALRM보냄

	/* Do something */
	printf("."); 
	fflush(stdout); // printf는 개행 문자 없으면 버퍼에만 저장, flush해줘야함

	return ;
}

int SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) // handler 설정
	{
		return -1;
	}
	
	AlarmSecs = nsecs; // 전역변수 지정
	
	alarm(nsecs); // nsecs 뒤에 SIGALRM signal 보냄

	return 0;
}

void main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1); // 1초 주기로 실행
	for ( ; ; )
		pause();
}

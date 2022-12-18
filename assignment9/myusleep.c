#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>


// select를 이용해 usleep만듦
void
myusleep(int secs, int usecs)
{
	struct timeval	tm; //select 에서 넣기 위한 시간 관련 struct

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

	if (select(0, 0, 0, 0, &tm) == -1)  { // 일정 시간동안 기다림
		perror("select");
		return;
	}
}

void
main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000); // 1.5초 sleep후 종료
}

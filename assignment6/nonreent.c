#include <stdio.h>
#include <signal.h>
#include <pwd.h>


void
MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1); // 매 초마다 다시 실행

	printf("in signal handler\n");
	// main에서 매우 빠른 속도로 함수를 계속 호출하고 있는데
	// 여기서 다시 호출하면 getpwnam가 리눅스에서 lock을 걸어 구현해서
	// 데드락에 빠지게 되어 멈춘다.
	if ((rootptr = getpwnam("root")) == NULL)  {
		perror("getpwnam");
		exit(1);
	}

	return;
}

main()
{
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1); // 1초 후에 handler 실행

	for( ; ; ) {
		// 사용자 정보를 가져오는 함수, non-reentrant 함수이다.
		if ((ptr = getpwnam("lsp34")) == NULL)  { 
			perror("getpwnam");
			exit(1);
		}
		//이 사이에서 signal이 발생했다면 다르기 때문에 조건문으로 들어감
		if (strcmp(ptr->pw_name, "lsp34") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}

// main 에서는 매우 빠른 속도로 항상 getpwnam에 들어가있다고 생각
// signal 이 발생하여 getpwnam을 실행하면 데드락.
// 만약 메인에서 적절히 sleep을 넣어주면 우연히 잘 실행되거나
// 우연히 데드락 또는 value가 달라서 출력후 종료되기도 함.

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int		Var = 6;
char	Buf[] = "a write to stdout\n";

main()
{
	int		var;
	pid_t	pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1);// Buf 출력
	printf("Before fork\n");

	//fork 및 error 처리
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { // child만 Var=7,var=89 
		/* child */
		Var++;
		var++;
	}
	else  { //parent 는 값 변경 없고 기다림
		/* parent */
		sleep(2);
	}
	// parent 와 child 각각 다른 값 출력
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	pid_t	pid;
	char	*argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	// execl
	if ((pid = fork()) < 0)  { // fork하여 child process에서 실행
		perror("fork");
		exit(1);
	} 
	else if (pid == 0)  { // 해당 경로의 mycp 를 실행하여 복사, 리스트사용
		// mycp exec.c exec2.c
		if (execl("/home/lsp34/assignment3/mycp", argv[0], argv[1], "exec1.c", NULL))  {
			perror("execl");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  { // 실행 완료까지 기다림
		perror("waitpid");
		exit(1);
	}

	// execv
	if ((pid = fork()) < 0)  { // 위와 같은 내용, 벡터 사용
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// mycp exec.c exec2.c
		if (execv("/home/lsp34/assignment3/mycp", argv))  {
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}

	// execlp
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// execute 'env' from PATH
		if (execlp("env", "env", NULL))  { 
		//경로 따로 안넣어줘도 PATH에 저장된 것으로 실행, 2번째는 argv[0]을 위한거
			perror("execlp");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}
}

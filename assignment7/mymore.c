#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

int 
main() 
{
	int fd[2];
	pid_t child;

	if(pipe(fd) < 0)  { // pipe 생성
		perror("pipe");
		exit(1);
	}
 
	child = fork();
	if(child == (pid_t)(-1))  {
        perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) { // child process
        close(1); // stdout을 close
		close(fd[0]); // child에서 읽기 안함. close
        	
        if(dup(fd[1]) == -1)  { // 비어있는 1번 stdout이 fd[1] 가 가르키는 파일과 같은 곳을 가르킴 ( 결국 stdout은 되지 않음)
        	perror("dup");
			exit(1);
		}

       	if((execlp("ls", "ls", "-l", NULL)) == -1)  {// ls -l 실행 시 stdout을 출력 안되고 fd[1]에 출력됨.
       		perror("execlp");
			exit(1);
		}
	} 
	else { // parent
        close(0); // stdin을 close하고 fd[0]을 통해 받아올 것임
		close(fd[1]); // parent 에서 읽기만 할 것이므로 닫음.
        	
       	if((dup2(fd[0], 0)) == 1)  { // 자동으로 비어있는 0으로 할당되지만 dup2를 이용해 명시적으로 설정 가능, stdin이 fd[0]에서 들어오는 값으로 대체
       		perror("dup");
			exit(1);
		}

        if((execlp("more", "more", NULL)) == -1)  { // more 실행
       		perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}

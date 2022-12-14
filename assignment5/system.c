#include <stdio.h>
#include <stdlib.h>

main()
{
	int		status;
	//date 명령어 실행-> 실제로는 새로운 프로세스에서 sh로 실행
	if ((status = system("date")) < 0)  { 
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
	// nosuchcommand 실행, 실제로는 sh에서 에러를 출력 , 없는 명령어
	if ((status = system("nosuchcommand")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status); // error 반환
	
	/* $ who; exit 44 */
	//who 실행 후 exit 44코드로 반환 실제로는 256이 곱해진 값이 반환
	if ((status = system("who; exit 44")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}

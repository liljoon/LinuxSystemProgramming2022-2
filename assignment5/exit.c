#include <stdio.h>
#include <stdlib.h>

void
myexit1()
{
	printf("first exit handler\n");
}

void
myexit2()
{
	printf("second exit handler\n");
}

main()
{
	/* Add myexit2 to exit functions */
	if (atexit(myexit2) != 0)  { // myexit2 함수 exit handler에 추가
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  { // myexit1 함수 추가
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  { // myexit1 함수 추가
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");
	//종료 후 등록 역순으로 작동 (1, 2, 2)
}

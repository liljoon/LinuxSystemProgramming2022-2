#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	// 경로명 없으면 에러
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	//rmdir 실행
	if (rmdir(argv[1]))  {
		perror("rmdir");
		exit(1);
	}
}

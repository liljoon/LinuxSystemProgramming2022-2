#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	// 기존 경로, 새로운 경로 입력 못 받으면 에러
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}
	// 심볼릭 링크
	if (symlink(argv[1], argv[2]) == -1)  {
		perror("link");
		exit(1);
	}
}

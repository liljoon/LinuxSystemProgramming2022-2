#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	// 경로명 없으면 에러
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	/* 755 permission */
	// 0755 권한으로 mkdir 실행
	if (mkdir(argv[1], 0755) == -1)  {
		perror("mkdir");
		exit(1);
	}
}

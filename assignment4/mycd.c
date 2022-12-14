#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	// 이동하고자 하는 경로 없으면 에러
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}
	// chdir
	if (chdir(argv[1]) == -1)  {
		perror("chdir");
		exit(1);
	}
}

// 실행 시 새로운 프로세스를 생성하여 해당 프로세스에서만 경로를 바꾸므로
// 우리가 실행시키는 쉘에서는 경로가 바뀌지 않음.

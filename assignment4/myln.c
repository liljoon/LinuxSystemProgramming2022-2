#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	// 기존 경로, 새로운 경로 가 없으면 에러 출력.
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}
	// 앞에 기존 경로, 뒤에 새로운 경로로 하드링크
	if (link(argv[1], argv[2]) < 0)  {
		perror("link");
		exit(1);
	}
}

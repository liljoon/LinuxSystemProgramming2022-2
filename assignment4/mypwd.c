#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	char	buf[MAX_BUF];

	// 현재 경로 buf 에 저장 최대 MAX_BUF길이 만큼.
	if (getcwd(buf, MAX_BUF) == NULL)  {
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
}

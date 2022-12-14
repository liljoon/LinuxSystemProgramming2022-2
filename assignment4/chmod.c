#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	struct stat	statbuf;

	if (stat("bar", &statbuf) < 0)  { // stat 을 이용하여 정보 가져옴
		perror("stat");
		exit(1);
	}

	// 기존 권한에 group write를 빼고, S_ISUID 특수권한 부여 (실행 시 일시적으로 user권한 가져옴)
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  {
		perror("chmod");
		exit(1);
	}

	/* S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH : ON */
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)  {
		perror("chmod");
		exit(1);
	}
}

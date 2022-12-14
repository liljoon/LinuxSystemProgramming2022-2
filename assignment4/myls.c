#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

main()
{
	DIR				*dp; // opendir 반환값 저장
	struct dirent	*dep; // readdir 순차적으로 반환값 저장

	// 현재 경로를 opendirgkdu dp에 반환값 저장
	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");
		exit(0);
	}
	//dep에 readdir을 계속 호출하여 NULL 나올때까지 반환
	while (dep = readdir(dp))  {
		printf("%s\n", dep->d_name);
	}
	
	closedir(dp); // 닫기
}

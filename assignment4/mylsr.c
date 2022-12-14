#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256

void
JustDoIt(char *path)
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL)  {
		perror("opendir");
		exit(0);
	}
	//현재 경로 출력
	printf("\n%s:\n", path);
	while (dep = readdir(dp))  { // 하나씩 읽으면서 ., .. 은 생략하고 파일, 경로 출력
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
	}
	
	rewinddir(dp); // 다시 처음으로 돌아가서
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name); // fullpath에 경로명 저장
		if (lstat(fullpath, &statbuf) == -1)  { // 만약 directory라면
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))  { // 재귀함수로 그 하위 경로도 전부 출력
			JustDoIt(fullpath);
		}
	}
	
	closedir(dp);
}

main()
{
	JustDoIt(".");
}

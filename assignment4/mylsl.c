#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <grp.h>
#include <time.h>

// permission 각 자리별로 체크해서 출력
void print_permission(mode_t mode)
{
	if (S_ISDIR(mode))
		printf("d");
	else
		printf("-");
	if((mode & S_IRUSR) != 0)
		printf("r");
	else
		printf("-");

	if((mode & S_IWUSR) != 0)
		printf("w");
	else
		printf("-");

	if((mode & S_IXUSR) != 0)
		printf("x");
	else
		printf("-");
	
	if((mode & S_IRGRP) != 0)
		printf("r");
	else
		printf("-");

	if((mode & S_IWGRP) != 0)
		printf("w");
	else
		printf("-");

	if((mode & S_IXGRP) != 0)
		printf("x");
	else
		printf("-");
	if((mode & S_IROTH) != 0)
		printf("r");
	else
		printf("-");

	if((mode & S_IWOTH) != 0)
		printf("w");
	else
		printf("-");

	if((mode & S_IXOTH) != 0)
		printf("x");
	else
		printf("-");

	printf(" " );
	
}

// uid값을 넣으면 실제 user이름 출력
void print_uid(uid_t uid)
{
	struct passwd *pwd;
	pwd = getpwuid(uid);
	printf("%s ", pwd->pw_name);
}

// gid값 넣으면 groupd이름 출력
void print_gid(gid_t gid)
{
	struct group *g;
	g = getgrgid(gid);
	printf("%s ", g->gr_name);
}

// time_t 받으면 적절히 format해서 출력
void print_time(time_t ti)
{
	struct tm *t = localtime(&ti);
	const char * months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	printf("%s %2d %02d:%02d ", months[t->tm_mon], t->tm_mday, t->tm_hour, t->tm_min);
}

int main()
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;

	if ((dp = opendir(".")) == NULL)
	{
		perror("opendir");
		return (-1);
	}
	while (dep = readdir(dp))
	{
		if(strcmp(dep->d_name, ".") == 0 ||  strcmp(dep->d_name,"..") == 0)
			continue;
		if (lstat(dep->d_name, &statbuf) == -1)
		{
			perror("lstat");
			continue;
		}
		print_permission(statbuf.st_mode);
		printf("%ld ", statbuf.st_nlink); // 링크개수
		print_uid(statbuf.st_uid);
		print_gid(statbuf.st_gid);
		printf("%5ld ", statbuf.st_size); // 파일 크기
		print_time(statbuf.st_mtime);
		printf("%s\n", dep->d_name); // 파일 이름
	}
	return 0;
}

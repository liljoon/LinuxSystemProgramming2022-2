#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void print_file(struct dirent *dep)
{
	struct stat		buf;
	if(strcmp(dep->d_name, ".") ==0 || strcmp(dep->d_name,"..")==0)
		pthread_exit(NULL);
	printf("%s ", dep->d_name);
	lstat(dep->d_name, &buf);
	printf("%ld\n", buf.st_size);
	pthread_exit(NULL);
}

main()
{
	DIR				*dp; // opendir 반환값 저장
	struct dirent	*dep; // readdir 순차적으로 반환값 저장
	pthread_t	tid;

	if ((dp = opendir(".")) == NULL)  {
		perror("opendir");
		exit(0);
	}
	while (dep = readdir(dp))  {
		pthread_create(&tid, NULL, print_file, dep);
		//pthread_join(tid,NULL);	
	}
	
	closedir(dp); 
}

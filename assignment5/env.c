#include <stdio.h>

main(int argc, char *argv[], char *envp[])
{
	int			i;
	char		**p;
	extern char	**environ; // c에서 제공하는 환경변수 저장한 전역변수

	printf("List command-line arguments\n"); // argv 출력
	for (i = 0 ; i < argc ; i++)  {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");
#if 1
	for (i = 0 ; environ[i] != NULL ; i++)  { //전역변수 이용하여 출력
		printf("%s\n", environ[i]);
	}
#else
	for (p = environ ; *p != NULL ; p++)  {
		printf("%s\n", *p);
	}
#endif

	printf("\n");
	printf("List environment variables from envp variable\n");
#if 1
	for (i = 0 ; envp[i] != NULL ; i++)  { // main 에 envp 이용 출력
		printf("%s\n", envp[i]);
	}
#else
	for (p = envp ; *p != NULL ; p++)  {
		printf("%s\n", *p);
	}
#endif
}

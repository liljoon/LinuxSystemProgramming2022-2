#include <stdio.h>

main(int argc, char *argv[])
{
	int		i;
	// 하나 이상의 경로는 있어야함.
	if (argc == 1)  {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}
	// 모든 입력 값에 대해서 다 지움
	for (i = 1 ; i < argc ; i++)  {
		if (remove(argv[i]) == -1)  {
			perror("remove");
			exit(1);
		}
	}
}

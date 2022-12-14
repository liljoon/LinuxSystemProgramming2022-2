#include <stdio.h>

#define	MAX_CMD		256


// thread 생성 없이, 실행시 함수가 끝나야 다시 while실행
void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main()
{
	char	cmd[MAX_CMD];

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		DoCmd(cmd);
	}
}

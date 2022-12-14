#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL); // stdout 에 buffer 없앰
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
		putc(c, stdout);
	}
}

main()
{
	pid_t	pid;

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// parent 와 child가 concurrent하게 출력, console이 shared data
	else if (pid == 0)  {
		CharAtaTime("output from child\n");
	} 
	else  {
		CharAtaTime("output from parent\n");
	}
}

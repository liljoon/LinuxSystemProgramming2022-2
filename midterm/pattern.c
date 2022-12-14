#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[1024];

	int cnt = 0;
	if ((fp = fopen(argv[1],"rb")) == NULL)
	{
		perror("fopen");
		exit(1);
	}
	while(fread(buf, sizeof(argv[2]), 1, fp) != 0)
	{
		if (strcmp(buf, argv[2])==0)
		{
			cnt++;
		}
	}
	printf("%d\n", cnt);
}

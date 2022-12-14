#include <stdio.h>

main(int argc, char *argv[])
{
	// old name, newname 없으면 에러
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	// old name, newname 으로 rename
	if (rename(argv[1], argv[2]) == -1)  {
		perror("rename");
		exit(1);
	}
}

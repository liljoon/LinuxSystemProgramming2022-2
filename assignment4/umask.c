#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	umask(0); // 아무것도 umask 하지 않음.
	// -rw-rw-rw 권한으로 생성
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}

	/* Set file mode creation mask: S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH */
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // group read,write other read,write 를 umask
	//umask 때문에 -rw------ 로 생성
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}
}

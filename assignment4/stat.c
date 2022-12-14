#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	struct stat	statbuf;  // 파일 정보 받을 구조체
	char		*mode;
	int			i;

	// 모든 입력값에 대해서 반복하여 출력
	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]); // 현재 파일 명 출력
		if (lstat(argv[i], &statbuf) == -1)  { // lstat 함수 에러처리, 주소이기에 & 빼먹지말기
			perror("lstat");
			continue;
		}
		// 파일 종류 확인 후 출력
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";
		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode); // 파일 정보 및 권한
		printf("\tst_ino = %d\n", statbuf.st_ino); // inode 번호
		printf("\tst_dev = %d\n", statbuf.st_dev); // 저장되어 있는 device 정보
		printf("\tst_rdev = %d\n", statbuf.st_rdev); // 
		printf("\tst_nlink = %d\n", statbuf.st_nlink);
		printf("\tst_uid = %d\n", statbuf.st_uid); // Userid
		printf("\tst_gid = %d\n", statbuf.st_gid); // groud id
		printf("\tst_size = %d\n", statbuf.st_size); // file size in bytes
		printf("\tst_atime = %d\n", statbuf.st_atime); // last access time
		printf("\tst_mtime = %d\n", statbuf.st_mtime); // last modification time
		printf("\tst_ctime = %d\n", statbuf.st_ctime); // status chage time
		printf("\tst_blksize = %d\n", statbuf.st_blksize); // best i/o block size
		printf("\tst_blocks = %d\n", statbuf.st_blocks); // block 개수
	}
}

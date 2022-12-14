#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// mmap을 사용하면 매번 read, write를 하지 않으므로 성능이 우수
// kernel 과 disk가 계속 처리하고 마지막에만 application에 확인
int main(int argc, char *argv[])
{
	int			fdin, fdout;
	char		*src, *dst;
	struct stat	statbuf;

	// argument 개수 에러 처리
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// 원본 파일 open
	if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	// 사본 파일 open 및 생성
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	// 원본 파일 정보 가져오기
	if (fstat(fdin, &statbuf) < 0)  {
		perror("fstat");
		exit(1);
	}
	//원본 파일 길이-1 만큼 커서 움직임
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1); //마지막 널 문자 작성하여 dummy 파일 생성

	// 0으로 시스템이 정한 주소에, 원본 길이만큼 할당, 읽기만 허용, 옵션은 다른 모든 프로세스 와 공유 가능, fdin을 mapping, offset은 파일 처음부터 전부 할당
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	// 사본 파일에 쓰기 가능으로 동일하게 mapping
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); // 메모리를 복사한느 것으로 파일 복사와 동일하게 동작

	close(fdin);
	close(fdout);
}

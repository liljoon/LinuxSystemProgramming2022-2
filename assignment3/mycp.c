#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];
	
	//실행시 뒤에 붙는 파일이 2개가 아닐 경우 에러 출력 후 종료
	if (argc != 3)  {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	//첫 파일 오픈 시 에러발생하면 에러 출력 후 종료
	// 읽기 모드로 오
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	// 두번째 파일 오픈 에러 발생시 에러 출력 후 종료
	// 쓰기 모드, 없으면 생성, 기존에 있으면 다 지움
	// 생성시 권한은 0644
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	// fd1의 처음부터 끝까지 fd2로 복사
	while (count = read(fd1, buf, MAX_BUF))  {
		write(fd2, buf, count);
	}
	// 파일 닫기
	close(fd1);
	close(fd2);
}

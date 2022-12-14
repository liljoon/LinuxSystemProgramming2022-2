#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int 	fd;
	char	ch = '\0';

	// file.hole 이라는 파일을 0400권한으로 생성 및 에러처리
	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");
		exit(1);
	}
	// file 내부에서의 커서를 처음부터 1023 떨어진만큼으로 이동 및 에러처리
	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	// 1023 번째 칸에 ch 문자 쓰기
	write(fd, &ch, 1);
	//파일 닫기
	close(fd);
}
// 1024bytes dummy file 생성

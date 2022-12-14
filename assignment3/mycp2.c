#include <stdio.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];
	int		count;
	// 입력 파일경로가 2개가 아니면 에러 출력 및 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// 첫번째 파일 오픈 및 에러 처리
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// 두번째 파일 오픈 및 에러 처리
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// fread를 통해 크기가 1 만큼 MAX_BUF의 길이를 src에서 읽어와서 fwrite를 통해 dst에 쓴다.
	while (count = fread(buf, 1, MAX_BUF, src))  {
		fwrite(buf, 1, count, dst);
	}
	//파일 닫기
	fclose(src);
	fclose(dst);
}

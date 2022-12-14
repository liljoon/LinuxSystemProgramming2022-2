#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	int		ch;

	// 입력파일이 2개가 아니면 에러출력 및 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// 첫 번째 파일 오픈 및 에러처리
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// 두번재 파일 오픈 및 에러처리
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// Loop를 통해 fgetc로 src의 문자를 dst로 넣음.
	while ((ch = fgetc(src)) != -1)  {
		fputc(ch, dst);
	}
	//파일 닫기
	fclose(src);
	fclose(dst);
}

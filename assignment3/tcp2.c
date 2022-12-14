#include <stdio.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];
	// 입력파일이 2개가 아니면 에러 처리 및 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	//첫번째 파일 오픈 및 에러처리
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// 두번째 파일 오픈 및 에러처리
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// fgets를 이용해 buf에 MAX_BUF - 1만큼 또는 \n나올때 까지 넣고 마지막엔 널문자 삽입, src에서 읽어옴.
	while (fgets(buf, MAX_BUF, src))  {
		fputs(buf, dst);
	}
	//파일 닫기
	fclose(src);
	fclose(dst);
}

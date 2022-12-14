#include <stdio.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE	*src1, *src2, *dst;
	char	buf[MAX_BUF];
	int		count;
	// argument 3개 아니면 에러처리
	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	// src1, src2, dst 파일 오픈 및 에러 처리
	if ((src1 = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((src2 = fopen(argv[2], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// src1 을 dst로 복사
	while (count = fread(buf, 1, MAX_BUF, src1))  {
		fwrite(buf, 1, count, dst);
	}
	// src2 를 dst로 복사
	while (count = fread(buf, 1, MAX_BUF, src2))  {
		fwrite(buf, 1, count, dst);
	}
	// 파일 닫기
	fclose(src1);
	fclose(src2);
	fclose(dst);
}

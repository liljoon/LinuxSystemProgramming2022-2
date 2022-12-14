#include <stdio.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	FILE	*fp;
	char	buf[MAX_BUF];
	int		line;

	// 실행 파일 뒤에 붙는 파일이 하나가 아닐 시 에러 
	if (argc != 2)  {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}
	// 파일을 읽기 및 텍스트 파일모드로 오픈 및 에러처리
	if ((fp = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	// 한 라인씩 읽고 출력하는데 앞에 line번호와 여백을 두고 출력 
	line = 1;
	while (fgets(buf, MAX_BUF, fp))  {
		printf("%4d: %s", line++, buf);
	}

	fclose(fp);
}

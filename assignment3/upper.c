#include <stdio.h>

void upper(char *filename)
{
	FILE	*fp;
	char	ch;

	// 파일 읽기모드로 오픈 및 에러 처리
	if ((fp = fopen(filename, "rt")) == NULL){
		perror("fopen");
		exit(1);
	}
	// 문자 하나씩 fgetc를 통해 읽고, 만약 소문자일 경우에만 아스키코드 차이로 연산
	// 그리고 putc를 통해 출력
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch >= 'a' && ch <= 'z')
			ch += 'A' - 'a';
		putc(ch, stdout);
	}
	//파일 닫기
	fclose(fp);
	return;
}

int main(int argc, char *argv[])
{
	// 모든 arguments에 대해서 출력
	while (--argc)
		upper(argv[argc]);
}

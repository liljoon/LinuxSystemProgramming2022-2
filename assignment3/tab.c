#include <stdio.h>

enum { FALSE, TRUE };

void
conv(char *fname)
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;
	
	// fname 경로의 파일을 오픈
	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}
	
	// 기존의 파일명에 .t를 붙여서 파일 생성
	strcpy(fdest, fname);
	strcat(fdest, ".t");
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	}
	
	// 각 줄의 처음에 시작되는 탭인지 체크하기 위한 변수(중간에 들어가는 탭은 그대로 탭)
	first = TRUE;
	while ((ch = fgetc(src)) != EOF)  {
		// 각 줄의 시작에 해당하는 탭일경우 공백 4칸으로 대체
		if (first && ch == '\t')  {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else  {
			fputc(ch, dst); // 탭이 아니면 그대로 복사
			if (first) // 문자가 시작되었으면 더 이상 처음이 아니라고 체크
				first = FALSE;
			if (ch == '\n') // 개행문자를 만났으면 다시 줄의 처음이므로 체크
				first = TRUE;
		}
	}
	//파일 닫기
	fclose(src);
	fclose(dst);
}

main(int argc, char *argv[])
{
	while (--argc)  { // 들어오는 파일 모두다 실행.
		conv(argv[argc]);
	}
}

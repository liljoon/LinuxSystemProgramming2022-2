#include <stdio.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	Record	rec;
	// dst, src 두개가 아니면 에러 반환 및 종료
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}
	// src,dst 파일 오픈 및 에러처리
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[2], "wb")) == NULL)  { // binary로 엶
		perror("fopen");
		exit(1);
	}
	//src파일을 읽어서 각각 이름, 번호, 전공으로 저장
	while (fgets(rec.stud, SMAX, src))  {
		*strchr(rec.stud, '\n') = '\0'; // strchr로 '\n'을 찾아 '\0'으로 대체.
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';

		fwrite(&rec, sizeof(rec), 1, dst); // Record struct 크기 그대로 fwrite로 dst binary파일로 저장.
	}
	//파일 닫기
	fclose(src);
	fclose(dst);
}

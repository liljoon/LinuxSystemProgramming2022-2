#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024

//ftell을 통해 현재 위치 임시 저장후 fseek을 통해 맨 뒤의 길이를 반환
long
filesize(FILE *fp)
{
	long	cur, size;

	cur = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, cur, SEEK_SET);
	return(size);
}

main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;
	char	buf[MAX_BUF];
	int		count, size;
	// argument가 3개가 아니면 에러 처리 및 종료
	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}
	// 파일 3개 오픈 및 에러처리
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	//src의 파일 길이를 반으로 나눔.
	size = filesize(src) / 2;
	//size만큼 dst1에 저장
	while (size > 0)  {
		count = (size > MAX_BUF) ?  MAX_BUF : size; // MAX_BUF와 size중 작은 크기 만큼 count저장
		fread(buf, 1, count, src); 
		fwrite(buf, 1, count, dst1);
		size -= count; // 읽은 만큼 size에서 제거
	}
	//남은 파일 내용 전부 dst2에 저장
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf, 1, count, dst2);
	}
	//파일 닫기
	fclose(src);
	fclose(dst1);
	fclose(dst2);
}

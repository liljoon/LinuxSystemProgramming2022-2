#include <stdio.h>
#include <unistd.h>
#include "record.h"

// Record struct member 전부 출력.
void
print_record(Record *rp)
{
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

main(int argc, char *argv[])
{
	FILE	*fp;
	Record	rec;
	
	// binary file 하나만 argument해야함
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}
	// binary 파일 오픈, 읽기 쓰기 binary 모드
	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");
	// binary 파일 rec크기 만큼 하나씩 읽어서 출력
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}
	rewind(fp); // 파일 맨 처음으로 이동
	getchar(); // 프로그램 일시정지용 입력하면 밑에 실행

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // 파일 처음부터 해당 숫자에 위치한 만큼 움직여 읽고 출력
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	
	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar(); // 일시정지

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");
	fseek(fp, sizeof(rec)*(-1L), SEEK_CUR);
	fwrite(&rec, sizeof(rec), 1, fp);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	//update 후 전부 출력
	rewind(fp);
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}
	//파일 닫기
	fclose(fp);
}

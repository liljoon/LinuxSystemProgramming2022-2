#include <stdio.h>

main(int argc, char *argv[])
{
	int		i;

	for (i = 0 ; i < argc ; i++)
		printf("argv[%d]=%s\n", i, argv[i]);
}

// main의 파라미터로 argc는 실행파일 뒤에 붙는 arguments + 1 개를 의미한다.
// argv는 문자열의 배열로 argc만큼 문자열이 있고 처음에는 사용자가 쉘에서 실행한 실행파일 경로이고
// 그 뒤부터는 그 뒤에 붙인 argument 들이 순서대로 온다.

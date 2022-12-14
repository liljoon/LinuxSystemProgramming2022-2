#include <stdio.h>

main()
{
	char c, s[80];
	int i; long l;
	float f; double d;
	
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); 
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d); 

	c = getchar();
	putchar(c);

	gets(s);
	puts(s);
}

// scanf 함수를 통해 여러가지 자료형을 입력받고 출력시킨다.
// getchar함수로 버퍼에 있는 문자 한글자를 가져온다.
// gets 함수를 통해 \0로 끝나는 string 을 s에 저장시킨다.
// puts 함수를 통해 s의 string을 출력시킨다.

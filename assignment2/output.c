#include <stdio.h>

main()
{
	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;
	
	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p); 
	putchar(c);
	puts(s);
}

// c에서의 여러가지 자료형의 변수를 선언하여 printf를 통해 출력한다.
// putchar는 char형 변수 하나만 출력시키는 함수이고, puts는 string을 출력시키는 함수이다.

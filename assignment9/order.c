#include <stdio.h>

// big-endian인지 little-endian 인지 확인
// server에서 intel cpu 사용중이어서 little-endian으로 출력
void main()
{
	int		a = 0x12345678;
	char	*p = (char *)&a;

#if 1
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
#else
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
#endif
}

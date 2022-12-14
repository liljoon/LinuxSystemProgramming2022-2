#include <stdio.h>

// string을 포인터 형태로 받아서 길이를 반환하는 함수이다. '\0'를 만나기전까지 Loop 한다.
int
strlen_p(char *str)
{
	int	len = 0;

	while (*str++)  {
		len++;
	}
	return len;
}

// string 을 배열 형태로 받아서 길이를 반환한다.
int
strlen_a(char str[])
{
	int i;

	for (i = 0 ; str[i] != '\0' ; i++)
		;
	return i;
}


// dst라는 string을 포인터로 받아서 src의 내용으로 끝까지 복사한다.
void
strcpy_p(char *dst, char *src)
{
	while (*src)  {
		*dst++ = *src++;
	}
	*dst = *src;
}

// dst string을 배열형식으로 받아서 src의 내용을 끝까지 복사한다.
void
strcpy_a(char dst[], char src[])
{
	int i;

	for (i = 0 ; src[i] != '\0'; i++)
		dst[i] = src[i];
	dst[i] = src[i];
}

// dst string을 포인터로 받아서 '\0' 문자부터 src의 처음을 덮어씌우면서 string을 합친다.
void
strcat_p(char *dst, char *src)
{
	while (*dst++)
		;
	dst--;
	while (*src)  {
		*dst++ = *src++;
	}
	*dst = *src;
}

// dst string을 배열로 받아서 '\0' 문자부터 src의 처음을 덮어씌우면서 string을 합친다.
void
strcat_a(char dst[], char src[])
{
	int i, j;

	for (i = 0 ; dst[i] != '\0' ; i++)
		;
	for (j = 0 ; src[j] != '\0' ; j++)
		dst[i+j] = src[j];
	dst[i+j] = src[j];
}

// dst 와 src string이 같은지 확인하는 함수이다. 같으면 계속 Loop를 돌다가 동시에 끝나면
// 0을 반환하고 같다는 의미이고, 하나라도 다르면 해당 문자의 아스키코드 차이 값을 출력한다.
int strcmp_p(char *dst, char *src)
{
	while (*dst == *src)
	{
		if(*dst == '\0')
			return 0;
		dst++;
		src++;
	}
	return (*dst - *src);
}

// dst와 src를 배열로 받아와서 위와 동일한 일을 수행한다.
int strcmp_a(char dst[], char src[])
{
	int i = 0;
	while (dst[i] == src[i])
	{
		if(dst[i] == '\0')
			return 0;
		i++;
	}
	return (dst[i] - src[i]);
}

//str1 과 str2 를 20개로 여유있게 공간을 할당한다.
// "Hello" 문자열의 길이를 출력하고, str1,str2에 복사하여 출력한다.
// ", World!"를 기존의 str1,str2에 뒤에 이어 붙여서 출력한다.
// 같은 문자열인 str1,str2를 비교하여 0을 출력하고
// 느낌표하나가 더 붙어있으므로 아스키코드 차이인 33을 출력한다.
main()
{
	int		len1, len2;
	char	str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);

	printf("strcmp: p=%d, a=%d\n", strcmp_p(str1, str2), strcmp_a(str1, str2));
	printf("strcmp: p=%d, a=%d\n", strcmp_p("Hello!", "Hello"), strcmp_a("Hello!", "Hello"));
}

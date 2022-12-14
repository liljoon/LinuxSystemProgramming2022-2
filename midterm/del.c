#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void del(char *str, char c)
{
	char str2[100];
	int i = 0;

	for(int j=0;j<strlen(str);j++)
	{
		if(*(str+j) != c)
		{
			*(str2+i) = *(str+j);
			i++;
		}
	}
	*(str2+i) = '\0';
	strcpy(str,str2);
}

int main()
{
	char str[20] = "Hello";
	del(str, 'l');
	puts(str);
}

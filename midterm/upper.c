#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
void upper(char *filename, char *filename2)
{
	FILE	*fp;
	FILE	*fp2;
	char	ch;
	int 	cnt = 0;
	int 	cnt2 = 0;
	int		cnt3 = 0;
	int		cnt4 = 0;
	int		cnt5 = 0;

	if ((fp = fopen(filename, "rt")) == NULL){
		perror("fopen");
		exit(1);
	}
	
	if ((fp2 = fopen(filename2, "wt")) == NULL){
		perror("fopen");
		exit(1);
	}
	while ((ch = fgetc(fp)) != EOF)
	{
		if (isprint(ch))
			cnt++;
		if (ch >= 'a' && ch <= 'z')
		{
			ch += 'A' - 'a';
			cnt2++;
		}
		else if(ch == ' ')
		{
			cnt3++;
		}
		else if(isdigit(ch))
		{
			cnt4++;
		}
		if(!isalpha(ch) && !isdigit(ch) && !isspace(ch))
			cnt5++;
		fputc(ch, fp2);

	}
	fclose(fp);
	fclose(fp2);
	printf("Converted file: %s\n", filename2);
	printf("Total characters: %d\n", cnt);
	printf("Converted characters: %d\n", cnt2);
	printf("Blank characters: %d\n", cnt3);
	printf("Number characters: %d\n", cnt4);
	printf("Special characters: %d\n", cnt5);
	return;
}

int main(int argc, char *argv[])
{
	upper(argv[1], argv[2]);
}

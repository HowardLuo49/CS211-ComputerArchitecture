#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, char **argv)
{
	if(argc > 1 && argv[1][0] != '\0')
	{
		int i = 0;
		while(argv[1][i] != '\0')
		{
			char temp = argv[1][i];
			if(islower(temp))
			{
				printf("%c", toupper(temp));
			}
			else
			{
				printf("%c", temp);
			}
			i++;
		}
		if(i != 0)
		{
			printf("!!\n");
		}
	}
}

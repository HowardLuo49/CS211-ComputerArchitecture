#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int main (int argc, char **argv)
{
	bool error = false;
	for(int i = 0; i < strlen(argv[1]); i++)
	{
		if(isdigit(argv[1][i]) && !error)
		{
			error = true;
			printf("error");
		}
	}
	if(!error)
	{
		int length = 0;
		for(int i = 1; i < strlen(argv[1]); i++)
		{
			if(argv[1][i - 1] != argv[1][i])
			{
				length++;
			}
		}
		if(2 * (length + 1) > strlen(argv[1]))
		{
			puts(argv[1]);
		}
		else
		{
			int n = strlen(argv[1]);
			if(!error && n != 0)
			{
				char *box = (char *)malloc(sizeof(char) * 2 * n);
				char *storage;
				storage = box;
				for(int i = 0; i < n; i++)
				{
					int counter = 1;
					for(int j = i; argv[1][j] == argv[1][j+1]; j++)
					{
						counter++;
					}
					storage += sprintf(storage, "%c%d", argv[1][i], counter);
					i += counter - 1;
				}
				if(strlen(box) > strlen(argv[1]))
				{
					puts(argv[1]);
				}
				else
				{
					puts(box);
				}
				free(box);
			}
		}
	}
}

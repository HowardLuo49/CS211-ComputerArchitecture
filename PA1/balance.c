#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv)
{
	char *first;
	int n = strlen(argv[1]);
	first = (char *)malloc(sizeof(char) * (n+1));
	char *pointer;
	pointer = first;

	int openCount = 0;
	for(int i = 0; i < n; i++)
	{
		if(argv[1][i] == '(' || argv[1][i] == '[' || argv[1][i] == '{')
		{
			openCount++;
			if(*pointer != '\0')
			{
				pointer++;
			}
			*pointer = argv[1][i];
		}
		else if(argv[1][i] == ')' || argv[1][i] == ']' || argv[1][i] == '}')
		{
			openCount--;
			if(openCount < 0)
			{
				printf("%d: %c", i, argv[1][i]);
				exit(EXIT_FAILURE);
			}
			if((argv[1][i] == ')' && *pointer == '(') || (argv[1][i] == ']' && *pointer == '[') || (argv[1][i] == '}' && *pointer == '{'))
			{
				*pointer = '\0';
				pointer--;
			}
			else
			{
				printf("%d: %c", i, argv[1][i]);
				exit(EXIT_FAILURE);
			}
		}
	}
	if(pointer == first)
	{
		exit(EXIT_SUCCESS);
	}
	else
	{
		printf("open: ");
		while (pointer != first)
		{
			if(*pointer == '(')
			{
				printf(")");
			}
			else if(*pointer == '[')
			{
				printf("]");
			}
			else if(*pointer == '{')
			{
				printf("}");
			}
			pointer --;
		}
		exit(EXIT_FAILURE);
	}
	free(pointer);
}

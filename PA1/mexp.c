#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
	FILE *f;
	int dimension[1];

	f = fopen(argv[1], "r");
	fscanf(f,"%d", dimension);
	
	int dim = dimension[0];

	int *given;
	given = (int *)malloc(sizeof(int) * (dim*dim));

	for(int i = 0; i < (dim * dim); i++)
	{
		fscanf(f, "%d", &given[i]);
	}

	int powerExp[1];
	fscanf(f,"%d", powerExp);
	int power = powerExp[0];

	int *result;
	result = (int *)malloc(sizeof(int) * (dim * dim));

	if(power == 0)
	{
		int positions = dim + 1;
		for(int i = 0; i < dim; i++)
		{
			result[positions * i] = 1;
		}
	}
	else
	{
		for(int i = 0; i < (dim * dim); i++)
		{
			result[i] = given[i];
		}
		
		for(int a = 0; a < power - 1; a++)
		{
			int *temporary;
			temporary = (int *)malloc(sizeof(int) * (dim * dim));
			for(int i = 0; i < (dim * dim); i++)
			{
				temporary[i] = result[i];
			}

			for(int i = 0; i < dim; i++)
			{
				for(int j = 0; j < dim; j++)
				{
					int sum = 0;
					for(int k = 0; k < dim; k++)
					{
						sum += temporary[i * dim + k] * given[j + dim * k];
					}
					result[i * dim + j] = sum;
				}
			}
			free(temporary);
		}
	}

	//printing
	for(int i = 0; i < dim; i++)
	{
		for(int j = 0; j < dim; j++)
		{
			printf("%d", result[dim * i + j]);
			if(j + 1 != dim)
			{
				printf(" ");
			}
		}
		printf("\n");
	}


	free(given);
	free(result);
	fclose(f);
}

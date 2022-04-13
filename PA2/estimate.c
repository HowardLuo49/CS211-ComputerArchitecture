#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** gaussJordan (double** matrix, double** result, int dim);
double** productOperation (double** matrixA, double** matrixB, double** result, int rDimA, int middleDim, int cDimB);
double** transpose (double** matrix, double** result, int rDim, int cDim);
void printMatrix(double** matrix, int row, int col);
void freeMatrix(double** matrix, int dim);

int main (int argc, char **argv)
{
	//check for train and data error
	FILE *fT;
	char a[6];
	fT = fopen(argv[1], "r");
	fscanf(fT, "%s", a);
	int comparison1 = strcmp(a, "data");
	if(comparison1 == 0)
	{
		printf("Error in file order\n");
		exit(1);
	}
	
	FILE *fD;
	char b[6];
	fD = fopen(argv[2], "r");
	fscanf(fD, "%s", b);
	int comparison2 = strcmp(b, "train");
	if(comparison2 == 0)
	{
		printf("Error in file order\n");
		exit(1);
	}

	//read # attributes
	int attributes[1];
	int attributesCheck[1];
	fscanf(fT, "%d", attributes);
	fscanf(fD, "%d", attributesCheck);

	if(attributes[0] != attributesCheck[0])
	{
		printf("error");
		exit(1);
	}

	//read # houses
	int houses1[1];
	int houses2[1];
	fscanf(fT, "%d", houses1);
	fscanf(fD, "%d", houses2);

	//read matrix in train
	double **train = malloc(houses1[0] * sizeof(double*));
	for(int i = 0; i < houses1[0]; i++)
	{
		train[i] = malloc((attributes[0] + 1) * sizeof(double));
	}
	double **train2 = malloc(houses1[0] * sizeof(double*));
	for(int i = 0; i < houses1[0]; i++)
	{
		train2[i] = malloc(sizeof(double));
	}

	for(int i = 0; i < houses1[0]; i++)
	{
		for(int j = 0; j < attributes[0] + 1; j++)
		{
			if(j == attributes[0])
			{
				fscanf(fT, "%lf", &train2[i][0]);
				train[i][0] = 1;
			}
			else
			{
				fscanf(fT, "%lf", &train[i][j + 1]);
			}
		}
	}

	//read matrix in data
	double **data = malloc(houses1[0] * sizeof(double*));
	for(int i = 0; i < houses2[0]; i++)
	{
		data[i] = malloc((attributes[0] + 1) * sizeof(double));
	}
	for(int i = 0; i < houses2[0]; i++)
	{
		for(int j = 0; j < attributes[0] + 1; j++)
		{
			if(j == 0)
			{
				data[i][j] = 1;
			}
			else
			{
				fscanf(fD, "%lf", &data[i][j]);
			}
			//fscanf(fD, "%lf", &data[i][j]);
		}
	}

	//transposing matrix
	double **transposed = malloc((attributes[0] + 1) * sizeof(double*));
	for(int i = 0; i < (attributes[0] + 1); i++)
	{
		transposed[i] = malloc(houses1[0] * sizeof(double));
	}
	transpose(train, transposed, houses1[0], attributes[0] + 1);

	//multiplying matrix XT & X
	double **product = malloc(houses1[0] * sizeof(double*));
	for(int i = 0; i < houses1[0]; i++)
	{
		product[i] = malloc(houses1[0] * sizeof(double*));
	}
	product = productOperation(transposed, train, product, attributes[0] + 1, houses1[0], attributes[0] + 1);

	//Inverting XT * X
	double **inverse = malloc((attributes[0] + 1) * sizeof(double*));
	for(int i = 0; i < (attributes[0] + 1); i++)
	{
		inverse[i] = malloc((attributes[0] + 1) * sizeof(double*));
	}
	for(int i = 0; i < (attributes[0] + 1); i++)
	{
		for(int j = 0; j < (attributes[0] + 1); j++)
		{
			if(i == j)
			{
				inverse[i][j] = 1;
			}
			else
			{
				inverse[i][j] = 0;
			}
		}
	}
	inverse = gaussJordan(product, inverse, attributes[0] + 1);

	//multiplying matrix (XT*X)-1 & XT
	double **product2 = malloc((attributes[0] + 1) * sizeof(double*));
	for(int i = 0; i < (attributes[0] + 1); i++)
	{
		product2[i] = malloc(houses1[0] * sizeof(double*));
	}
	product2 = productOperation(inverse, transposed, product2, attributes[0] + 1, attributes[0] + 1, houses1[0]);

	//multiplying matrix (XT*X)-1*XT & Y
	double **product3 = malloc((attributes[0] + 1) * sizeof(double*));
	for(int i = 0; i < (attributes[0] + 1); i++)
	{
		product3[i] = malloc(1 * sizeof(double*));
	}
	product3 = productOperation(product2, train2, product3, attributes[0] + 1, houses1[0], 1);

	for(int i = 0; i < houses2[0]; i++)
	{
		double tempSum = 0;
		for(int j = 0; j < attributes[0] + 1; j++)
		{
			tempSum += data[i][j] * product3[j][0];
		}
		printf("%.0f\n", tempSum);
	}

	//freeing everything
	freeMatrix(inverse, (attributes[0] + 1));
	freeMatrix(product, houses1[0]);
	freeMatrix(product2, (attributes[0] + 1));
	freeMatrix(product3, (attributes[0] + 1));
	freeMatrix(transposed, (attributes[0] + 1));
	freeMatrix(train2, houses1[0]);
	freeMatrix(train, houses1[0]);
	freeMatrix(data, houses2[0]);

	exit(0);
}

double** gaussJordan (double** matrix, double** result, int dim)
{
	for(int i = 0; i < dim; i++)
	{
		double head = matrix[i][i];
		if(head != 0)
		{
			for(int j = 0; j < dim; j++)
			{
				if(matrix[i][j] != 0)
					matrix[i][j] /= head;
				if(result[i][j] != 0)
					result[i][j] /= head;
			}
		}
		for(int j = i + 1; j < dim; j++)
		{
			double head2 = matrix[j][i];
			for(int k = 0; k < dim; k++)
			{
				matrix[j][k] -= (head2 * matrix[i][k]);
				result[j][k] -= (head2 * result[i][k]);
			}
		}
	}
	for(int i = dim - 1; i >= 0; i--)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			double head2 = matrix[j][i];
			for(int k = dim - 1; k >= 0; k--)
			{
				matrix[j][k] -= (head2 * matrix[i][k]);
				result[j][k] -= (head2 * result[i][k]);
			}
		}
	}

	return result;
}

double** productOperation (double** matrixA, double** matrixB, double** result, int rDimA, int middleDim, int cDimB)
{
	for(int i = 0; i < rDimA; i++)
	{
		for(int j = 0; j < cDimB; j++)
		{
			double temp = 0;
			for(int k = 0; k < middleDim; k++)
			{
				temp += matrixA[i][k] * matrixB[k][j];
			}
			result[i][j] = temp;
		}
	}
	return result;
}

double** transpose (double** matrix, double** result, int rDim, int cDim)
{
	for(int i = 0; i < rDim; i++)
	{
		for(int j = 0; j < cDim; j++)
		{
			result[j][i] = matrix[i][j];
		}
	}

	return result;
}

void printMatrix(double** matrix, int row, int col)
{
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			printf("%f", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void freeMatrix(double** matrix, int dim)
{
	for(int i = 0; i < dim; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

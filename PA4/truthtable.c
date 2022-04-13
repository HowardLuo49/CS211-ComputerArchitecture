#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include <stdbool.h>

struct node{
	int val;
	int varNum;
	char* varName;
	struct node* next;
};

struct gate{
	int type;
	int input1;
	int input2;
	int moreInputs[100];
	int moreSelectors[100];
	int output1;
	int moreOutputs[100];
	struct gate* next;
};

void freer(struct node* head);
void freer2 (struct gate* head);
int applyValues(struct node* nodeList, char* inputs, char* values, int QuantityLength);
void evaluate(int time, struct gate* gateList, struct node* nodeList);
void multiplexer (struct gate* gate, struct node* nodeList);
void decoder(int time, struct gate* gate, struct node* nodeList);
void xor(int input1, int input2, int output1, struct node* nodeList);
void not(int input1, int output1, struct node* nodeList);
void or(int input1, int input2, int output1, struct node* nodeList);
void and(int input1, int input2, int output1, struct node* nodeList);
void printSolution(struct node* nodeList, char* outputs, int outputQuantity);
void printInputs(struct node* nodeList, char* inputs, int inputQuantity);
void pass(int input1, int output1, struct node* nodeList);
int binaryToInt(int binary);

int main(int argc, char** argv)
{
	FILE *f;
	f = fopen(argv[1], "r");
	if(f == NULL)
	{
		printf("File Error\n");
		exit(1);
	}

	//reading inputs
	char* inputWord = malloc(5*sizeof(char) + 1);
	fscanf(f, "%s", inputWord);
	free(inputWord);

	int inputQuantity;
	fscanf(f, "%d", &inputQuantity);

	char* inputs = malloc((inputQuantity * sizeof(int)));

	char* reader = malloc(7*(inputQuantity * sizeof(char)) + 1);
	char* readerHead = reader;
	struct node* head = (struct node*) malloc(sizeof(struct node));
	struct node* pointer = head;
	fscanf(f, "%s", reader);
	int lastNum = 1;
	inputs[0] = lastNum;
	head -> varNum = lastNum;
	head -> varName = reader;
	head -> next = NULL;
	lastNum++;
	for(int i = 0; i < inputQuantity - 1; i++)
	{
		reader += 7;
		fscanf(f, "%s", reader);
		inputs[i + 1] = lastNum;
		struct node* temp = (struct node*) malloc(sizeof(struct node));
		temp -> varNum = lastNum;
		temp -> varName = reader;
		temp -> next = NULL;
		pointer -> next = temp;
		pointer = pointer -> next;
		lastNum++;
	}

	//reading outputs
	char* outputWord = malloc(6*sizeof(char) + 1);
	fscanf(f, "%s", outputWord);
	free(outputWord);

	int outputQuantity;
	fscanf(f, "%d", &outputQuantity);

	char* outputs = malloc((outputQuantity * sizeof(int)));

	char* reader2 = malloc(7*(outputQuantity * sizeof(int)) + 1);
	char* readerHead2 = reader2;
	fscanf(f, "%s", reader2);
	outputs[0] = lastNum;
	struct node* temp = (struct node*) malloc(sizeof(struct node));
	temp -> varNum = lastNum;
	temp -> varName = reader2;
	temp -> next = NULL;
	pointer -> next = temp;
	pointer = pointer -> next;
	lastNum++;
	for(int i = 0; i < outputQuantity - 1; i++)
	{
		reader2 += 7;
		fscanf(f, "%s", reader2);
		outputs[i + 1] = lastNum;
		struct node* temp = (struct node*) malloc(sizeof(struct node));
		temp -> varNum = lastNum;
		temp -> varName = reader2;
		temp -> next = NULL;
		pointer -> next = temp;
		pointer = pointer -> next;
		lastNum++;
	}

	//reading gates
	bool completed = false;
	bool firstGate = true;
	struct gate* gateHead;
	struct gate* gatePointer;
	
	struct node* newNodePointer = head;
	while(newNodePointer -> next != NULL)
	{
		newNodePointer = newNodePointer -> next;
	}

	char* output1 = malloc(100*(outputQuantity * sizeof(char)) + 1);
	char* output1Head = output1;
	char* singleOutputs = malloc(500 + (outputQuantity * sizeof(char)) + 1);
	char* singleOutputsHead = singleOutputs;
	char* singleOutputsM = malloc(500 + (outputQuantity * sizeof(char)) + 1);
	char* singleOutputsMHead = singleOutputsM;

	while(!completed)
	{
		char gateType[12];
		char input1[10];
		char input2[10];
		//char output1[10];
		int read = fscanf(f, "%s", gateType);
		//printf("[read: %d]\n", read);
		if(read != -1)
		{
			int inputCount = 0;
			struct gate* temp = (struct gate*) malloc(sizeof(struct gate));
			if(strcmp(gateType, "AND") == 0)
			{
				temp -> type = 1;
				inputCount = 2;
			}
			else if(strcmp(gateType, "OR") == 0)
			{
				temp -> type = 2;
				inputCount = 2;
			}
			else if(strcmp(gateType, "NOT") == 0)
			{
				temp -> type = 3;
				inputCount = 1;
			}
			else if(strcmp(gateType, "NAND") == 0)
			{
				temp -> type = 4;
				inputCount = 2;
			}
			else if(strcmp(gateType, "NOR") == 0)
			{
				temp -> type = 5;
				inputCount = 2;
			}
			else if(strcmp(gateType, "XOR") == 0)
			{
				temp -> type = 6;
				inputCount = 2;
			}
			else if(strcmp(gateType, "PASS") == 0)
			{
				temp -> type = 7;
				inputCount = 1;
			}
			else if(strcmp(gateType, "DECODER") == 0)
			{
				temp -> type = 8;
				inputCount = 0;
			}
			else if(strcmp(gateType, "MULTIPLEXER") == 0)
			{
				temp -> type = 9;
				inputCount = -1;
			}
			if(inputCount == -1)
			{
				int indexMoreInputsM = 0;
				int selectorCount;
				char* manyRegularInputs = malloc(100*(inputQuantity * sizeof(char)) + 1);
				char* manyRegularInputsHead = manyRegularInputs;
				char* manySelectorInputs = malloc(100*(inputQuantity * sizeof(char)) + 1);
				char* manySelectorInputsHead = manySelectorInputs;
				fscanf(f, "%d", &selectorCount);
				//int regularInputCount = pow(2, selectorCount);
				int regularInputCount = 1 << selectorCount;
				for(int i = 0; i < regularInputCount; i++)
				{
					fscanf(f, "%s", manyRegularInputs);
					if(strcmp(manyRegularInputs, "0") == 0)
					{
						temp -> moreInputs[i] = -2;
					}
					if(strcmp(manyRegularInputs, "1") == 0)
					{
						temp -> moreInputs[i] = -1;
					}
					struct node* ifExisting = head;
					while(ifExisting != NULL)
					{
						if(strcmp(ifExisting -> varName, manyRegularInputs) == 0)
						{
							temp -> moreInputs[i] = ifExisting -> varNum;
						}
						ifExisting = ifExisting -> next;
					}
					manyRegularInputs += 7;
				}
				for(int i = 0; i < selectorCount; i++)
				{
					fscanf(f, "%s", manySelectorInputs);
					struct node* ifExisting = head;
					while(ifExisting != NULL)
					{
						if(strcmp(ifExisting -> varName, manySelectorInputs) == 0)
						{
							temp -> moreSelectors[i] = ifExisting -> varNum;
						}
						ifExisting = ifExisting -> next;
					}
					manySelectorInputs += 7;
				}

				bool outputFound = false;
				fscanf(f, "%s", singleOutputsM);
				struct node* ifExisting = head;
				while(ifExisting != NULL)
				{
					if(strcmp(ifExisting -> varName, singleOutputsM) == 0)
					{
						temp -> moreOutputs[indexMoreInputsM] = ifExisting -> varNum;
						indexMoreInputsM++;
						outputFound = true;
					}
					ifExisting = ifExisting -> next;
				}
				if(!outputFound)
				{
					struct node* newResult = (struct node*) malloc(sizeof(struct node));
					newResult -> varNum = lastNum;
					newResult -> varName = singleOutputsM;
					newResult -> next = NULL;
					newNodePointer -> next = newResult;

					temp -> moreOutputs[indexMoreInputsM] = lastNum;

					indexMoreInputsM++;
					newNodePointer = newNodePointer -> next;
					lastNum++;
					singleOutputsM += 7;
				}

				temp -> next = NULL;
				free(manyRegularInputsHead);
				free(manySelectorInputsHead);
			}
			if(inputCount == 0)
			{
				int indexMoreInputs = 0;
				int* quantityInputs = malloc(sizeof(int));
				char* manyInputs = malloc(100*(inputQuantity * sizeof(char)) + 1);
				char* manyInputsHead = manyInputs;
				fscanf(f, "%d", quantityInputs);
				//printf("{Quantity inputs %d}\n", *quantityInputs);
				for(int i = 0; i < *quantityInputs; i++)
				{
					fscanf(f, "%s", manyInputs);
					struct node* ifExisting = head;
					while(ifExisting != NULL)
					{
						if(strcmp(ifExisting -> varName, manyInputs) == 0)
						{
							//printf("ifExisting->varNum: %d\n", (ifExisting->varNum));
							temp -> moreInputs[i] = ifExisting -> varNum;
						}
						ifExisting = ifExisting -> next;
					}
					manyInputs += 7;
				}
				//int quantityOutputs = pow(2, *quantityInputs);
				int quantityOutputs = 1 << *quantityInputs;
				for(int i = 0; i < quantityOutputs; i++)
				{
					//char singleOutput[100];
					bool outputFound = false;
					fscanf(f, "%s", singleOutputs);
					struct node* ifExisting = head;
					while(ifExisting != NULL)
					{
						if(strcmp(ifExisting -> varName, singleOutputs) == 0)
						{
							//printf("FOUND\n");
							temp -> moreOutputs[indexMoreInputs] = ifExisting -> varNum;
							indexMoreInputs++;
							outputFound = true;
						}
						ifExisting = ifExisting -> next;
					}
					if(!outputFound)
					{
						struct node* newResult = (struct node*) malloc(sizeof(struct node));
						newResult -> varNum = lastNum;
						newResult -> varName = singleOutputs;
						newResult -> next = NULL;
						newNodePointer -> next = newResult;

						temp -> moreOutputs[indexMoreInputs] = lastNum;
						indexMoreInputs++;

						newNodePointer = newNodePointer -> next;
						lastNum++;
						singleOutputs += 7;
					}
				}
				temp -> next = NULL;
				free(manyInputsHead);
				free(quantityInputs);
			}
			if(inputCount == 2)
			{
				fscanf(f, "%s", input1);
				fscanf(f, "%s", input2);
				fscanf(f, "%s", output1);

				bool outputFound = false;
				struct node* ifExisting = head;
				if(strcmp(input1, "1") == 0)
				{
					temp -> input1 = -1;
				}
				if(strcmp(input1, "0") == 0)
				{
					temp -> input1 = -2;
				}
				if(strcmp(input2, "1") == 0)
				{
					temp -> input2 = -1;
				}
				if(strcmp(input2, "0") == 0)
				{
					temp -> input2 = -2;
				}
				while(ifExisting != NULL)
				{
					if(strcmp(ifExisting -> varName, input1) == 0)
					{
						temp -> input1 = ifExisting -> varNum;
					}
					if(strcmp(ifExisting -> varName, input2) == 0)
					{
						temp -> input2 = ifExisting -> varNum;
					}
					if(strcmp(ifExisting -> varName, output1) == 0)
					{
						temp -> output1 = ifExisting -> varNum;
						outputFound = true;
					}
					ifExisting = ifExisting -> next;
				}
				temp -> next = NULL;
				if(!outputFound)
				{
					temp -> output1 = lastNum;
					struct node* newResult = (struct node*) malloc(sizeof(struct node));
					newResult -> varNum = lastNum;
					newResult -> varName = output1;
					newResult -> next = NULL;
					newNodePointer -> next = newResult;
					newNodePointer = newNodePointer -> next;
					lastNum++;
				}
			}
			if(inputCount == 1)
			{
				fscanf(f, "%s", input1);
				fscanf(f, "%s", output1);

				bool outputFound = false;
				struct node* ifExisting = head;
				if(strcmp(input1, "1") == 0)
				{
					temp -> input1 = -1;
				}
				if(strcmp(input1, "0") == 0)
				{
					temp -> input1 = -2;
				}
				while(ifExisting != NULL)
				{
					if(strcmp(ifExisting -> varName, input1) == 0)
					{
						temp -> input1 = ifExisting -> varNum;
					}
					if(strcmp(ifExisting -> varName, output1) == 0)
					{
						temp -> output1 = ifExisting -> varNum;
						outputFound = true;
					}
					ifExisting = ifExisting -> next;
				}
				temp -> next = NULL;
				if(!outputFound)
				{
					temp -> output1 = lastNum;
					struct node* newResult = (struct node*) malloc(sizeof(struct node));
					newResult -> varNum = lastNum;
					newResult -> varName = output1;
					newResult -> next = NULL;
					newNodePointer -> next = newResult;
					newNodePointer = newNodePointer -> next;
					lastNum++;
				}
			}
			if(firstGate)
			{
				gateHead = temp;
				gatePointer = gateHead;
				firstGate = false;
			}
			else
			{
				gatePointer -> next = temp;
				gatePointer = gatePointer -> next;
			}
			output1 += 7;
		}
		else
		{
			completed = true;
		}
	}

	char* values = malloc((inputQuantity * sizeof(int)));
	int time = 0;
	for(int i = 0; i < 1 << inputQuantity; i++)
	{
		int count = 0;
		for(int j = inputQuantity - 1; j >= 0; j--)
		{
			values[count] = ((1<<j) & i) > 0;
			count++;
		}
		
		applyValues(head, inputs, values, inputQuantity);
		printInputs(head, inputs, inputQuantity);
		evaluate(time, gateHead, head);
		printSolution(head, outputs, outputQuantity);
		time++;
	}

	free(singleOutputsMHead);
	free(singleOutputsHead);
	free(output1Head);
	free(outputs);
	free(values);
	free(inputs);
	freer2(gateHead);
	freer(head);
	free(readerHead);
	free(readerHead2);
	fclose(f);
}

void evaluate(int time, struct gate* gateList, struct node* nodeList)
{
	struct gate* gatePointer = gateList;
	while(gatePointer != NULL)
	{
		if(gatePointer -> type == 1)
		{
			and(gatePointer -> input1, gatePointer -> input2, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 2)
		{
			or(gatePointer -> input1, gatePointer -> input2, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 3)
		{
			not(gatePointer -> input1, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 4)
		{
			and(gatePointer -> input1, gatePointer -> input2, gatePointer -> output1, nodeList);
			not(gatePointer -> output1, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 5)
		{
			or(gatePointer -> input1, gatePointer -> input2, gatePointer -> output1, nodeList);
			not(gatePointer -> output1, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 6)
		{
			xor(gatePointer -> input1, gatePointer -> input2, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 7)
		{
			pass(gatePointer -> input1, gatePointer -> output1, nodeList);
		}
		else if(gatePointer -> type == 8)
		{
			decoder(time, gatePointer, nodeList);
		}
		else if(gatePointer -> type == 9)
		{
			multiplexer(gatePointer, nodeList);
		}
		gatePointer = gatePointer -> next;
	}
}

void multiplexer (struct gate* gate, struct node* nodeList)
{
	struct gate* gatePointer = gate;
	int regularInputCount = 0;
	int selectorCount = 0;
	while(gatePointer -> moreInputs[regularInputCount] >= 0)
	{
		regularInputCount++;
	}
	while(gatePointer -> moreSelectors[selectorCount] >= 0)
	{
		selectorCount++;
	}

	int binary = 0;
	for(int i = 0; i < selectorCount; i++)
	{
		binary *= 10;
		bool found = false;
		struct node* nodePointer = nodeList;
		while(!found && nodePointer != NULL)
		{
			if(gate -> moreSelectors[i] == -1)
			{
				binary += 1;
				found = true;
			}
			else if(gate -> moreSelectors[i] == -2)
			{
				binary += 0;
				found = true;	
			}
			else if(nodePointer -> varNum == gate -> moreSelectors[i])
			{
				binary += nodePointer -> val;
				found = true;
			}
			nodePointer = nodePointer -> next;
		}
	}

	int decimal = binaryToInt(binary);

	int solution = gate -> moreInputs[decimal];

	if(solution == -1)
	{
		solution = 1;
	}
	else if(solution == -2)
	{
		solution = 0;
	}
	else
	{
		struct node* finalNodePointer = nodeList;
		bool found = false;
		while(!found && finalNodePointer != NULL)
		{
			if(finalNodePointer -> varNum == solution)
			{
				solution = finalNodePointer -> val;
				found = true;
			}
			finalNodePointer = finalNodePointer -> next;
		}
	}

	struct node* nodePointer2 = nodeList;
	bool found = false;
	while(!found && nodePointer2 != NULL)
	{
		if(nodePointer2 -> varNum == gate -> moreOutputs[0])
		{
			nodePointer2 -> val = solution;
			found = true;
		}
		nodePointer2 = nodePointer2 -> next;
	}
}

void decoder(int time, struct gate* gate, struct node* nodeList)
{
	struct gate* gatePointer = gate;
	int inputCount = 0;
	while(gatePointer -> moreInputs[inputCount] >= 0)
	{
		inputCount++;
	}
	//int outputCount = pow(2, inputCount);
	int outputCount = 1 << inputCount;

	int binary = 0;
	struct node* nodePointer = nodeList;
	for(int i = 0; i < inputCount; i++)
	{
		binary *= 10;
		bool found = false;
		while(!found && nodePointer != NULL)
		{
			if(nodePointer -> varNum == gate -> moreInputs[i])
			{
				binary += nodePointer -> val;
				found = true;
			}
			nodePointer = nodePointer -> next;
		}
	}
	int decimal = binaryToInt(binary);

	for(int i = 0; i < outputCount; i++)
	{
		struct node* nodePointer = nodeList;
		while(nodePointer != NULL)
		{
			if(nodePointer -> varNum == gate -> moreOutputs[i])
			{
				if(decimal == i)
				{
					nodePointer -> val = 1;
				}
				else
				{
					nodePointer -> val = 0;
				}
			}
			nodePointer = nodePointer -> next;
		}
	}

}

int binaryToInt(int binary)
{
	int decimal = 0;
	int i = 0;
	int rem;
	int holder = binary;

	while(holder != 0)
	{
		rem = holder%10;
		holder/=10;
		//decimal+=rem*pow(2,i);
		decimal += rem * (1 << i);
		i++;
	}
	return decimal;
}

void pass(int input1, int output1, struct node* nodeList)
{
	struct node* nodePointer = nodeList;
	int temp = 0;
	while(nodePointer != NULL)
	{
		if(nodePointer -> varNum == input1)
		{
			temp = nodePointer -> val;
		}
		nodePointer = nodePointer -> next;
	}
	struct node* nodePointerFinalize = nodeList;
	while(nodePointerFinalize != NULL)
	{
		if(nodePointerFinalize -> varNum == output1)
		{
			nodePointerFinalize -> val = temp;
			return;
		}
		nodePointerFinalize = nodePointerFinalize -> next;
	}
}

void xor(int input1, int input2, int output1, struct node* nodeList)
{
	struct node* nodePointer = nodeList;
	int val1;
	int val2;
	if(input1 == -1)
	{
		val1 = 1;
	}
	if(input1 == -2)
	{
		val1 = 0;
	}
	if(input2 == -1)
	{
		val2 = 1;
	}
	if(input2 == -2)
	{
		val2 = 0;
	}
	else
	{
		while(nodePointer != NULL)
		{
			if(nodePointer -> varNum == input1)
			{
				val1 = nodePointer -> val;
			}
			else if(nodePointer -> varNum == input2)
			{
				val2 = nodePointer -> val;
			}
			nodePointer = nodePointer -> next;
		}
		struct node* nodePointerFinalize = nodeList;
		while(nodePointerFinalize != NULL)
		{
			if(nodePointerFinalize -> varNum == output1)
			{
				if((val1 == 0 && val2 == 1) || (val1 == 1 && val2 == 0))
				{
					nodePointerFinalize -> val = 1;
				}
				else
				{
					nodePointerFinalize -> val = 0;
				}
				return;
			}
			nodePointerFinalize = nodePointerFinalize -> next;
		}
	}
}

void not(int input1, int output1, struct node* nodeList)
{
	struct node* nodePointer = nodeList;
	int temp = 0;
	while(nodePointer != NULL)
	{
		if(nodePointer -> varNum == input1)
		{
			temp = nodePointer -> val;
		}
		nodePointer = nodePointer -> next;
	}
	if(temp == 0)
	{
		temp = 1;
	}
	else
	{
		temp = 0;
	}
	struct node* nodePointerFinalize = nodeList;
	while(nodePointerFinalize != NULL)
	{
		if(nodePointerFinalize -> varNum == output1)
		{
			nodePointerFinalize -> val = temp;
			return;
		}
		nodePointerFinalize = nodePointerFinalize -> next;
	}
}

void or(int input1, int input2, int output1, struct node* nodeList)
{
	struct node* nodePointer = nodeList;
	int val1;
	int val2;
	if(input1 == -1)
	{
		val1 = 1;
	}
	if(input1 == -2)
	{
		val1 = 0;
	}
	if(input2 == -1)
	{
		val2 = 1;
	}
	if(input2 == -2)
	{
		val2 = 0;
	}
	while(nodePointer != NULL)
	{
		if(nodePointer -> varNum == input1)
		{
			val1 = nodePointer -> val;
		}
		else if(nodePointer -> varNum == input2)
		{
			val2 = nodePointer -> val;
		}
		nodePointer = nodePointer -> next;
	}
	struct node* nodePointerFinalize = nodeList;
	while(nodePointerFinalize != NULL)
	{
		if(nodePointerFinalize -> varNum == output1)
		{
			nodePointerFinalize -> val = val1|val2;
			return;
		}
		nodePointerFinalize = nodePointerFinalize -> next;
	}
}

void and(int input1, int input2, int output1, struct node* nodeList)
{
	struct node* nodePointer = nodeList;
	int val1;
	int val2;
	if(input1 == -1)
	{
		val1 = 1;
	}
	if(input1 == -2)
	{
		val1 = 0;
	}
	if(input2 == -1)
	{
		val2 = 1;
	}
	if(input2 == -2)
	{
		val2 = 0;
	}
	while(nodePointer != NULL)
	{
		if(nodePointer -> varNum == input1)
		{
			val1 = nodePointer -> val;
		}
		else if(nodePointer -> varNum == input2)
		{
			val2 = nodePointer -> val;
		}
		nodePointer = nodePointer -> next;
	}
	struct node* nodePointerFinalize = nodeList;
	while(nodePointerFinalize != NULL)
	{
		if(nodePointerFinalize -> varNum == output1)
		{
			nodePointerFinalize -> val = val1&val2;
			return;
		}
		nodePointerFinalize = nodePointerFinalize -> next;
	}
}

int applyValues(struct node* nodeList, char* inputs, char* values, int QuantityLength)
{
	struct node* pointer = nodeList;
	for(int i = 0; i < QuantityLength; i++)
	{
		pointer = nodeList;
		bool changed = false;
		while((pointer -> next != NULL) && changed == false)
		{
			if((pointer -> varNum - (int) inputs[i]) == 0)
			{
				pointer -> val = values[i];
				changed = true;
			}
			pointer = pointer -> next;
		}
	}
	return 0;
}

void printInputs(struct node* nodeList, char* inputs, int inputQuantity)
{
	struct node* pointer = nodeList;
	for(int i = 0; i < inputQuantity; i++)
	{
		pointer = nodeList;
		bool printed = false;
		while((pointer -> next != NULL) && printed == false)
		{
			if((pointer -> varNum - (int) inputs[i]) == 0)
			{
				printf("%d ", pointer -> val);
				printed = true;
			}
			pointer = pointer -> next;
		}
	}
}

void printSolution(struct node* nodeList, char* outputs, int outputQuantity)
{
	struct node* pointer = nodeList;
	printf("|");
	for(int i = 0; i < outputQuantity; i++)
	{
		pointer = nodeList;
		bool printed = false;
		while((pointer != NULL) && printed == false)
		{
			if((pointer -> varNum - (int) outputs[i]) == 0)
			{
				printf(" %d", pointer -> val);
				printed = true;
			}
			pointer = pointer -> next;
		}
	}
	printf("\n");
}

void freer (struct node* head)
{
	if(head == NULL)
	{
		return;	
	}
	if(head -> next != NULL)
	{
		freer(head -> next);
	}
	free(head);
}

void freer2 (struct gate* head)
{
	if(head == NULL)
	{
		return;	
	}
	if(head -> next != NULL)
	{
		freer2(head -> next);
	}
	free(head);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node
{
	int data;
	struct Node* left;
	struct Node* right;
};

struct Node* insert (struct Node* head, struct Node* inputData)
{
	struct Node* pointer = head;
	struct Node* previous = pointer;

	if(head == NULL)
	{
		head = inputData;
		printf("inserted");
		return inputData;
	}
	else
	{
		bool inserted = false;
		while(!inserted)
		{
			 if(inputData -> data < pointer -> data)
			 {
			 	previous = pointer;
			 	pointer = pointer -> left;
			 	if(pointer == NULL)
				{
					previous -> left = inputData;
					printf("inserted");
					inserted = true;
				}
			 }
			 else if(inputData -> data > pointer -> data)
			 {
			 	previous = pointer;
			 	pointer = pointer -> right;
				if(pointer == NULL)
				{
				 	previous -> right = inputData;
				 	printf("inserted");
				 	inserted = true;
				}
			 }
			 else
			 {
			 	printf("not inserted");
			 	free(inputData);
			 	inserted = true;
			 }
		}
		return head;
	}
}

bool search (struct Node* head, int n)
{
	if(head == NULL)
	{
		return false;
	}
	else if(head -> data == n)
	{
		return true;
	}
	else if(n > head -> data)
	{
		return search(head -> right, n);
	}
	else
	{
		return search(head -> left, n);
	}
}

void print (struct Node* head)
{
	if(head != NULL)
	{
		printf("(");
		if((head -> left) != NULL)
		{
			print(head -> left);
		}
		printf("%d", head -> data);
		if((head -> right) != NULL)
		{
			print(head -> right);
		}
		printf(")");
	}
}

struct Node* delete (struct Node* head, int deleteNode)
{
	struct Node* pointer = head;
	struct Node* previous = pointer;
	struct Node* previous2 = pointer;
	struct Node* previous3 = pointer;
	int lastTraversal = -1;

	if(search(head, deleteNode))
	{
		printf("deleted");
	}
	else
	{
		printf("absent");
		return head;
	}

	while((pointer -> data) != deleteNode)
	{
		if(deleteNode < (pointer -> data))
		{
			lastTraversal = 0;
			previous = pointer;
			pointer = pointer -> left;
		}
		else
		{
			lastTraversal = 1;
			previous = pointer;
			pointer = pointer -> right;
		}
	}

	if((pointer -> left) == NULL && (pointer -> right) == NULL)
	{
		if(lastTraversal == 0)
		{
			free(pointer);
			previous -> left = NULL;
			return head;
		}
		else if(lastTraversal == 1)
		{
			free(pointer);
			previous -> right = NULL;
			return head;
		}
		else if(lastTraversal == -1)
		{
			free(head);
			// head -> data = 0;
			// head -> right = NULL;
			// head -> left = NULL;
			return NULL;
		}
	}
	else if((pointer -> left) == NULL)
	{
		if(lastTraversal == 0)
		{
			previous -> left = pointer -> right;
			free(pointer);
			return head;
		}
		else if(lastTraversal == 1)
		{
			previous -> right = pointer -> right;
			free(pointer);
			return head;
		}
		else
		{
			head = pointer -> right;
			free(pointer);
			return head;
		}
	}
	else if((pointer -> right) == NULL)
	{
		if(lastTraversal == 0)
		{
			previous -> left = pointer -> left;
			free(pointer);
			return head;
		}
		else if(lastTraversal == 1)
		{
			previous -> right = pointer -> left;
			free(pointer);
			return head;
		}
		else
		{
			head = pointer -> left;
			free(pointer);
			return head;
		}
	}
	else
	{
		previous2 = pointer;
		previous3 = pointer;
		pointer = pointer -> left;
		int counter = 0;
		while((pointer -> right) != NULL)
		{
			previous3 = pointer;
			pointer = pointer -> right;
			counter++;
		}
		if(counter == 0)
		{
			if (lastTraversal == 1)
			{
				previous2 -> left = pointer -> left;
				previous2 -> data = pointer -> data;
				free(pointer);
			}
			else if (lastTraversal == 0)
			{
				previous2 -> left = pointer -> left;
				previous2 -> data = pointer -> data;
				free(pointer);
			}
			else if(lastTraversal == -1)
			{
				previous -> data = pointer -> data;
				previous -> left = pointer -> left;
				pointer -> left = NULL;
				free(pointer);
			}
		} 
		else 
		{
			if(pointer -> left != NULL)
			{
				previous3 -> right = pointer -> left;
				pointer -> left = NULL;
			}
			else
			{
				previous3 -> right = NULL;
			}
			previous2 -> data = pointer -> data;
			free(pointer);
		}
	}
	return head;
}

void freer (struct Node* head)
{
	if(head == NULL)
	{
		return;	
	}
	if(head -> left != NULL)
	{
		freer(head -> left);
	}
	if(head -> right != NULL)
	{
		freer(head -> right);
	}
	free(head);
}

int main (int argc, char **argv)
{
	char input1[2];
	int input2[1];

	scanf("%c", input1);
	scanf("%d", input2);

	struct Node *head = NULL;

	bool completed = false;

	while(!completed)
	{
		if(input1[0] == 'i')
		{
			struct Node *sampleNode = (struct Node*)malloc(sizeof(struct Node));
			(sampleNode -> data) = input2[0];
			(sampleNode -> left) = NULL;
			(sampleNode -> right) = NULL;

			head = insert(head, sampleNode);
		}
		else if(input1[0] == 's')
		{
			if(search(head, input2[0]))
			{
				printf("present");
			}
			else
			{
				printf("absent");
			}
		}
		else if(input1[0] == 'p')
		{
			print(head);
		}
		else if(input1[0] == 'd')
		{
			head = delete(head, input2[0]);
		}
		printf("\n");

		int num = scanf("%s", input1);
		if(num == 1)
		{
			if(input1[0] != 'p')
			{
				scanf("%d", input2);
			}
		}
		else
		{
			completed = true;
		}
	}
	freer(head);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node
{
	int data;
	struct Node *next;
};

int main (int argc, char **argv)
{
	char input1[2];
	int input2[1];

	scanf("%c", input1);
	scanf("%d", input2);

	struct Node* head = NULL;
	struct Node* current = head;
	struct Node* previous = head;

	int length = 0;

	bool completed = false;

	while(!completed)
	{
		struct Node* product = (struct Node*)malloc(sizeof(struct Node));
		(product -> data) = input2[0];
		(product -> next) = NULL;

		if(input1[0] == 'i')
		{
			//inserting
			length++;
			current = head;
			previous = head;
			if(head == NULL)
				head = product;
			else
			{
				while(current != NULL && (current -> data) < input2[0])
				{
					previous = current;
					current = current -> next;
				}
				if(current == NULL && previous != NULL)
					previous -> next = product;
				else if (current -> data == input2[0])
				{
					length --;
					free(product);
				}
				else
				{
					if(current == previous)
					{
						product -> next = head;
						head = product;
					}
					else
					{
						previous -> next = product;
						product -> next = current;
					}
				}
			}
			//inserting complete
		}
		else if(input1[0] == 'd')
		{
			//deleting
			length--;
			current = head;
			previous = head;
			while(current != NULL && current -> data != product -> data)
			{
				previous = current;
				current = current -> next;
			}
			if(current == NULL)
			{
				free(current);
				length++;
			}
			else if(current == head)
			{
				head = head -> next;
				free(current);
			}
			else
			{
				previous -> next = current -> next;
				free(current);
			}
			free(product);
			//deleting complete
		}

		//printing
		printf("%d :", length);
		for(struct Node* counterPointer = head; counterPointer != NULL; counterPointer = counterPointer -> next)
		{
			printf(" %d", counterPointer -> data);
		}
		printf("\n");

		// if(scanf("%s", input1) != input1[0])
		// {
		// 	int num = scanf("%d", input2);
		// 	printf("(%d, %c, %d)", input2[0], input1[0], num);
			
		// 	if(num != 1)
		// 	{
		// 		completed = true;
		// 	}
		// }

		int num = scanf("%s", input1);
		if(num == 1)
		{
			scanf("%d", input2);
		}
		else
		{
			completed = true;
		}
	}

	struct Node* freer;
	while(head != NULL)
	{
		freer = head;
		head = head -> next;
		free(freer);
	}
}

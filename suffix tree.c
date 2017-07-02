// A C Program to implement Ukkonen's Suffix Tree Construction
// Building generalized suffix tree for two strings
// And Finding Longest Common Substring of the two input strings
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256

struct SuffixTreeNode {
	struct SuffixTreeNode *children[MAX_CHAR];
	struct SuffixTreeNode *suffixLink;
	int start;
	int *end;
	int suffixIndex;
};

typedef struct SuffixTreeNode Node;

char text[100]; //Input string
Node *root = NULL; //Pointer to root node
Node *lastNewNode = NULL;
Node *activeNode = NULL;
int activeEdge = -1;
int activeLength = 0;
int remainingSuffixCount = 0;
int leafEnd = -1;
int *rootEnd = NULL;
int *splitEnd = NULL;
int size = -1; //Length of input string
int size1 = 0; //Size of 1st string

Node *newNode(int start, int *end)
{
	Node *node =(Node*) malloc(sizeof(Node));
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		node->children[i] = NULL;
	node->suffixLink = root;
	node->start = start;
	node->end = end;
	node->suffixIndex = -1;
	return node;
}

int edgeLength(Node *n) {
	if(n == root)
		return 0;
	return *(n->end) - (n->start) + 1;
}

int walkDown(Node *currNode)
{
	/*activePoint change for walk down (APCFWD) using
	Skip/Count Trick (Trick 1). If activeLength is greater
	than current edge length, set next internal node as
	activeNode and adjust activeEdge and activeLength
	accordingly to represent same activePoint*/
	if (activeLength >= edgeLength(currNode))
	{
		activeEdge += edgeLength(currNode);
		activeLength -= edgeLength(currNode);
		activeNode = currNode;
		return 1;
	}
	return 0;
}

void extendSuffixTree(int pos)
{
	leafEnd = pos;
	remainingSuffixCount++;
	lastNewNode = NULL;
	while(remainingSuffixCount > 0) {

		if (activeLength == 0)
			activeEdge = pos;
		if (activeNode->children[text[activeEdge]] == NULL)
		{
			activeNode->children[text[activeEdge]] = newNode(pos, &leafEnd);
			if (lastNewNode != NULL)
			{
				lastNewNode->suffixLink = activeNode;
				lastNewNode = NULL;
			}
		}
		else
		{
			Node *next = activeNode->children[text[activeEdge]];
			if (walkDown(next))
				continue;
			if (text[next->start + activeLength] == text[pos])
			{
				if(lastNewNode != NULL && activeNode != root)
				{
					lastNewNode->suffixLink = activeNode;
					lastNewNode = NULL;
				}
				activeLength++;
				break;
			}
			splitEnd = (int*) malloc(sizeof(int));
			*splitEnd = next->start + activeLength - 1;
			Node *split = newNode(next->start, splitEnd);
			activeNode->children[text[activeEdge]] = split;
			split->children[text[pos]] = newNode(pos, &leafEnd);
			next->start += activeLength;
			split->children[text[next->start]] = next;
			if (lastNewNode != NULL)
				lastNewNode->suffixLink = split;
			lastNewNode = split;
		}
		remainingSuffixCount--;
		if (activeNode == root && activeLength > 0)
		{
			activeLength--;
			activeEdge = pos - remainingSuffixCount + 1;
		}
		else if (activeNode != root)
			activeNode = activeNode->suffixLink;
	}
}

void print(int i, int j)
{
	int k;
	for (k=i; k<=j && text[k] != '#'; k++)
		printf("%c", text[k]);
	if(k<=j)
		printf("#");
}
void setSuffixIndexByDFS(Node *n, int labelHeight)
{
	if (n == NULL) return;

	if (n->start != -1)
	{
		//Print the label on edge from parent to current node
		//Uncomment below line to print suffix tree
		//print(n->start, *(n->end));
	}
	int leaf = 1;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->children[i] != NULL)
		{
			//Uncomment below two lines to print suffix index
		// if (leaf == 1 && n->start != -1)
		//	 printf(" [%d]\n", n->suffixIndex);

			//Current node is not a leaf as it has outgoing
			//edges from it.
			leaf = 0;
			setSuffixIndexByDFS(n->children[i], labelHeight +
								edgeLength(n->children[i]));
		}
	}
	if (leaf == 1)
	{
		for(i= n->start; i<= *(n->end); i++)
		{
			if(text[i] == '#')
			{
				n->end = (int*) malloc(sizeof(int));
				*(n->end) = i;
			}
		}
		n->suffixIndex = size - labelHeight;
		//Uncomment below line to print suffix index
	// printf(" [%d]\n", n->suffixIndex);
	}
}

void freeSuffixTreeByPostOrder(Node *n)
{
	if (n == NULL)
		return;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->children[i] != NULL)
		{
			freeSuffixTreeByPostOrder(n->children[i]);
		}
	}
	if (n->suffixIndex == -1)
		free(n->end);
	free(n);
}

/*Build the suffix tree and print the edge labels along with
suffixIndex. suffixIndex for leaf edges will be >= 0 and
for non-leaf edges will be -1*/
void buildSuffixTree()
{
	size = strlen(text);
	int i;
	rootEnd = (int*) malloc(sizeof(int));
	*rootEnd = - 1;

	/*Root is a special node with start and end indices as -1,
	as it has no parent from where an edge comes to root*/
	root = newNode(-1, rootEnd);

	activeNode = root;
	for (i=0; i<size; i++)
		extendSuffixTree(i);
	int labelHeight = 0;
	setSuffixIndexByDFS(root, labelHeight);
}

int doTraversal(Node *n, int labelHeight, int* maxHeight, 
int* substringStartIndex)
{
	if(n == NULL)
	{
		return;
	}
	int i=0;
	int ret = -1;
	if(n->suffixIndex < 0) //If it is internal node
	{
		for (i = 0; i < MAX_CHAR; i++)
		{
			if(n->children[i] != NULL)
			{
				ret = doTraversal(n->children[i], labelHeight + 
					edgeLength(n->children[i]), 
					maxHeight, substringStartIndex);
				
				if(n->suffixIndex == -1)
					n->suffixIndex = ret;
				else if((n->suffixIndex == -2 && ret == -3) ||
					(n->suffixIndex == -3 && ret == -2) || 
					n->suffixIndex == -4)
				{
					n->suffixIndex = -4;//Mark node as XY
					//Keep track of deepest node
					if(*maxHeight < labelHeight)
					{
						*maxHeight = labelHeight;
						*substringStartIndex = *(n->end) - 
							labelHeight + 1;
					}
				}
			}
		}
	}
	else if(n->suffixIndex > -1 && n->suffixIndex < size1)//suffix of X
		return -2;//Mark node as X
	else if(n->suffixIndex >= size1)//suffix of Y
		return -3;//Mark node as Y
	return n->suffixIndex;
}

void getLongestCommonSubstring()
{
	int maxHeight = 0;
	int substringStartIndex = 0;
	doTraversal(root, 0, &maxHeight, &substringStartIndex);
	
	int k;
	for (k=0; k<maxHeight; k++)
		printf("%c", text[k + substringStartIndex]);
	if(k == 0)
		printf("No common substring");
	else
		printf(", of length: %d",maxHeight);
	printf("\n");
}

int main()
{
	printf("Enter the size of string 1 : ");
	scanf("%d",&size1);
	getchar();
	size1++;
	char a[size1];
	char a1[size1];
	printf("Enter string 1 : ");
	gets(a);
	strcpy(a1,a);
	printf("Enter string 2 : ");
	char b[200];
	gets(b);
	strcat(a,"#");
	strcat(a,b);
	strcat(a,"$");
	strcpy(text,a);
	printf("Longest Common Substring in %s and %s is: ",a1,b);
	buildSuffixTree();
	getLongestCommonSubstring();
	freeSuffixTreeByPostOrder(root);
	return 0;
}


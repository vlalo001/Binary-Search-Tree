#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1000

// maximum number of characters to read per line with fgets
#define MAX_READ 128

// our binary search tree struct
struct TreeNode
{
	char *szKey;
	struct TreeNode *Left;
	struct TreeNode *Right;
};

// global counter of duplicates
int iDuplicates = 0;

// creates a single node with its respective keyword
struct TreeNode * CreateSingleNode(struct TreeNode *TempNode, char *szKey)
{
	// store keyword
	TempNode->szKey = szKey;

	// set left and right to nothing
	TempNode->Left = NULL;
	TempNode->Right = NULL;

	return TempNode;
}

// insert a node into the tree based on strcmp
struct TreeNode* InsertNewNode(struct TreeNode *MainNode, char *szKey, int iIsCharacterSensitive)
{
	// If tree is empty, node becomes root
	if (MainNode == NULL)
	{
		//printf("Creating new Node for Key: %s\n", szKey);
		
		// init a temporary node
		struct TreeNode *TempNode = malloc(sizeof(struct TreeNode));
    	return CreateSingleNode(TempNode, szKey);
		free(TempNode);
	}

	else
	{
		// key is less than the main node key, add to left
		if (mystrcmp(szKey, MainNode->szKey, iIsCharacterSensitive) < 0)
		{
			//printf("Comparing: %s to %s\n", szKey, MainNode->szKey);
			//printf("Adding to Left: %s\n", szKey);
		    MainNode->Left = InsertNewNode(MainNode->Left, szKey, iIsCharacterSensitive);
		    return MainNode;
		}

		// key is greater than main node key, add to left
		else if (mystrcmp(szKey, MainNode->szKey, iIsCharacterSensitive) > 0)
		{
			//printf("Adding to Right: %s\n", szKey);
		    MainNode->Right = InsertNewNode(MainNode->Right, szKey, iIsCharacterSensitive);
		    return MainNode;
		}
	
		// In case of duplicate keys, you can use a counter scheme (i.e., using an integer to indicate the number of duplicate keys the node is currently representing)
		// Pay attention to situations that a duplicate is found, in which case, you ought to simply increment the count and remove the new node, instead of inserting the duplicate node.
		else if (mystrcmp(szKey, MainNode->szKey, iIsCharacterSensitive) == 0)
		{
			iDuplicates++;
		    //printf("Duplicate Node Found: %s\n", szKey);
			return MainNode;
		}
	}
}

// recursively prints to stdout OR to a file
void RecursiveInOrder(struct TreeNode* InputNode, FILE *OutputFile, int iPrintToOutputFile)
{
	// if the left subtree isn't NULL, go to it
 	if (InputNode->Left != NULL)
	{
		RecursiveInOrder(InputNode->Left, OutputFile, iPrintToOutputFile);
	}

	// check if the node has a NULL key
	if (InputNode->szKey != NULL)
	{
		// if an output file has been specified, write to it
		if (iPrintToOutputFile)
		{
			fprintf(OutputFile, "%s\n", InputNode->szKey);
		}
		
		// otherwise just print to stdout
		else
		{
 			printf("%s\n", InputNode->szKey);
		}
	}

	// we have found a NULL key
	else
	{
		printf("Error: InputNode->szKey is NULL\n");
	}

	// if the right subtree isn't NULL, go to it
	if (InputNode->Right != NULL)
	{
		RecursiveInOrder(InputNode->Right, OutputFile, iPrintToOutputFile);
	}
}

// function based off of the textbook implementation described on page 89
// modified to perform BOTH case-sensitive AND non-case-sensitive comparisons
// 0 == the strings are equal
// > 0 == str1 > str2
// < 0 == str1 < str2
int mystrcmp(char *str1, char *str2, int iIsCaseSensitive)
{
	// placeholder for the current position in the character array
	int iPos;

	int iTraverse;

	// case-sensitive case, each individual character must be equal
	if (iIsCaseSensitive)
	{
		// compare character-by-character from str1 and str2
		for (iPos = 0; (str1[iPos] == str2[iPos]); iPos++)
		{
			// if we have reached the empty character of the string \0
			if (str1[iPos] == '\0')
			{
				return 0;
			}
		}
	}

	// non-case-sensitive version, each [A-Z] individual character is made lowercase in both strings
	else
	{
		// for all the chars in string 1, if it's [A-Z], make it lowercase
		for (iTraverse = 0; str1[iTraverse]; iTraverse++)
		{
			if (isalpha(str1[iTraverse]))
			{
				str1[iTraverse] = tolower(str1[iTraverse]);
			}
		}

		// for all the chars in string 2, if it's [a-z], make it lowercase
		for (iTraverse = 0; str2[iTraverse]; iTraverse++)
		{
			if (isalpha(str2[iTraverse]))
			{
				str2[iTraverse] = tolower(str2[iTraverse]);
			}
		}

		// now that case has been ignored, just compare them normally
		for (iPos = 0; ((str1[iPos]) == (str2[iPos])); iPos++)
		{
			// if we have reached the empty character of the string \0
			if (str1[iPos] == '\0')
			{
				return 0;
			}
		}
	}

	return str1[iPos] - str2[iPos];
}

// recursively prints the tree in postorder
void RecursivePostOrder(struct TreeNode* InputNode, FILE *Output)
{
	// if the left subtree isn't NULL, go to it
 	if (InputNode->Left != NULL)
	{
		RecursivePostOrder(InputNode->Left, Output);
	}

	// if the right subtree isn't NULL, go to it
	if (InputNode->Right != NULL)
	{
		RecursivePostOrder(InputNode->Right, Output);
	}

	// check if the node has a NULL key
	if (InputNode->szKey != NULL)
	{
		// check if an output file has been specified
		if (Output != NULL)
		{
			// output the postorder traversal to the file
			fprintf(Output, "%s\n", InputNode->szKey);
			
			// free the key
			free(InputNode->szKey);
		}

		// no output file was specified
		else
		{
			// output the postorder traversal to stdout
			printf("%s\n", InputNode->szKey);

			// free the key
			free(InputNode->szKey);	
		}
	}

	// we have found a NULL key
	else
	{
		printf("Error: InputNode->szKey is NULL\n");
	}
}

// returns a sorted Binary Search Tree using a FILE input
struct TreeNode* ConstructBinaryTree(FILE *Input, int iIsCharacterSensitive)
{
	// stores one entire line of data (stops reading at newline '\n')
	char *szLineData = (char *)malloc(sizeof(char) * MAXLINE);

	// Question: what gets stored in the root szKey? the first key that is read from the input file?
	// we can identify the first szKey that is read from the input file by checking when ipostcounter is 0
	int iPosCounter = 0;

	// we will build on this tree instance
	struct TreeNode *MainTree = (struct TreeNode *)malloc(sizeof(struct TreeNode));

	// temporary tree instance for storing various information (key, ect)
	struct TreeNode *TempTree;

	// the szKey pointer in the TreeNode struct will point to these values for string comparison
	// stores a maximum of 128 lines, with each line being a maximum of 128 characters
	char **szTemp;
	int iInitArray;

	// allocate memory for each key
	szTemp = malloc(MAX_READ * sizeof(char *));
	for (iInitArray = 0; iInitArray < MAX_READ; iInitArray++)
	{
		szTemp[iInitArray] = malloc(MAX_READ * sizeof(char));
	}
	
	// The program reads from the input file (or stdin) one line at a time
	while (fgets(szLineData, MAX_READ, Input) != NULL)
	{
		// we dont want to process empty lines
		if (mystrcmp(szLineData, "\n", iIsCharacterSensitive) == 0)
		{
			continue;
		}

		// if they root node's key is null, this must be the first iteration
		if (iPosCounter == 0)
		{
			// store the first line 
			strcat(szTemp[iPosCounter], szLineData);
			MainTree->szKey = szTemp[iPosCounter];
			//printf("\nRoot Node Key = %s\n", MainTree->szKey);
			iPosCounter++;

			// go to the next iteration
			continue;
		}

		iPosCounter++;

		// create a tree node that stores (a copy of) the string
		TempTree = (struct TreeNode *)malloc(sizeof(struct TreeNode));

		// stores temp value of szLineData
		strcat(szTemp[iPosCounter], szLineData);

		// szKey points to szLineData, which contains the key for each iteration
		TempTree->szKey = szTemp[iPosCounter];

		// insert the new key (TempTree->szKey) into the tree as a new node
		InsertNewNode(MainTree, TempTree->szKey, iIsCharacterSensitive);

		// deallocate and dereference the temporary node
		free(TempTree);
		TempTree = NULL;

	} // end while

	// free the data we read in
	free(szLineData);

	return MainTree;
}

// main executable entry point
int main(int argc, char *argv[])
{
	// compare strings case sensitive [1] , or non-case sensisitve [0]
	int iIsCharacterSensitive = 0;

	// do we store results to an output file
	int iIsOutputFilePresent = 0;

	// read from input file [1], or read from standard input [0]
	int iIsInputFilePresent = 0;

	// current character being read
	int iCurrChar = 0;

	// used to obtain the input_file_name/output_file_name arguments
	int iProcessArgs;
	
	// counter used to get position in argv array
	int iCurrPos = 1;

	// input and outfile file names - malloc it
	char *szOutputFileName = (char *)malloc(sizeof(char) * MAX_READ);
	char *szInputFileName = (char *)malloc(sizeof(char) * MAX_READ);

	// input and outfile FILE streams
	FILE *InputFile, *OutputFile;

	// stores the sorted binary search tree
	struct TreeNode *SortedSearchTree = (struct TreeNode *)malloc(sizeof(struct TreeNode));

	// parse command line arguments
	// case-sensitive is optional (-c)
	// output file is mandatory, if (-o) is specified
	while ((iCurrChar = getopt (argc, argv, "c::o:")) != -1)
	{
		switch (iCurrChar)
		{
			case 'c':
				++iCurrPos;
				printf("Enabled Character-sensitive string comparison.\n");
				iIsCharacterSensitive = 1;
				break;

			case 'o':
				++iCurrPos;
				// if a destination file has been specified, the next parameter will be NON-NULL
				// program will output the sorted lines to the given output file
				if (argv[iCurrPos] != 0)
				{
					printf("Enabled writing to output file.\n");
					// the user has specified an output file
					iIsOutputFilePresent = 1;
					// store the output file name in szOutputFileName
					strcat(szOutputFileName, argv[iCurrPos]);
					printf("Output File: %s\n", szOutputFileName);
				}
				// increment again as we will use iCurrPos to check for the existance of an input file
				++iCurrPos;
				break;

			default:
				break;
		}
	}

	// after we've matched all arguments, check the last token for the input file
	// we know that an input file has been specified if the token after argv[iCurrPos] is NULL
	// if it's not NULL, then it means we're given an std input
	if ((argv[iCurrPos] != 0 && argv[iCurrPos + 1] == 0))
	{
		// input file has been specified by user
		iIsInputFilePresent = 1;

		// copy input file name to our local variable
		strcat(szInputFileName, argv[iCurrPos]);
		printf("Target Input File: %s\n", szInputFileName);
	}
	
	// no input file specified, the input will be from the standard input
	else
	{
		argc -= optind;
		argv += optind;

		if (argc == 0)
		{
			// set the input file to be stdin
			InputFile = stdin;
			iIsInputFilePresent = 1;
			printf("Accepted input from Stdin.\n\n");
		}
	}


	// the user has specified an input file
	if (iIsInputFilePresent)
	{
		// only open an input file by its name if were not reading from stdin
		if (argc != 0)
		{
			// open the input file for reading
			InputFile = fopen(szInputFileName, "r");
			if (InputFile == NULL)
			{
				printf("Input file [%s] can't be opened. Terminating.\n", szInputFileName);
				// invalid input file specified, exiting.
				return 0;
			}
		}
		
		// sort the input file into a binary search tree
		// SortedSearchTree stores the resulting tree
		SortedSearchTree = ConstructBinaryTree(InputFile, iIsCharacterSensitive);		
	}

	// the user has specified an output file
	if (iIsOutputFilePresent)
	{
		// attempt to open the target file
		OutputFile = fopen(szOutputFileName, "w");

		// check if the open was successful
		if (OutputFile == NULL)
		{
			printf("Can't open output file %s\n", szOutputFileName);
		}

		// print the inorder traversal to our output file
		fprintf(OutputFile, "%s\n", "InOrder Traversal:\n");
		RecursiveInOrder(SortedSearchTree, OutputFile, iIsOutputFilePresent);
		printf("\nInOrder traversal saved to output file.\n");
		
		fprintf(OutputFile, "%s", "\n");
		fprintf(OutputFile, "%s\n", "PostOrder Traversal:\n");

		// print the postorder traversal to our output file
		// and free the keys for each node
		RecursivePostOrder(SortedSearchTree, OutputFile);

		printf("PostOrder traversal saved to output file.\n");
	}

	// the user has not specified an output file
	else
	{
		// print the tree inorder
		printf("\nPerforming Inorder Traversal:\n");
		
		// case where no output file is specified, and we print the inorder to stdout
		RecursiveInOrder(SortedSearchTree, NULL, iIsOutputFilePresent);

		// print the postorder traversal to stdout
		// and free the keys for each node
		printf("\nPerforming PostOrder Traversal:\n");
		RecursivePostOrder(SortedSearchTree, NULL);
	}

	// print the total amount of duplicates found
	printf("\nTotal Duplicate Nodes Found: %d\n\n\n", iDuplicates);


	// close the input file, if one was specified
	if (iIsInputFilePresent)
	{
		fclose(InputFile);
	}
	
	// close the output file, if one was specified
	if (iIsOutputFilePresent)
	{
		// print the number of duplicates we found
		char *szPrintDuplicates = (char *)malloc(sizeof(char *) * MAX_READ);
		sprintf(szPrintDuplicates, "\nTotal Duplicate Nodes Found: %d", iDuplicates);
		fprintf(OutputFile, "%s\n", szPrintDuplicates);
		fclose(OutputFile);

		// free memory
		free(szPrintDuplicates);
	}

	// free and dereference the sorted tree
	free(SortedSearchTree);
	free(szOutputFileName);
	free(szInputFileName);
	SortedSearchTree = NULL;

	return 0;
}

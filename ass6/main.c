#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "hashMap.h"

void keyPrint(KeyType k)
{
  char *key = (char *)k;
  printf("%s", key);
}
void valPrint(ValueType v)
{
  int val = (int)v;
  printf("%d", val);
}
/*
 the getWord function takes a FILE pointer and returns you a string which was
 the next word in the in the file. words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.

 when there are no more words in the input file this function will return NULL.

 this function will malloc some memory for the char* it returns. it is your job
 to free this memory when you no longer need it.
 */
char* getWord(FILE *file);

int main (int argc, const char * argv[]) {
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	const char* filename;
	struct hashMap *hashTable;
	int tableSize = 1000;
	clock_t timer;
	FILE *fileptr;
	int numberOfWords = 0;
    /*
     this part is using command line arguments, you can use them if you wish
     but it is not required. DO NOT remove this code though, we will use it for
     testing your program.
     if you wish not to use command line arguments manually type in your
     filename and path in the else case.
     */
    if(argc == 2)
        filename = argv[1];
    else
        filename = "input1.txt"; /*specify your input text file here*/
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n opening file: %s\n", filename);
	fileptr=fopen(filename,"r");
	hashTable = createMap(tableSize);
	char * word=getWord(fileptr);
	while(word!=NULL)
	{
		if (containsKey(hashTable,word)==0)
		{
			insertMap(hashTable,word,(int*)1);
			containsKey(hashTable,word);
		}
		else 
		{
			ValueType v = atMap(hashTable,word);
			v++;
			removeKey(hashTable,word);
			insertMap(hashTable,word,v);
		}
		word=getWord(fileptr);
		if(word == NULL){break;}
		numberOfWords++;
	}
	fclose(fileptr);
	printMap(hashTable, keyPrint, valPrint);
	printf("\n\n\n\n");
	end = clock();
	cpu_time_used = ((double)(end - start)) / (double)CLOCKS_PER_SEC;
	
	//printMap(hashTable);   /* Prints structure of hash table */
	printKeyValues(hashTable, keyPrint, valPrint); /* Prints all key-value pairs, one pair per line */
	printf("\nconcordance ran in %f seconds\n", cpu_time_used);
	printf("Table emptyBuckets = %d\n", emptyBuckets(hashTable));
    printf("Table count = %d\n", size(hashTable));
	printf("Table capacity = %d\n", capacity(hashTable));
	printf("Table load = %f\n", tableLoad(hashTable));
	printf("Deleting keys\n");
	removeKey(hashTable, "and");
	removeKey(hashTable, "me");
	removeKey(hashTable, "the");
	/* Test out the iterator */
#ifdef ITERATOR_IN
         struct mapItr *myItr;
         myItr = createMapIterator(hashTable);
         KeyType  key;
         /* Free up our keys and values using our iterator!!  Also printing them as we go along */
         while(hasNextMap(myItr))
           {
             key = nextMap(myItr);
             int *value = atMap(hashTable,key);
             printf("Freeing ...Key = %s, value = %d \n", key, *value);
             free(value);  /* To match the malloc above*/
             free(key);
           }
#endif
    deleteMap(hashTable);
	printf("\nDeleted the table\n");
	printf("\nNumber of words is: %d \n", numberOfWords);
	return 0;
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
	char* word = malloc(sizeof(char) * maxLength);
	assert(word != NULL);
	while( (character = fgetc(file)) != EOF)
	{
                character = tolower(character);  /* In should be the same as in ..so force all to lowercase */
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   character == 39) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}

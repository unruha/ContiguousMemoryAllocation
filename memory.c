#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#define MEMSIZE 80
#define BYTE 1

// maximum length of the command
#define MAX_LINE 80

// gets the line entered by the user that is the command to be executed next
char* getCommand();

// free all values in argumnts array
void freeArgs();

// allocates N bytes for process using one of the 3 allocation algorithms
void allocateMemory(char* name, int size, int algo);

// display contents of memory
void showMemory(char** memory);

int main()
{
    int should_run = 1;

    // string array holding the contents of the memory
    char** memory = malloc(MEMSIZE * sizeof(char*));
    for (int i = 0; i < MEMSIZE; i++)
    {
        memory[i] = malloc((BYTE + 1) * sizeof(char));
    }

    // initialize values of memory
    for (int i = 0; i < MEMSIZE; i++)
    {
        memory[i] = ".";
    }

    while(should_run)
    {
        printf("cma>");

        // get the command from the user
        char* command = getCommand();

        // string array to store the arguments
        char* args[MAX_LINE];

        // tokenize the string and store individual arguments
        tokenize(command, args);

        // store the type of operation
        char* operation = args[0];

        // handle each operation type
        if (strcmp(operation, "A") == 0)
        {
            char* name = args[1];
            int size = atoi(args[2]);
            char* algoType = args[3];
            int algo = -1;
            if (strcmp(algoType, "F") == 0)
            {
                allocateMemory(name, size, 1);
            }
            else if (strcmp(algoType, "B") == 0)
            {
                allocateMemory(name, size, 2);
            }
            else if (strcmp(algoType, "W") == 0)
            {
                allocateMemory(name, size, 3);
            }
            else
            {
                printf("Error: Invalid algorithm type\n");
            }
        }
        else if (strcmp(operation, "F") == 0)
        {
            printf("Free\n");
        }
        else if (strcmp(operation, "S") == 0)
        {
            showMemory(memory);
        }
        else if (strcmp(operation, "R") == 0)
        {
            printf("Read\n");
        }
        else if (strcmp(operation, "C") == 0)
        {
            printf("Compact\n");
        }
        else if (strcmp(operation, "E") == 0)
        {
            exit(0);
        }
        else 
        {
            printf("Invalid Operation\n");
        }

        // set all values in args to NULL
        freeArgs(args);
    }
    
    return 0;
}

// allocates N bytes for process using one of the 3 allocation algorithms
// algo: 1 = first-fit, 2 = best-fit, 3 = worst-fit
void allocateMemory(char* name, int size, int algo)
{
    printf("Allocate\n");
    printf("name: %s\n", name);
    printf("size: %d\n", size);
    printf("algo: %d\n", algo);

    // error check input
    if (size > 80)
    {
        printf("Error: Cannot allocate more than 80 bytes");
        return;
    }
    if (algo < 1 || algo > 3)
    {
        printf("Error: Invalid allocation algorithm type");
        return;
    }
    
}

// display contents of memory array
void showMemory(char** memory)
{
    for (int i = 0; i < MEMSIZE; i++)
    {
        printf(memory[i]);
    }
    printf("\n");
}

void freeArgs(char* args[])
{
    // set all values in args to NULL
    for (int i = 0; args[i] != NULL; i++)
    {
        args[i] = NULL;
    }
}

// gets the line entered by the user that is the command to be executed next
char* getCommand()
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}
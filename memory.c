#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

#define MEMSIZE 80

// maximum length of the command
#define MAX_LINE 80

// gets the line entered by the user that is the command to be executed next
char* getCommand();

// free all values in argumnts array
void freeArgs();

// allocates N bytes for process using one of the 3 allocation algorithms
allocateMemory(char* name, int size, int algo);

int main()
{
    int should_run = 1;

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
            int size = args[2];
            int algo = args[3];
            allocateMemory(name, size, algo);
        }
        else if (strcmp(operation, "F") == 0)
        {
            printf("Free\n");
        }
        else if (strcmp(operation, "S") == 0)
        {
            printf("Show\n");
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
            printf("Exit\n");
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
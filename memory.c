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

// chooses and runs the command
void runCommand(char* command, char** memory);

// free all values in arguments array
void freeArgs();

// allocates N bytes for process using one of the 3 allocation algorithms
void allocateMemory(char** memory, char* name, int size, int algo);

// use first fit allocation algorithm
void firstFit(char** memory, char* name, int size);

// use best fit allocation algorithm
void bestFit(char** memory, char* name, int size);

// use worst fit allocation algorithm
void worstFit(char** memory, char* name, int size);

// compact memory block to the lower indexes
void compactMemory(char** memory);

// read commands from file
void readFile(char* name, char** memory);

// free memory with given name
void freeMemory(char** memory, char* name);

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
        // get the command from the user
        char* command = getCommand();

        // run the command
        runCommand(command, memory);
    }
    
    return 0;
}

// chooses and runs the command
void runCommand(char* command, char** memory)
{
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
            allocateMemory(memory, name, size, 1);
        }
        else if (strcmp(algoType, "B") == 0)
        {
            allocateMemory(memory, name, size, 2);
        }
        else if (strcmp(algoType, "W") == 0)
        {
            allocateMemory(memory, name, size, 3);
        }
        else
        {
            printf("Error: Invalid algorithm type\n");
        }
    }
    else if (strcmp(operation, "F") == 0)
    {
        char* name = args[1];
        freeMemory(memory, name);
    }
    else if (strcmp(operation, "S") == 0)
    {
        showMemory(memory);
    }
    else if (strcmp(operation, "R") == 0)
    {
        char* fileName = args[1];
        // readFile(fileName, memory);
        printf("Read File command doesnt work correctly\n");
    }
    else if (strcmp(operation, "C") == 0)
    {
        compactMemory(memory);
    }
    else if (strcmp(operation, "E") == 0)
    {
        exit(0);
    }
    else 
    {
        printf("Invalid Operation\n");
    }

}

// allocates N bytes for process using one of the 3 allocation algorithms
// algo: 1 = first-fit, 2 = best-fit, 3 = worst-fit
void allocateMemory(char** memory, char* name, int size, int algo)
{
    // error check input
    if (size > 80)
    {
        printf("Error: Cannot allocate more than 80 bytes\n");
        return;
    }
    if (algo < 1 || algo > 3)
    {
        printf("Error: Invalid allocation algorithm type\n");
        return;
    }
    
    // handle first fit
    if (algo == 1)
    {
        firstFit(memory, name, size);
    }
    else if (algo == 2)
    {
        bestFit(memory, name, size);
    }
    else if (algo == 3)
    {
        worstFit(memory, name, size);
    }
}

void firstFit(char** memory, char* name, int size)
{
    // boolean to determine whether we need to continue looking for allocation space
    int foundSpace = 0;
    // pointer to the start and end index of the allocation range
    int start = 0;
    int end = 0;

    // search for allocatable space if we have not yet found it
    while (foundSpace == 0)
    {
        // move the starting pointer until it lands on unallocated memory space
        while (memory[start] != ".")
        {
            start++;
            // error check for start pointer running off end of memory
            if (start > MEMSIZE - 1)
            {
                printf("Allocation Error: Start pointer ran off end of memory\n");
                return;
            }
        }

        // check if there is enough memory remaining to do the allocation
        if (start + size > MEMSIZE)
        {
            printf("Allocation Error: Not enough contiguous memory\n");
            return;
        }
        
        // pointer to the ending index of the allocation range
        end = start;

        foundSpace = 1;

        // move the end pointer 'size' times or until it reaches non-empty memory
        // reaching non-empty memory would mean that we cannot allocate in this range
        for (int i = 0; i < size - 1; i++)
        {
            end++;
            // error check for running off end of memory
            if (end > MEMSIZE - 1)
            {
                printf("Allocation Error: End pointer ran off end of memory\n");
                return;
            }

            if (memory[end] != ".")
            {
                foundSpace = 0;
                break;
            }
        }
        // if we found an allocatable space with the correct size, then stop
        if (foundSpace == 1)
        {
            break;
        }
        else if (foundSpace == 0)
        {
            start = end;
        }
    }

    // we have found allocatable space, so allocate the memory
    for (int i = start; i <= end; i++)
    {
        memory[i] = name;
    }
}

void bestFit(char** memory, char* name, int size)
{
    // holds the starting location of an empty chunk of memory
    int start = 0;

    // holds the starting location of the current best starting location for allocation
    int bestStart = -1;

    // holds the current smallest length of contiguous memory that is allocatable by 'size'
    int smallest = 9999;

    // boolean holds whether or not we have searched the entire list
    int finished = 0;

    // find the starting location of the smallest free area of memory
    // but still must be larger than 'size'
    while (finished == 0)
    {
        // move the starting pointer until it lands on unallocated memory space
        while (memory[start] != ".")
        {
            start++;
            // error check for start pointer running off end of memory
            if (start > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        if (start > MEMSIZE - 1)
        {
            break;
        }
        if (start + size > MEMSIZE)
        {
            finished = 1;
            break;
        }
        int end = start;
        int tempSize = 9999;
        // move the end pointer until it reaches non empty memory to find the length of empty contiguous memory
        while (memory[end] == ".")
        {
            end++;
            // if we have ran off the end of the memory, we are done searching
            if (end > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        tempSize = end - start;
        if (tempSize < smallest && tempSize >= size)
        {
            smallest = tempSize;
            bestStart = start;
        }
        start = end;
    }

    // determine whether allocatable memory block was found
    if (bestStart != -1)
    {
        for (int i = bestStart; i < bestStart + size; i++)
        {
            memory[i] = name;
        }
    }
    else
    {
        printf("Allocation Error: Not enough contiguous memory");
    }
    
}

void worstFit(char** memory, char* name, int size)
{
    // holds the starting location of an empty chunk of memory
    int start = 0;

    // holds the starting location of the current best starting location for allocation
    int bestStart = -1;

    // holds the current smallest length of contiguous memory that is allocatable by 'size'
    int largest = -1;

    // boolean holds whether or not we have searched the entire list
    int finished = 0;

    // find the starting location of the smallest free area of memory
    // but still must be larger than 'size'
    while (finished == 0)
    {
        // move the starting pointer until it lands on unallocated memory space
        while (memory[start] != ".")
        {
            start++;
            // error check for start pointer running off end of memory
            if (start > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        if (start > MEMSIZE - 1)
        {
            break;
        }
        if (start + size > MEMSIZE)
        {
            finished = 1;
            break;
        }
        int end = start;
        int tempSize = -1;
        // move the end pointer until it reaches non empty memory to find the length of empty contiguous memory
        while (memory[end] == ".")
        {
            end++;
            // if we have ran off the end of the memory, we are done searching
            if (end > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        tempSize = end - start;
        if (tempSize > largest && tempSize >= size)
        {
            largest = tempSize;
            bestStart = start;
        }
        start = end;
    }

    // determine whether allocatable memory block was found
    if (bestStart != -1)
    {
        for (int i = bestStart; i < bestStart + size; i++)
        {
            memory[i] = name;
        }
    }
    else
    {
        printf("Allocation Error: Not enough contiguous memory");
    }
}

void freeMemory(char** memory, char* name)
{
    for (int i = 0; i < MEMSIZE; i++)
    {
        if (strcmp(memory[i], name) == 0)
        {
            memory[i] = ".";
        }
    }
}

void compactMemory(char** memory)
{
    // holds the starting location of a sequence of free memory
    int p1 = 0;

    // holds the starting location of sequence of occupied memory to the right of free memory
    int p2 = 0;

    // boolean determines whether we are done compacting
    int finished = 0;

    while (finished == 0)
    {
        // move p1 to the first block of free memory
        while (memory[p1] != ".")
        {
            p1++;
            if (p1 > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        if (p1 > MEMSIZE - 1)
        {
            break;
        }

        p2 = p1;
        // move p2 to the chunk of occupied memory to the right of p1
        while (memory[p2] == ".")
        {
            p2++;
            if (p2 > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
        if (p2 > MEMSIZE - 1)
        {
            break;
        }

        while (memory[p1] == "." && memory[p2] != ".")
        {
            // swap the values of the memory at p1 and p2
            char* temp = memory[p1];
            memory[p1] = memory[p2];
            memory[p2] = temp;

            p1++;
            p2++;

            // check if p2 ran off the end
            if (p2 > MEMSIZE - 1)
            {
                finished = 1;
                break;
            }
        }
    }
    
}

void readFile(char* name, char** memory)
{
    FILE* filePointer;
    int bufferLength = 100;
    char buffer[bufferLength];

    filePointer = fopen(name, "r");

    while (fgets(buffer, bufferLength, filePointer))
    {
        printf("State of the memory:\n");
        showMemory(memory);
        runCommand(buffer, memory);
    }

    fclose(filePointer);
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
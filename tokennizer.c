#include "tokenizer.h"

// tokenize user input into individual arguments
void tokenize(char* command, char* args[])
{
    // tokenize user input
    char* token;
    token = strtok(command, " \n");

    // add each token to arguments
    int i = 0;
    while (token != NULL)
    {
      args[i] = token;
      token = strtok(NULL, " \n");
      ++i;
    }

}
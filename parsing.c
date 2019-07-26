#include "shell.h"
int check_append(char *read_input)
{
  int check=0;
  int len=strlen(read_input);
  for(int i=0;i<len-1;i++)
  {
    if(read_input[i] == '>' && read_input[i+1] == '>')
    {
      check=1;
    } 
  }
  return check;
}
char **parsing(char *read_input, char* delimitor)
{
  int size = 100, i = 0;
  char **tokens = malloc(size * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(read_input, delimitor);
  while (token != NULL) 
  {
    tokens[i] = token;
    i++;

    if (i >= size) 
    {
      size += 100;
      tokens = realloc(tokens, size * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, delimitor);
  }
  tokens[i] = NULL;
  return tokens;
}
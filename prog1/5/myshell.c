#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Check if user entered a blank command
bool emptyCommand(char* input) {
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] != '\n' && input[i] != ' ' && input[i] != '\0') {
      return false;
    }
  }
  return true;
}

// Parse user's command into tokens
char** parseCommand(char* input) {
  // int filled = 0;
  int size = 0;
  char** tokens = (char**) malloc(size * sizeof(char*));

  // parse input command string character by character because a simply strtok 
  // would not work here. Spaces can be cause invalid tokenization because they
  // can appear in arguments bounded by quotations
  int start = -1;
  bool quotesToken = false;
  int i;
  for (i = 0; i < strlen(input); i++) {
    // end of line
    if (input[i] == '\0' || input[i] == '\n') {
      break;
    }
    if (start == -1) {
      // start new token
      start = i;
      if (input[i] == '"') {
        quotesToken = true;
      }
    } else if (input[i] == '"' && quotesToken) {
      // end of quotation-enclosed token (which can contain spaces)
      quotesToken = false;
    } else if ((input[i] == ' ' && !quotesToken)) {
      // end of token
      char* token = malloc((i-start) * sizeof(char));
      strncpy(token, input+start, (i-start) * sizeof(char));
      // resize tokens array 
      tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
      tokens[size] = token;
      size++;
      start = -1;
    }
  }
  // add last token
  if (start != -1) {
    char* token = malloc((i-start) * sizeof(char));
    strncpy(token, input+start, (i-start) * sizeof(char));
    // resize tokens array 
    tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
    tokens[size] = token;
    size++;
  }
  // add null token at end
  tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
  tokens[size] = (char*)NULL;
  return tokens;
}

int spawnSubCommand(char* binFile, char** args) {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    printf("Error occurred");
    return 1;
  } else if (pid == 0) {
    // child process
    execvp(binFile, args);
  } else {
    // parent process
    wait(NULL);
  }
  return 0;
}

int main() {
  char inp[100];
  while(true) {
    printf("mrprice_osh> ");
    fgets(inp, 100, stdin);

    if (!emptyCommand(inp)) {
      // run user's command
      char** tokens = parseCommand(inp);
      int cmdStatus = spawnSubCommand(tokens[0], tokens);
      if (cmdStatus == 1) {
        return 1;
      }
    }
  }
}

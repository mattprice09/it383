#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


struct Command {
  char** tokens;
  bool bg;
  char* outputFile;
};

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
struct Command parseCommand(char* input) {
  // int filled = 0;
  int size = 0;
  char** tokens = (char**) malloc(size * sizeof(char*));
  bool bg = false;

  // parse input command string character by character because a simply strtok 
  // would not work here. Spaces can be cause invalid tokenization because they
  // can appear in arguments bounded by quotations
  int start = -1;
  bool quotesToken = false;
  bool outputRedir = false;
  char* outputFile = NULL;
  int i;
  for (i = 0; i < strlen(input); i++) {
    // end of line
    if (input[i] == '\0' || input[i] == '\n') {
      break;
    }
    if (start == -1) {
      // start new token
      // skip leading spaces
      if (input[i] != ' ') {
        start = i;
        if (input[i] == '"') {
          // start of items within quotation
          quotesToken = true;
        } else if (input[i] == '>') {
          // handle redirection of output
          outputRedir = true;
          start = -1;
        }
      }
    } else if (input[i] == '"' && quotesToken) {
      // end of quotation-enclosed token (which can contain spaces)
      quotesToken = false;
    } else if ((input[i] == ' ' && !quotesToken)) {
      // end of token
      if (outputRedir) {
        // output file
        outputFile = malloc((i-start) * sizeof(char));
        strncpy(outputFile, input+start, (i-start) * sizeof(char));
        outputRedir = false;
      } else {
        // regular token
        char* token = malloc((i-start) * sizeof(char));
        strncpy(token, input+start, (i-start) * sizeof(char));
        // resize tokens array
        tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
        tokens[size] = token;
        size++;
      }
      start = -1;
    }
  }
  // add last token
  if (outputRedir) {
    outputFile = malloc((i-start) * sizeof(char));
    strncpy(outputFile, input+start, (i-start) * sizeof(char));
  } else {
    char* token = malloc((i-start) * sizeof(char));
    strncpy(token, input+start, (i-start) * sizeof(char));
    if (*token == '&') {
      bg = true;
    } else if (start != -1) {
      // resize tokens array 
      tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
      tokens[size] = token;
      size++;
    }
  }
  // add null token at end
  tokens = (char**) realloc(tokens, (size+1) * sizeof(char*));
  tokens[size] = (char*)NULL;
  // make sure output file string is initialized
  if (outputFile == NULL) {
    outputFile = "";
  }

  struct Command cmd = { tokens, bg, outputFile };
  return cmd;
}

// Gets many command objects, split by "|" (piping) 
struct Command* getAllCommands(char* input, int numCommands) {
  // get array of Command objects
  struct Command* cmds = (struct Command*) malloc(numCommands * sizeof(struct Command));
  int curr = 0;
  for (char *tok = strtok(input, "|"); tok != NULL; tok = strtok(NULL, "|")) {
    cmds[curr] = parseCommand(tok);
    curr++;
  }
  return cmds;
}

// Spawn a child process and run command
int spawnSubCommand(int in, int out, struct Command* cmd) {
  pid_t pid;
  pid = fork();
  if (pid < 0) {
    printf("Error occurred");
    return 1;
  } else if (pid == 0) {
    if (in != 0) {
      dup2(in, 0);
      close(in);
    }
    if (out != 1) {
      dup2(out, 1);
      close(out);
    }
    return execvp(cmd->tokens[0], cmd->tokens);
  }
  return pid;
}

// Run 1 or more commands. If more than 1 command, pipe output
// between them.
int runCommandChain(struct Command* cmds, int numCommands) {
  
  pid_t pid;
  int fd[2];
  int in = 0;

  // pipe all but the final command
  int i;
  for (i = 0; i < numCommands-1; i++) {
    pipe(fd);
    spawnSubCommand(in, fd[1], cmds+i);
    close(fd[1]);
    in = fd[0];
  }

  // run final command
  pipe(fd);
  pid = fork();
  if (pid < 0) {
    printf("Error occurred");
    return 1;
  } else if (pid == 0) {
    // child process
    if (in != 0) {
      dup2(in, 0);
      close(in);
    }
    close(fd[1]);

    // redirect output if specified
    if (strcmp(cmds[i].outputFile, "") != 0) {
      close(1);
      int outfd = open(cmds[i].outputFile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
      dup(outfd);
    }
    execvp(cmds[i].tokens[0], cmds[i].tokens);
    return 1;

  } else {
    // parent process
    if (!cmds[i].bg) {
      // run in fg
      close(fd[1]);
      close(fd[0]);
      for (i = 0; i < numCommands; i++) {
        wait(NULL);
      }

    } else {
      // run in bg
      printf("[1] Running (Pid: %d) ", pid);
      for (int j = 0; cmds[i].tokens[j]; j++) {
        printf("%s ", cmds[i].tokens[j]);
      }
      printf("&\n");
      close(fd[1]);
      close(fd[0]);
    }
  }
  return 0;
}

int main() {
  int controller = 0;
  int histCount = 0;
  int histCap = 1;
  char** history = (char**) malloc(histCap * sizeof(char*));
  while(true) {
    controller++;
    char* inp = malloc(100 * sizeof(char));
    printf("mrprice_osh> ");
    fgets(inp, 100, stdin);

    if (!emptyCommand(inp)) {

      // history functionality
      if (inp[0] != '!' && strcmp(inp, "history\n") != 0) {
        // add to history
        if (histCount == histCap) {
          // double history array's size
          histCap = histCap * 2;
          history = (char**) realloc(history, (histCap) * sizeof(char*));
        }
        // deep copy user input
        char* newStr = malloc(sizeof(char) * strlen(inp));
        strcpy(newStr, inp);
        // store history
        history[histCount] = newStr;
        histCount++;
      } else {
        // view history
        if (strcmp(inp, "history\n") == 0) {
          // all history
          for (int i = 0; i < histCount; i++) {
            printf(" %d  %s", i, history[i]);
          }
          continue;
        } else if (strcmp(inp, "!!\n") == 0) {
          // run most recent command
          inp = history[histCount-1];
        } else {
          // run specific command from history
          int i;
          for (i = 1; i < strlen(inp); i++) {
            if (!isdigit(inp[i])) {
              break;
            }
          }
          int histNum;
          if (i > 1) {
            char* digitStr = malloc((i-1) * sizeof(char));
            strncpy(digitStr, inp+1, (i-1) * sizeof(char));
            histNum = (int)strtol(digitStr, NULL, 10);
            // run the command
            if (histNum < histCount) {
              inp = history[histNum];
            }
            free(digitStr);
          }
        }
      }

      // get the number of pipes in the command
      int numPipes = 0;
      for (int i = 0; i < strlen(inp); i++) {
        if (inp[i] == '|') {
          numPipes++;
        }
      }
      // get all commands
      struct Command* cmds = getAllCommands(inp, numPipes+1);
      // run command(s)
      int cmdStatus = runCommandChain(cmds, numPipes+1);
      if (cmdStatus == 1) {
        return 1;
      }
      free(cmds);
    }
    // if (controller >= 10) {
    //   return 1;
    // }
  }
  free(history);
}

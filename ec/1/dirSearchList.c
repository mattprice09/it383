#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


// Join a directory name with a file name, separated by a "/"
char* get_abs_path(char* dir, char* fname) {

  // size of full path (+1 extra for extra slash)
  int n = strlen("/") + strlen(dir) + strlen(fname) + 1;
  // build the string
  char* abs = malloc(n * sizeof(char));
  strcpy(abs, dir);
  strcat(abs, "/");
  strcat(abs, fname);

  return abs;
}


// Get args to be used for execvp call.
// Format: ["ls", "-l", <<path>>, NULL]
char** get_ls_args(char* path) {

  // size of full array
  int n = strlen("ls") + strlen("-l") + strlen(path);

  char** args = malloc(n * sizeof(char*));

  // allocate memory for the 3 strings in the array
  args[0] = malloc(strlen("ls") * sizeof(char));
  args[1] = malloc(strlen("ls") * sizeof(char));
  args[2] = malloc(strlen(path) * sizeof(char));
  // copy the strings into the array
  strcpy(args[0], "ls");
  strcpy(args[1], "-l");
  strcpy(args[2], path);
  args[3] = NULL;

  return args;
}


// Checks if a string filepath is a directory
int is_directory(const char *path) {
  struct stat statbuf;
  if (stat(path, &statbuf) != 0)
    return 0;
  return S_ISDIR(statbuf.st_mode);
}


// Runs `ls -l <<path>>` in a child process
int print_ls_output(char* path) {

  char** ls_args = get_ls_args(path);

  pid_t pid;
  pid = fork();
  if (pid < 0) {
    printf("Error occurred");
    return 1;
  } else if (pid == 0) {
    // child process
    execvp("ls", ls_args);
    return 1;
  } else {
    // parent process
    wait(NULL);
  }

  free(ls_args);
  return 0;
}


/**
* Recursively search a directory for a given string representing some substring of a file name.
* For any matches found within the starting directory, run sub-command `ls -l <<match_fp>>` and
* print to console.
*/
int search_dir(char* cwd, char* target) {

  int found = 0;

  DIR *d = opendir(cwd);
  struct dirent *dobj;
  if (d) {
    // read all files/directories within the cwd
    while ((dobj = readdir(d)) != NULL) {

      if (dobj->d_name[0] == '.') {
        // ignore hidden files
        continue;
      }

      // get full path name of current file
      char* fpath = get_abs_path(cwd, dobj->d_name);

      // printf("Checking %s...\n", fpath);
      if (is_directory(fpath) == 1) {
        // directory, make recursive call
        if (search_dir(fpath, target) == 1) {
          found = 1;
        }
      } else {
        // not a directory, see if we have a match
        if(strstr(dobj->d_name, target) != NULL) {
          // found a match! print details on match
          print_ls_output(fpath);
          found = 1;
        }
      }
      free(fpath);
    }
    closedir(d);
  }
  return found;
}


int main(int argc, char *argv[]) {

  if (argc < 3) {
    printf("> ERROR: Correct usage: `./dirSearchList <<search_term>> <<search_directory>>");
    return 1;
  }

  if (search_dir(argv[2], argv[1]) == 0) {
    // no results found
    printf("SEARCH FAILED!!\n");
  }
}

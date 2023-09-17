#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {
  char *args[MAX_LINE/2 + 1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */
  char input[MAX_LINE];
  
  while (should_run) {
    printf("osh>");
    fflush(stdout);
    
    fgets(input, MAX_LINE, stdin);
    input[strcspn(input, "\n")] = '\0'; // Removing the newline character at the end
    
    int background = 0;
    if(input[strlen(input) - 1] == '&') {
      background = 1;
      input[strlen(input) - 1] = '\0'; // Removing & to properly execute the command
    }
    
    // Parse the input
    int i = 0;
    char *token = strtok(input, " ");
    while(token != NULL) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL; // Null terminate the list of arguments

    if(i == 0) {
      continue; // No command entered
    }

    // Special case for exit command
    if(strcmp(args[0], "exit") == 0) {
      should_run = 0;
      continue;
    }
    
    pid_t pid = fork(); // Creating new process
    if(pid == 0) {
      // Child process
      execvp(args[0], args);
      // If execvp fails
      printf("Command not found\n");
      exit(1);
    } else if(pid > 0) {
      // Parent process
      if(!background) {
        wait(NULL); // Only wait if it's a foreground process
      }
    } else {
      printf("Fork failed\n");
      exit(1);
    }
  }
  return 0;
}

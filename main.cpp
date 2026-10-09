#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

int myShell_cd(char** args) {
    if (args[1] == NULL) {
        printf(":( expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            printf(":( No such directory exists. \n");
        }
    }
    return 1;
}

int myShell_help() {
    printf("A simple shell :)\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built-in:\n");

    const char* builtin_str[] = {"cd", "help", "exit", "rm", "mv"};

    for (int i = 0; i < 5; i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int myShell_exit() {
    return 0;
}

int myShell_rm(char** args) {
    if (args[1] == NULL) {
        printf(":( Expected argument to \"rm\"\n");
    } else {
        if (remove(args[1]) != 0) {
            printf(":( No such directory exists. \n");
        }
    }
    return 1;
}

int isDirectory(const char* path) {
    struct stat path_stat;
    if( (stat(path, &path_stat)) != 0) {
        return 0;
    }
    return 1;
}

int myShell_mv(char** args) {
    if (args[1] == NULL || args[2] == NULL) {
        printf(":( Expected two arguments to \"mv\"\n");
    } else {
        // Rename if both are arguments are files/folders
        if (rename(args[1], args[2]) != 0) {
            // If rename fails, check if 2nd argument is a valid directory
            if(isDirectory(args[2])) {
                // Create the system call for moving file to specified directory
                string command = "mv " + string(args[1]) + " " + string(args[2]);
                if (system(command.c_str()) != 0) {
                    printf(":( Error performing move.\n");
                }
            } else {
                printf(":( Invalid arguments to \"mv\"");
            }
        }
    }
    return 1;
}

int myShell_launch(char** args) {
    pid_t pid = fork();

    if (pid == -1) {
        printf(":( Error Forking! \n");
        return 1;
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            printf(":( Error executing command \n");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return 1;
    }

    return 1; // Return value for the parent process
}

int myShell_execute(char** args) {
    if (args[0] == NULL) {
        // empty command
        return 1;
    }

    const char* builtin_str[] = {"cd", "help", "exit", "rm", "mv"};

    for (int i = 0; i < 5; i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            switch (i) {
                case 0:
                    return myShell_cd(args);
                case 1:
                    return myShell_help();
                case 2:
                    return myShell_exit();
                case 3:
                    return myShell_rm(args);
                case 4:
                    return myShell_mv(args);
            }
        }
    }

    return myShell_launch(args);
}

char* myShell_read_line() {
    char* line = NULL;
    size_t len = 0;
    ssize_t read = getline(&line, &len, stdin);

    if (read == -1) {
        printf("Exiting Shell...\n");
        free(line);
        exit(EXIT_SUCCESS);
    }

    return line;
}

char** myShell_split_line(const char* line) {
    int bufsize = 64;
    int position = 0;
    char** tokens = (char**)malloc(bufsize * sizeof(char*));
    char* token;

    token = strtok((char*)line, " \t\r\n\a");
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
        }

        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

void myShell_loop() {
    char* line;
    char** args;
    int status;

    do {
        printf(":) ");
        line = myShell_read_line();
        args = myShell_split_line(line);
        status = myShell_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main() {
    myShell_loop();
    return EXIT_SUCCESS;
}
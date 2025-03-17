#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "main.h"
#include "debug.h"


#define BUFFER_BLOCK_SIZE 64
#define BUFFER_ARG_BLOCK_SIZE 8
#define TOKEN_DELIM " \t\r\n\a"

#define ARG_LENGTH 8
#define TESTING 1

int main(int argc, char** argv){
    shell_loop();
}

void shell_loop(void){
    char* line;
    char **args;
    int status;

    do {
        printf("> ");
        line = read_line();
        args = split_line(line);
        status = launch(args);

        // Free line allocated in read_line
        free(line);
        // Free args allocated in split_line
        free(args);
    } while (status);
}

/**
 * @brief Reads a line from stdin using getchar()
*/
char* read_line(void){
    // Tmp variables
    int buffer_size = BUFFER_BLOCK_SIZE;
    char* buffer = (char*) malloc(sizeof(char) * BUFFER_BLOCK_SIZE);
    int current_char;

    int current_position = 0;

    // Check for allocation error
    if (!buffer){
        perror("[Read Line]: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        current_char = getchar();
        // If we should return
        if (current_char == EOF || current_char == '\n'){
            // Add null char
            buffer[current_position] = '\0';
            return buffer;
        }

        buffer[current_position] = current_char;
        current_position++;

        // If we should resize
        if (current_position >= buffer_size){
            buffer_size += BUFFER_BLOCK_SIZE;
            buffer = (char*) realloc(buffer, sizeof(char) * buffer_size);
            
            if (!buffer){
                perror("[Read Line]: Memory allocation failure\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return buffer;
}

/**
 * @brief Function to tokanize input
 */
char** split_line(char* line){
    int buffer_size = BUFFER_ARG_BLOCK_SIZE;
    char** tokens = (char**) malloc(sizeof(char*) * BUFFER_ARG_BLOCK_SIZE);
    char* token;
    int current_position = 0;

    // Check for allocation error
    if (!tokens){
        perror("[Split Line]: Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIM);
    while(token != NULL){
        tokens[current_position] = token;
        current_position++;

        // If we need to resize
        if(current_position >= buffer_size){
            buffer_size += BUFFER_ARG_BLOCK_SIZE;
            tokens = realloc(tokens, sizeof(char*) * buffer_size);
            if (!tokens){
                perror("[Split Line]: Memory allocation failure\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[current_position] = NULL;
    return tokens;
 
}

int launch(char** args){
    pid_t pid;
    int status;

    // Fork this process
    pid = fork();
    // If 0, returned to the newly created child process
    if (pid < 0){
        perror("[Launch]:   Error when forking\n");
        exit(EXIT_FAILURE);
    }
    // If "we" are the newly created child
    else if(pid == 0){
        if (execvp(args[0], args) == -1){
            perror("[Launch (child)]:   Error when launching\n");
        }
        exit(EXIT_FAILURE);
    }
    // If we are the original process
    else{
        do{
            waitpid(pid, &status, WUNTRACED);
        }
        while(!(WIFEXITED(status)) && !WIFSIGNALED(status)); 
    }
    return 1;
}
#ifndef MAIN_H
#define MAIN_H

char* read_line(void);
void shell_loop(void);
char** split_line(char* line);
int launch(char** args);
int excecute(char** args);

#endif
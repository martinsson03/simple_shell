char* read_line(void);
void shell_loop(void);
char** split_line(char* line);
int launch(char** args);
int excecute(char** args);
void print_prompt(void);
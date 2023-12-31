#if !defined(SIMPLE_SSERVER_H)
#define SIMPLE_SSERVER_H


#include "simple-sserver-datastructure.h"

#define LINE_MAX 255
#define SUCCESSFUL_COMPILATION 1
#define FAILED_COMPILATION 0
#define FILE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

Commands read_commands(const char compile_cmds[], const char test_cmds[]);
void clear_commands(Commands *const commands);
int compile_program(Commands commands);
int test_program(Commands commands);

#endif

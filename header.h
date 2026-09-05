#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// errors.c
void check_start_errors(int argc, char *argv[], FILE** file);
long check_return_integer(char* string);
void check_get_nextline(char** line, size_t* length, ssize_t* read, FILE** file);
void check_empty_token(char* string);
void check_task_values(long time_until_new, long deadline, long time_needed);

#endif
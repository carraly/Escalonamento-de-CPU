#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// errors.c
void check_start_errors(int argc, char *argv[], FILE** file);
long check_return_integer(char* string);

#endif
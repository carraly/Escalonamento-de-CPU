#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct Task {
    char* name;
    long time_until_new;
    long deadline;
    long time_needed;
} Task;

typedef struct Node {
    Task task;
    struct Node* next;
} Node;

// errors.c
void check_start_errors(int argc, char *argv[], FILE** file);
long check_return_integer(char* string);
void check_get_nextline(char** line, size_t* length, ssize_t* read, FILE** file);
void check_empty_token(char* string);
void check_task_values(long time_until_new, long deadline, long time_needed);

// rate.c
void rate_scheduler(Node* head, long total_time);

// edf.c
void edf_scheduler(Node* head, long total_time);

// math.c
long calculate_maximum_queue(Node* head, long total_time);

#endif
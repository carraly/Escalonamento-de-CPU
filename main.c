#include "header.h"

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

int main(int argc, char *argv[]) {
    FILE* file;

    check_start_errors(argc, argv, file);

    char* line = NULL;
    size_t length = 0;
    ssize_t read;

    if ((read = getline(&line, &length, file)) == -1) {
        fprintf(stderr, "Failed reading first line\n");
        exit(EXIT_FAILURE);
    }

    char* string = strtok(line, " \n");
    check_integers(string);
    long total_time = (long)string;
    
    Node* head;
}
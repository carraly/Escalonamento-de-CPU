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

    check_start_errors(argc, argv, &file);

    char* line = NULL;
    size_t length = 0;
    ssize_t read;

    check_get_nextline(&line, &length, &read, &file);

    char* string = strtok(line, " \n");
    long total_time = check_return_integer(string);
    
    string = strtok(NULL, " \n");
    if (string != NULL) {
        fprintf(stderr, "More arguments than expected in first line\n");
        exit(EXIT_FAILURE);
    }

    Node* head = NULL;
    while (read != -1) {
        check_get_nextline(&line, &length, &read, &file);
        string = strtok(line, " \n");
        for (int i = 0; i < 4; i++) {
            
        }
    }
}
#include "header.h"
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

void add_task(char** name, long time_until_new, long deadline, long time_needed, Node** head) {
    Node* temp = *head;
    if (*head == NULL) {
        *head = (Node*) malloc(sizeof(Node));
        temp = *head;
    }else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = (Node*) malloc(sizeof(Node));
        temp = temp->next;
    }
    temp->task.name = *name;
    temp->task.time_until_new = time_until_new;
    temp->task.deadline = deadline;
    temp->task.time_needed = time_needed;
    temp->next = NULL;
}

int main(int argc, char *argv[]) {
    FILE* file;

    check_start_errors(argc, argv, &file);

    char* line = NULL;
    size_t length = 0;
    ssize_t read;

    check_get_nextline(&line, &length, &read, &file);
    if (read == -1) {
        fprintf(stderr, "First line is empty\n");
        exit(EXIT_FAILURE);
    }

    char* string = strtok(line, " \n");
    long total_time = check_return_integer(string);
    
    string = strtok(NULL, " \n");
    if (string != NULL) {
        fprintf(stderr, "More arguments than expected in first line\n");
        exit(EXIT_FAILURE);
    }

    Node* head = NULL;
    check_get_nextline(&line, &length, &read, &file);
    while (read != -1) {
        char* name;
        long time_until_new;
        long deadline;
        long time_needed;
        
        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                string = strtok(line, " \n");
            }else {
                string = strtok(NULL, " \n");
            }

            check_empty_token(string);
            
            if (i == 0) {
                name = (char*) malloc(strlen(string) + 1); // strlen não conta o \0
                strcpy(name, string);
            }else if (i == 1) {
                time_until_new = check_return_integer(string);
            }else if (i == 2) {
                deadline = check_return_integer(string);
            }else {
                time_needed = check_return_integer(string);
            }
        }
        add_task(&name, time_until_new, deadline, time_needed, &head);
        check_get_nextline(&line, &length, &read, &file);
    }
}
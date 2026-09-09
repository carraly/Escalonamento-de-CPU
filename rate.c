#include "header.h"

void rate_add_tasks_queue(Node** tasks_queue, Task new_task, long passed_time) {

    if (*tasks_queue == NULL) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        (*tasks_queue)->task.deadline += passed_time;
        return;
    }

    Node* last = *tasks_queue;
    if (new_task.time_until_new < last->task.time_until_new) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        (*tasks_queue)->task.deadline += passed_time;
        (*tasks_queue)->next = last;
        return;
        }

    while (last->next != NULL) {
        if (new_task.time_until_new < last->next->task.time_until_new) {
            break;
        }
        last = last->next;
    }

    if (last->next == NULL) {
        last->next = (Node*) malloc(sizeof(Node));
        last = last->next;
        last->task = new_task;
        last->task.deadline += passed_time;
        last->next = NULL;
    }else {
        Node* temp = last->next;
        last->next = (Node*) malloc(sizeof(Node));
        last = last->next;
        last->task = new_task;
        last->task.deadline += passed_time;
        last->next = temp;
    }
}

void rate_change_task(char** current_action, char* new_action, long* passed_time, long* previous_time, FILE* file, char mode) {
    if (strcmp(*current_action, new_action) == 0) {
        return;
    }
 
    long duration = *passed_time - *previous_time;
    if (duration > 0) {
        if (strcmp(*current_action, "idle") == 0) {
            fprintf(file, "idle for %ld units\n", duration);
        }else if (mode != 'K') {
            fprintf(file, "[%s] for %ld units - %c\n", *current_action, duration, mode);
        }
    }
    
    *previous_time = *passed_time;
    strcpy(*current_action, new_action);
}

void rate_scheduler(Node* head, long total_time, long max_name) {
    FILE* file = fopen("rate_vchlm.out", "w");
    fprintf(file, "EXECUTION BY RATE\n");
 
    long passed_time = 0;
    long previous_time = 0;

    if (max_name < strlen("idle")) {
        max_name = strlen("idle");
    }
    char* current_action = (char*) malloc(max_name * sizeof(char) + 1); // Maldito +1 para caractere nulo, sempre esqueço
    strcpy(current_action, "idle");

    Node* tasks_queue = NULL;
    Node* temp = head;
    char mode = 'H';
    long lost_deadlines = 0;
    long complete_execution = 0;
    long killed = 0;

    while (passed_time < total_time) {
        temp = head;
        while (temp != NULL) {
            if (passed_time == 0 || passed_time % temp->task.time_until_new == 0) {
                rate_add_tasks_queue(&tasks_queue, temp->task, passed_time);
            }
            temp = temp->next;
        }
        temp = tasks_queue;
        Node* temp2;
        while (temp != NULL) {
            if (passed_time >= temp->task.deadline) {
                if (temp == tasks_queue) {
                    tasks_queue = temp->next;
                    free(temp);
                    temp = tasks_queue;
                    mode = 'L';
                    lost_deadlines++;
                    continue;
                }else {
                    temp2->next = temp->next;
                    free(temp);
                    temp = temp2->next;
                    lost_deadlines++;
                    continue;
                }
            }
            temp2 = temp;
            temp = temp->next;
        }
        
        if (tasks_queue == NULL) {
            rate_change_task(&current_action, "idle", &passed_time, &previous_time, file, mode);
            passed_time++;
            continue;
        }
        
        rate_change_task(&current_action, tasks_queue->task.name, &passed_time, &previous_time, file, mode);
        tasks_queue->task.time_needed--;
        
        if (tasks_queue->task.time_needed == 0) {
            Node* temp_queue = tasks_queue;
            tasks_queue = tasks_queue->next;
            free(temp_queue);
            mode = 'F';
            complete_execution++;
        }else {
            mode = 'H';
        }

        passed_time++;
    }
 
    long remaining_time = passed_time - previous_time;
    if (strcmp(current_action, "idle") == 0) {
        if (remaining_time > 0) {
            fprintf(file, "idle for %ld units\n", remaining_time);
        }
    }else if (remaining_time > 0 && mode == 'F') {
        fprintf(file, "[%s] for %ld units - %c\n", current_action, remaining_time, mode);
    }
 
    temp = tasks_queue;
    while (temp != NULL) {
        Node* next = temp->next;
        free(temp);
        temp = next;
        killed++;
    }
 
    fprintf(file, "\n");
    fprintf(file, "LOST DEADLINES: %ld\n", lost_deadlines);
    fprintf(file, "COMPLETE EXECUTION: %ld\n", complete_execution);
    fprintf(file, "KILLED: %ld\n", killed);
 
    free(current_action);
    fclose(file);
}
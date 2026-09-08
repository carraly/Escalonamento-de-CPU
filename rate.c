#include "header.h"

void rate_add_tasks_queue(Node** tasks_queue, Task new_task) {

    if (*tasks_queue == NULL) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        return;
    }

    Node* last = *tasks_queue;
    if (new_task.time_until_new < last->task.time_until_new) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
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
        last->next = NULL;
    }else {
        Node* temp = last->next;
        last->next = (Node*) malloc(sizeof(Node));
        last = last->next;
        last->task = new_task;
        last->next = temp;
    }
}

void rate_change_task(char** current_action, char* new_action, long* passed_time, long* previous_time) {
    if (strcmp(*current_action, new_action) == 0) {
        return;
    }

    printf("[%s] for %ld units - ", *current_action, *passed_time-*previous_time);
    *previous_time = *passed_time;
    strcpy(*current_action, new_action);
}

void rate_scheduler(Node* head, long total_time, long max_name) {
    FILE* file = fopen("rate_vchlm.out", "w");

    long passed_time = 0;
    long previous_time = 0;
    char* current_action = (char*) malloc(max_name * sizeof(char));
    strcpy(current_action, "idle");
    Node* tasks_queue = NULL;
    Node* temp = head;

    while (passed_time < total_time) {
        temp = head;
        while (temp != NULL) {
            if (passed_time == 0 || passed_time % temp->task.time_until_new == 0) {
                rate_add_tasks_queue(&tasks_queue, temp->task);
            }
            temp = temp->next;
        }
        if (tasks_queue != NULL) {
            rate_change_task(&current_action, tasks_queue->task.name, &passed_time, &previous_time);
        }else {
            rate_change_task(&current_action, "idle", &passed_time, &previous_time);
        }
        tasks_queue->task.time_needed--;
        passed_time++;

        if (tasks_queue->task.time_needed == 0) {
            Node* temp_queue = tasks_queue;
            tasks_queue = tasks_queue->next;
            free(temp_queue); 
        }
    }

    fclose(file);
}
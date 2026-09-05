#include "header.h"

void rate_add_tasks_queue(Node** tasks_queue, Node* temp) {
    Task new_task = temp->task;

    if (*tasks_queue == NULL) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        return;
    }

    Node* last = *tasks_queue;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = (Node*) malloc(sizeof(Node));
    last = last->next;
    last->task = new_task;
    last->next = NULL;
}

void rate_scheduler(Node* head, long total_time) {
    FILE* file = fopen("rate_vchlm.out", "w");

    long passed_time = 0;
    Node* tasks_queue = NULL;
    Node* temp = head;

    while (passed_time < total_time) {
        while (temp != NULL) {
            if (passed_time == 0 || temp->task.time_until_new / passed_time == 0) {
                rate_add_tasks_queue(&tasks_queue, temp);
            }
            temp = temp->next;
        }
        passed_time++;
    }
}
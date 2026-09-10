#include "header.h"

long edf_find_task_index(Node* head, char* name) {
    long index = 0;
    while (strcmp(head->task.name, name) != 0) {
        head = head->next;
        index++;
    }
    return index;
}

int edf_has_priority(Node* head, char* new_name, long new_deadline, Task existing) {
    if (new_deadline != existing.deadline) {
        return new_deadline < existing.deadline;
    }
    return edf_find_task_index(head, new_name) < edf_find_task_index(head, existing.name);
}

void edf_add_tasks_queue(Node** tasks_queue, Task new_task, long passed_time, Node* head) {

    if (*tasks_queue == NULL) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        (*tasks_queue)->task.deadline += passed_time;
        return;
    }

    long new_absolute_deadline = new_task.deadline + passed_time;

    Node* last = *tasks_queue;
    if (edf_has_priority(head, new_task.name, new_absolute_deadline, last->task)) {
        *tasks_queue = (Node*) malloc(sizeof(Node));
        (*tasks_queue)->task = new_task;
        (*tasks_queue)->task.deadline += passed_time;
        (*tasks_queue)->next = last;
        return;
        }

    while (last->next != NULL) {
        if (edf_has_priority(head, new_task.name, new_absolute_deadline, last->next->task)) {
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

void edf_change_task(char** current_action, char* new_action, long* passed_time, long* previous_time, FILE* file, char mode) {
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

void edf_scheduler(Node* head, long total_time, long max_name) {
    FILE* file = fopen("edf_vchlm.out", "w");
    fprintf(file, "EXECUTION BY EDF\n\n");
 
    long passed_time = 0;
    long previous_time = 0;

    if ((size_t) max_name < strlen("idle")) {
        max_name = strlen("idle");
    }
    char* current_action = (char*) malloc(max_name * sizeof(char) + 1); // Maldito +1 para caractere nulo, sempre esqueço
    strcpy(current_action, "idle");
 
    long num_tasks = 0;
    Node* counter = head;
    while (counter != NULL) {
        num_tasks++;
        counter = counter->next;
    }
 
    long* lost_count = (long*) calloc(num_tasks, sizeof(long));
    long* complete_count = (long*) calloc(num_tasks, sizeof(long));
    long* killed_count = (long*) calloc(num_tasks, sizeof(long));
 
    Node* tasks_queue = NULL;
    Node* temp = head;
    char mode = 'H';
 
    while (passed_time < total_time) {
        temp = head;
        while (temp != NULL) {
            if (passed_time == 0 || passed_time % temp->task.time_until_new == 0) {
                edf_add_tasks_queue(&tasks_queue, temp->task, passed_time, head);
            }
            temp = temp->next;
        }
        temp = tasks_queue;
        Node* temp2;
        while (temp != NULL) {
            if (passed_time >= temp->task.deadline) {
                if (temp == tasks_queue) {
                    tasks_queue = temp->next;
                    lost_count[edf_find_task_index(head, temp->task.name)]++;
                    free(temp);
                    temp = tasks_queue;
                    mode = 'L';
                    continue;
                }else {
                    temp2->next = temp->next;
                    lost_count[edf_find_task_index(head, temp->task.name)]++;
                    free(temp);
                    temp = temp2->next;
                    continue;
                }
            }
            temp2 = temp;
            temp = temp->next;
        }
        
        if (tasks_queue == NULL) {
            edf_change_task(&current_action, "idle", &passed_time, &previous_time, file, mode);
            passed_time++;
            continue;
        }
        
        edf_change_task(&current_action, tasks_queue->task.name, &passed_time, &previous_time, file, mode);
        tasks_queue->task.time_needed--;
        
        if (tasks_queue->task.time_needed == 0) {
            Node* temp_queue = tasks_queue;
            tasks_queue = tasks_queue->next;
            complete_count[edf_find_task_index(head, temp_queue->task.name)]++;
            free(temp_queue);
            mode = 'F';
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
        killed_count[edf_find_task_index(head, temp->task.name)]++;
        free(temp);
        temp = next;
    }
 
    fprintf(file, "\n");
 
    fprintf(file, "LOST DEADLINES\n");
    temp = head;
    long index = 0;
    while (temp != NULL) {
        fprintf(file, "[%s] %ld\n", temp->task.name, lost_count[index]);
        temp = temp->next;
        index++;
    }
 
    fprintf(file, "\nCOMPLETE EXECUTION\n");
    temp = head;
    index = 0;
    while (temp != NULL) {
        fprintf(file, "[%s] %ld\n", temp->task.name, complete_count[index]);
        temp = temp->next;
        index++;
    }
 
    fprintf(file, "\nKILLED\n");
    temp = head;
    index = 0;
    while (temp != NULL) {
        fprintf(file, "[%s] %ld\n", temp->task.name, killed_count[index]);
        temp = temp->next;
        index++;
    }
 
    free(current_action);
    free(lost_count);
    free(complete_count);
    free(killed_count);
 
    fclose(file);
}
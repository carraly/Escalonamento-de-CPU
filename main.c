#include "header.h"

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
}
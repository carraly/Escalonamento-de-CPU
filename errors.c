#include "header.h"

void check_start_errors(int argc, char *argv[], FILE** file) {
    if (argc != 3) {
        fprintf(stderr, "Unexpected amount of arguments: expected 2\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Unexpected first argument: expected 'rate' or 'edf'\n");
        exit(EXIT_FAILURE);
    }

    *file = fopen(argv[2], "r");
    if (*file == NULL) {
        fprintf(stderr, "Failed opening file\n");
        exit(EXIT_FAILURE);
    }
}

long check_return_integer(char* string) {
    char* endptr;
    long value = strtol(string, &endptr, 10);

    if (endptr == string) {
        fprintf(stderr, "Failed conversion: no digits were found\n");
        exit(EXIT_FAILURE);
    }else if (*endptr != '\0') {
        fprintf(stderr, "Failed conversion: mixed digits and caracteres\n");
        exit(EXIT_FAILURE);
    }else if (value <= 0) {
        fprintf(stderr, "Non positive number was inputed\n");
        exit(EXIT_FAILURE);
    }

    return value;
}
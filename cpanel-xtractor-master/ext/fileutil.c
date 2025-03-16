#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileutil.h"

struct LinesList {
    char **lines;
    size_t count;
};

struct LinesList readLinesfromfile(const char *filepath) {
    struct LinesList linesList = {NULL, 0};
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return linesList;
    }

    char buffer[1024]; // Use a fixed-size buffer to read lines
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        linesList.lines = realloc(linesList.lines, (linesList.count + 1) * sizeof(char *));
        linesList.lines[linesList.count] = strdup(buffer);
        linesList.count++;
    }

    fclose(file);

    return linesList;
}

// return int | 1 - file exists | 0 - file does not exist
int if_path_exists(const char *path) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void freeLinesList(struct LinesList *linesList) {
    for (size_t i = 0; i < linesList->count; i++) {
        free(linesList->lines[i]);
    }
    free(linesList->lines);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "main.h"
#include "ext/ui.h"
#include "ext/dirscan.h"
#include "ext/fileutil.h"


struct FileList {
    char **files; // All files in list fileList.files[index]
    char **dirs;
    size_t count_files; // Files detected in directory
    size_t count_dirs; // Files detected in directory
};

struct LinesList {
    char **lines;
    size_t count;
};

void execop() {

    const char *filepath = "./ext/dirscan.c";
    
    if (if_path_exists(filepath)) {
        struct LinesList linesList = readLinesfromfile(filepath);

        for (size_t i = 0; i < linesList.count; i++) {
            printf("%s", linesList.lines[i]);
        }

        freeLinesList(&linesList);
    } else {
        printf("File does not exist: %s\n", filepath);
    }
}


#define MAX_LINE_LENGTH 128
#define MAX_PATH_LENGTH 2048

void extractInfo(FILE *file, FILE *outputFile) {
    char line[MAX_LINE_LENGTH];
    char url[MAX_LINE_LENGTH], username[MAX_LINE_LENGTH], password[MAX_LINE_LENGTH], application[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "URL:") != NULL) {
            strcpy(url, line + 5); // Skip "URL: "
            url[strcspn(url, "\n")] = '\0'; // Remove newline character
        }
        else if (strstr(line, "Username:") != NULL) {
            strcpy(username, line + 10); // Skip "Username: "
            username[strcspn(username, "\n")] = '\0'; // Remove newline character
        }
        else if (strstr(line, "Password:") != NULL) {
            strcpy(password, line + 10); // Skip "Password: "
            password[strcspn(password, "\n")] = '\0'; // Remove newline character
        }
        else if (strstr(line, "Application:") != NULL) {
            strcpy(application, line + 13); // Skip "Application: "
            application[strcspn(application, "\n")] = '\0'; // Remove newline character
        }
    }
    fprintf(outputFile, "%s:%s@%s:%s\n", username, password, url, application);
}

int main() {
    char logs_path[64];
    printf("Path to logs folder: ");
    scanf("%s", logs_path);

    struct FileList fileList = listFiles(logs_path);

    FILE *outputFile = fopen("./extracted.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    for (size_t i = 0; i < fileList.count_dirs; ++i) {
        if (strcmp(fileList.dirs[i], ".") != 0 && strcmp(fileList.dirs[i], "..") != 0) {
            char logs_path_plus_logdir[MAX_PATH_LENGTH];
            strcpy(logs_path_plus_logdir, logs_path);
            strcat(logs_path_plus_logdir, "/");
            strcat(logs_path_plus_logdir, fileList.dirs[i]);
            strcat(logs_path_plus_logdir, "/Passwords.txt");

            if (if_path_exists(logs_path_plus_logdir) == 1) {
                FILE *file = fopen(logs_path_plus_logdir, "r");
                if (file == NULL) {
                    perror("Error opening file");
                    return 1;
                }
                extractInfo(file, outputFile);
                fclose(file);
            }
        }
    }

    fclose(outputFile);

    return 0;
}
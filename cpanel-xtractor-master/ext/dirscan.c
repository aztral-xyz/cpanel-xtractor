#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dirscan.h"

#ifdef _WIN32
#include <windows.h>
#define PATH_SEPARATOR '\\'
#else
#include <dirent.h>
#define PATH_SEPARATOR '/'
#endif

// Define file types
#define FLAG_FILE 8
#define FLAG_DIRECTORY 4

struct FileList {
    char **files;
    char **dirs;
    size_t count_files;
    size_t count_dirs;
};

struct FileList listFiles(const char *path) {
    struct FileList fileList = {NULL, NULL, 0, 0};

#ifdef _WIN32
    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    char searchPath[MAX_PATH];
    sprintf(searchPath, "%s\\*", path);

    hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Unable to open directory\n");
        return fileList;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                fileList.count_dirs++;
                fileList.dirs = realloc(fileList.dirs, fileList.count_dirs * sizeof(char *));
                fileList.dirs[fileList.count_dirs - 1] = strdup(findData.cFileName);
            }
        } else {
            fileList.count_files++;
            fileList.files = realloc(fileList.files, fileList.count_files * sizeof(char *));
            fileList.files[fileList.count_files - 1] = strdup(findData.cFileName);
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
#else
    DIR *directory = opendir(path);
    if (directory == NULL) {
        perror("Unable to open directory");
        return fileList;
    }

    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL) {
        if ((entry->d_type == DT_REG)) {
            fileList.count_files++;
            fileList.files = realloc(fileList.files, fileList.count_files * sizeof(char *));
            fileList.files[fileList.count_files - 1] = strdup(entry->d_name);
        } else if ((entry->d_type == DT_DIR)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                fileList.count_dirs++;
                fileList.dirs = realloc(fileList.dirs, fileList.count_dirs * sizeof(char *));
                fileList.dirs[fileList.count_dirs - 1] = strdup(entry->d_name);
            }
        }
    }

    closedir(directory);
#endif

    return fileList;
}

void freeListFiles(struct FileList *fileList) {
    for (size_t i = 0; i < fileList->count_files; i++) {
        free(fileList->files[i]);
    }
    free(fileList->files);

    for (size_t i = 0; i < fileList->count_dirs; i++) {
        free(fileList->dirs[i]);
    }
    free(fileList->dirs);
}

// int main() {
//     struct FileList fileList = listFiles(".");
//     printf("Files:\n");
//     for (size_t i = 0; i < fileList.count_files; i++) {
//         printf("%s\n", fileList.files[i]);
//     }
//     printf("Directories:\n");
//     for (size_t i = 0; i < fileList.count_dirs; i++) {
//         printf("%s\n", fileList.dirs[i]);
//     }
//     freeListFiles(&fileList);
//     return 0;
// }
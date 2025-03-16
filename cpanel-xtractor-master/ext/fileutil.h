#ifndef FILEUTIL_H_
#define FILEUTIL_H_

struct LinesList readLinesfromfile(const char *filepath);

int if_path_exists(const char *path);

void freeLinesList(struct LinesList *linesList);

void execop();

#endif
#ifndef DIRSCAN_H_
#define DIRSCAN_H_

#define FLAG_FILE 8
#define FLAG_DIRECTORY 4

struct FileList listFiles(const char *path);

#endif
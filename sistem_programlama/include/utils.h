#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Yardımcı fonksiyonlar
int is_text_file(const char *filename);
long get_file_size(const char *filename);
int get_file_permissions(const char *filename);
int set_file_permissions(const char *filename, int permissions);
int is_valid_archive(const char *filename);
char *get_filename_from_path(const char *path);

#endif

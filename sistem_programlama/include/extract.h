#ifndef EXTRACT_H
#define EXTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "archive.h"

// Çıkarma fonksiyonları
int extract_archive(const char *archive_name, const char *extract_dir);
int read_archive_header(FILE *archive_fp, Archive *archive);
int extract_files(FILE *archive_fp, Archive *archive, const char *extract_dir);
int create_directory_recursive(const char *path);

#endif

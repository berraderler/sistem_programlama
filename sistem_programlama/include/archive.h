#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILES 32
#define MAX_SIZE 200 * 1024 * 1024  // 200 MB
#define DEFAULT_ARCHIVE "a.sau"
#define RECORD_SEPARATOR '|'
#define FIELD_SEPARATOR ','

typedef struct {
    char filename[256];
    int permissions;
    long size;
} FileRecord;

typedef struct {
    FileRecord records[MAX_FILES];
    int count;
} Archive;

// Arşivleme fonksiyonları
int create_archive(const char *archive_name, char **input_files, int file_count);
int add_file_to_archive(FILE *archive_fp, const char *filename, long *offset);
int write_archive_header(FILE *archive_fp, FileRecord *records, int count);

#endif

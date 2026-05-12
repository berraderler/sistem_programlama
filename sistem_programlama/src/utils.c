#include "../include/utils.h"
#include <ctype.h>

// Dosyanın metin dosyası olup olmadığını kontrol et
int is_text_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;  // Dosya açılamadı
    }
    
    // Dosyayı okuyabilir mi kontrol et
    int ch;
    int byte_count = 0;
    int has_null = 0;
    
    while ((ch = fgetc(fp)) != EOF && byte_count < 512) {
        // Null byte varsa binary dosyadır
        if (ch == 0) {
            has_null = 1;
            break;
        }
        // Control karakterleri kontrol et (0x00-0x08, 0x0E-0x1F hariç CR/LF/TAB)
        if (ch < 32 && ch != '\n' && ch != '\r' && ch != '\t') {
            has_null = 1;
            break;
        }
        byte_count++;
    }
    
    fclose(fp);
    
    // Eğer null byte var veya hiçbir veri yoksa binary
    if (has_null || byte_count == 0) {
        return 0;
    }
    
    return 1;  // Metin dosyası
}

// Dosya boyutunu al
long get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1;
    }
    return st.st_size;
}

// Dosya izinlerini al
int get_file_permissions(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return -1;
    }
    return st.st_mode & 0777;
}

// Dosya izinlerini ayarla
int set_file_permissions(const char *filename, int permissions) {
    return chmod(filename, permissions);
}

// Arşiv dosyası geçerli mi kontrol et
int is_valid_archive(const char *filename) {
    if (strlen(filename) < 5) return 0;
    
    const char *ext = filename + strlen(filename) - 4;
    return strcmp(ext, ".sau") == 0;
}

// Yoldan dosya adını al
char *get_filename_from_path(const char *path) {
    char *filename = (char *)malloc(strlen(path) + 1);
    strcpy(filename, path);
    
    // Son '/' veya '\' bularak sonrasını al
    char *ptr = strrchr(filename, '/');
    if (!ptr) ptr = strrchr(filename, '\\');
    
    if (ptr) {
        char *result = (char *)malloc(strlen(ptr) + 1);
        strcpy(result, ptr + 1);
        free(filename);
        return result;
    }
    
    return filename;
}

#include "../include/archive.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Arşiv başlığını yaz
int write_archive_header(FILE *archive_fp, FileRecord *records, int count) {
    // Dosya adları, izinler ve boyutları birleştir
    char buffer[16384];
    memset(buffer, 0, sizeof(buffer));
    
    long offset = 0;
    
    for (int i = 0; i < count; i++) {
        char record[512];
        snprintf(record, sizeof(record), "|%s,%o,%ld",
                 records[i].filename,
                 records[i].permissions,
                 records[i].size);
        
        if (offset + strlen(record) >= sizeof(buffer)) {
            fprintf(stderr, "Arşiv başlığı çok büyük!\n");
            return -1;
        }
        
        strcat(buffer, record);
        offset += strlen(record);
    }
    
    // Header boyutunu ilk 10 bayta yaz
    char size_str[32];
    snprintf(size_str, sizeof(size_str), "%010ld", offset);
    fwrite(size_str, 1, 10, archive_fp);
    
    // Header verilerini yaz
    fwrite(buffer, 1, offset, archive_fp);
    
    return 0;
}

// Dosyayı arşive ekle
int add_file_to_archive(FILE *archive_fp, const char *filename, long *offset) {
    // Dosya bilgilerini kontrol et
    if (!is_text_file(filename)) {
        fprintf(stderr, "%s giriş dosyasının formatı uyumsuzdur!\n", filename);
        return -1;
    }
    
    long size = get_file_size(filename);
    if (size < 0) {
        fprintf(stderr, "%s dosyası açılamadı!\n", filename);
        return -1;
    }
    
    // Dosyayı oku ve arşive yaz
    FILE *input_fp = fopen(filename, "rb");
    if (!input_fp) {
        fprintf(stderr, "%s dosyası açılamadı!\n", filename);
        return -1;
    }
    
    char buffer[4096];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input_fp)) > 0) {
        fwrite(buffer, 1, bytes_read, archive_fp);
        *offset += bytes_read;
    }
    
    fclose(input_fp);
    return 0;
}

// Arşiv oluştur
int create_archive(const char *archive_name, char **input_files, int file_count) {
    // Arşiv adını kontrol et
    const char *final_archive_name = archive_name ? archive_name : DEFAULT_ARCHIVE;
    
    // Dosya sayısını kontrol et
    if (file_count > MAX_FILES) {
        fprintf(stderr, "Maksimum %d dosya arşivlenebilir!\n", MAX_FILES);
        return -1;
    }
    
    // Dosyaları okumak için kayıtları hazırla
    FileRecord records[MAX_FILES];
    long total_size = 0;
    
    for (int i = 0; i < file_count; i++) {
        if (!is_text_file(input_files[i])) {
            fprintf(stderr, "%s giriş dosyasının formatı uyumsuzdur!\n", input_files[i]);
            return -1;
        }
        
        long size = get_file_size(input_files[i]);
        if (size < 0) {
            fprintf(stderr, "%s dosyası açılamadı!\n", input_files[i]);
            return -1;
        }
        
        total_size += size;
        
        if (total_size > MAX_SIZE) {
            fprintf(stderr, "Toplam dosya boyutu 200 MB'ı geçiyor!\n");
            return -1;
        }
        
        strcpy(records[i].filename, get_filename_from_path(input_files[i]));
        records[i].permissions = get_file_permissions(input_files[i]);
        records[i].size = size;
    }
    
    // Arşiv dosyasını oluştur
    FILE *archive_fp = fopen(final_archive_name, "wb");
    if (!archive_fp) {
        fprintf(stderr, "Arşiv dosyası oluşturulamadı!\n");
        return -1;
    }
    
    // Başlığı yaz
    if (write_archive_header(archive_fp, records, file_count) != 0) {
        fclose(archive_fp);
        return -1;
    }
    
    // Dosyaları arşive ekle
    long offset = 0;
    for (int i = 0; i < file_count; i++) {
        if (add_file_to_archive(archive_fp, input_files[i], &offset) != 0) {
            fclose(archive_fp);
            remove(final_archive_name);
            return -1;
        }
    }
    
    fclose(archive_fp);
    printf("Arşiv başarıyla oluşturuldu: %s\n", final_archive_name);
    
    return 0;
}

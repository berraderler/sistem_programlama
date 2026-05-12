#include "../include/extract.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#endif

// Dizin oluştur (özyinelemeli)
int create_directory_recursive(const char *path) {
    char temp[512];
    strcpy(temp, path);
    
    // Windows ve Linux uyumluluğu
    for (int i = 0; temp[i]; i++) {
        if (temp[i] == '\\') temp[i] = '/';
    }
    
    for (int i = 1; temp[i]; i++) {
        if (temp[i] == '/') {
            temp[i] = '\0';
            mkdir(temp, 0755);
            temp[i] = '/';
        }
    }
    
    return mkdir(temp, 0755);
}

// Arşiv başlığını oku
int read_archive_header(FILE *archive_fp, Archive *archive) {
    char size_str[11];
    
    // İlk 10 bayttan başlık boyutunu oku
    if (fread(size_str, 1, 10, archive_fp) != (size_t)10) {
        fprintf(stderr, "Arşiv dosyası geçerli değildir!\n");
        return -1;
    }
    
    size_str[10] = '\0';
    long header_size = atol(size_str);
    
    if (header_size <= 0 || header_size > 100000) {
        fprintf(stderr, "Arşiv dosyası geçerli değildir!\n");
        return -1;
    }
    
    // Başlık verilerini oku
    char *header_data = (char *)malloc(header_size + 1);
    if (fread(header_data, 1, header_size, archive_fp) != (size_t)header_size) {
        fprintf(stderr, "Arşiv dosyası bozuk!\n");
        free(header_data);
        return -1;
    }
    header_data[header_size] = '\0';
    
    // Kayıtları parse et
    archive->count = 0;
    char *ptr = header_data;
    
    while (*ptr && archive->count < MAX_FILES) {
        if (*ptr == '|') {
            ptr++;
            
            // Dosya adını çıkar
            char *comma1 = strchr(ptr, ',');
            if (!comma1) break;
            
            int name_len = comma1 - ptr;
            strncpy(archive->records[archive->count].filename, ptr, name_len);
            archive->records[archive->count].filename[name_len] = '\0';
            
            // İzinleri çıkar
            ptr = comma1 + 1;
            char *comma2 = strchr(ptr, ',');
            if (!comma2) break;
            
            int perm_len = comma2 - ptr;
            char perm_str[16];
            strncpy(perm_str, ptr, perm_len);
            perm_str[perm_len] = '\0';
            archive->records[archive->count].permissions = strtol(perm_str, NULL, 8);
            
            // Boyutu çıkar
            ptr = comma2 + 1;
            char *pipe = strchr(ptr, '|');
            if (!pipe) {
                // Son kayıt
                archive->records[archive->count].size = atol(ptr);
                archive->count++;
                break;
            }
            
            int size_len = pipe - ptr;
            char size_str[32];
            strncpy(size_str, ptr, size_len);
            size_str[size_len] = '\0';
            archive->records[archive->count].size = atol(size_str);
            
            archive->count++;
            ptr = pipe;
        } else {
            ptr++;
        }
    }
    
    free(header_data);
    return 0;
}

// Dosyaları arşivden çıkar
int extract_files(FILE *archive_fp, Archive *archive, const char *extract_dir) {
    for (int i = 0; i < archive->count; i++) {
        char filepath[512];
        
        if (extract_dir) {
            snprintf(filepath, sizeof(filepath), "%s/%s", extract_dir, archive->records[i].filename);
        } else {
            strcpy(filepath, archive->records[i].filename);
        }
        
        // Dosya yolunda dizin oluştur
        char dirpath[512];
        strcpy(dirpath, filepath);
        char *last_slash = strrchr(dirpath, '/');
        if (last_slash) {
            *last_slash = '\0';
            create_directory_recursive(dirpath);
        }
        
        // Dosyayı oluştur ve yaz
        FILE *output_fp = fopen(filepath, "wb");
        if (!output_fp) {
            fprintf(stderr, "%s dosyası oluşturulamadı!\n", filepath);
            return -1;
        }
        
        // Dosya içeriğini oku ve yaz
        char buffer[4096];
        long remaining = archive->records[i].size;
        
        while (remaining > 0) {
            size_t to_read = remaining > (long)sizeof(buffer) ? sizeof(buffer) : (size_t)remaining;
            size_t bytes_read = fread(buffer, 1, to_read, archive_fp);
            
            if (bytes_read == 0) {
                fprintf(stderr, "Arşiv dosyası prematüre sona erdi!\n");
                fclose(output_fp);
                return -1;
            }
            
            fwrite(buffer, 1, bytes_read, output_fp);
            remaining -= bytes_read;
        }
        
        fclose(output_fp);
        
        // İzinleri ayarla
        set_file_permissions(filepath, archive->records[i].permissions);
    }
    
    return 0;
}

// Arşivi çıkar
int extract_archive(const char *archive_name, const char *extract_dir) {
    // Arşiv dosyasını kontrol et
    if (!is_valid_archive(archive_name)) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        return -1;
    }
    
    // Arşiv dosyasını aç
    FILE *archive_fp = fopen(archive_name, "rb");
    if (!archive_fp) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        return -1;
    }
    
    Archive archive;
    archive.count = 0;
    
    // Başlığı oku
    if (read_archive_header(archive_fp, &archive) != 0) {
        fclose(archive_fp);
        return -1;
    }
    
    // Çıkış dizinini oluştur (gerekirse)
    if (extract_dir) {
        if (create_directory_recursive(extract_dir) != 0 && errno != EEXIST) {
            // Hata - ancak EEXIST sorun değil
        }
    }
    
    // Dosyaları çıkar
    if (extract_files(archive_fp, &archive, extract_dir) != 0) {
        fclose(archive_fp);
        return -1;
    }
    
    fclose(archive_fp);
    printf("Arşiv başarıyla açıldı: %s\n", archive_name);
    
    return 0;
}

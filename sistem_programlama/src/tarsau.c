#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/archive.h"
#include "../include/extract.h"

void print_usage(const char *program_name) {
    printf("Kullanım:\n");
    printf("  %s -b [options] file1 file2 ... fileN\n", program_name);
    printf("  %s -a archive.sau [extract_dir]\n", program_name);
    printf("\nSeçenekler:\n");
    printf("  -b              Dosyaları arşivle\n");
    printf("  -a              Arşivden çıkar\n");
    printf("  -o FILE         Arşiv dosyası adı (varsayılan: a.sau)\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // -b modu: Arşivleme
    if (strcmp(argv[1], "-b") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Hata: Arşivlenecek dosyalar belirtilmedi!\n");
            print_usage(argv[0]);
            return 1;
        }
        
        const char *archive_name = NULL;
        char *input_files[MAX_FILES];
        int file_count = 0;
        
        // Parametreleri parse et
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    archive_name = argv[++i];
                } else {
                    fprintf(stderr, "Hata: -o sonrasında dosya adı belirtilmedi!\n");
                    return 1;
                }
            } else {
                if (file_count >= MAX_FILES) {
                    fprintf(stderr, "Hata: Maksimum %d dosya arşivlenebilir!\n", MAX_FILES);
                    return 1;
                }
                input_files[file_count++] = argv[i];
            }
        }
        
        if (file_count == 0) {
            fprintf(stderr, "Hata: Arşivlenecek dosyalar belirtilmedi!\n");
            return 1;
        }
        
        // Arşiv oluştur
        if (create_archive(archive_name, input_files, file_count) != 0) {
            return 1;
        }
        
        return 0;
    }
    
    // -a modu: Çıkarma
    else if (strcmp(argv[1], "-a") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Hata: Arşiv dosyası adı belirtilmedi!\n");
            print_usage(argv[0]);
            return 1;
        }
        
        const char *archive_name = argv[2];
        const char *extract_dir = (argc > 3) ? argv[3] : NULL;
        
        // Arşivi çıkar
        if (extract_archive(archive_name, extract_dir) != 0) {
            return 1;
        }
        
        return 0;
    }
    
    else {
        fprintf(stderr, "Hata: Bilinmeyen seçenek '%s'\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }
    
    return 0;
}

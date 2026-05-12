# tarsau - Arşivleme Programı

Sistem Programlama 2025-2026 Bahar Dönemi Projesi

## Proje Açıklaması

**tarsau**, tar, rar veya zip gibi işlev gören ancak sıkıştırma yapmayan bir arşivleme programıdır. Program Linux/Unix işletim sisteminde C dilinde yazılmış olup, komut satırı arayüzü ile çalışır.

## Özellikler

- ✓ Metin dosyalarını arşivleme (.sau formatında)
- ✓ Arşivden dosya çıkarma
- ✓ Dosya izinlerinin korunması
- ✓ Maksimum 32 dosya desteği
- ✓ 200 MB toplam boyut sınırı
- ✓ Hata yönetimi ve validasyon

## Kurulum

### Gereksinimler
- GCC derleyici
- Linux/Unix işletim sistemi
- Make

### Derleme

```bash
make
```

Derlenmiş program `bin/tarsau` konumunda olacaktır.

### Yükleme (isteğe bağlı)

```bash
make install
```

## Kullanım

### Arşivleme (-b modu)

```bash
./bin/tarsau -b file1.txt file2.txt file3.txt -o archive.sau
```

**Parametreler:**
- `-b`: Arşivleme modunu etkinleştir
- `-o FILE`: Arşiv dosyası adını belirt (varsayılan: a.sau)

**Örnek:**
```bash
./bin/tarsau -b t1 t2 t3 t4.txt t5.dat -o myarchive.sau
./bin/tarsau -b *.txt  # Varsayılan a.sau kullanır
```

### Çıkarma (-a modu)

```bash
./bin/tarsau -a archive.sau [extract_dir]
```

**Parametreler:**
- `-a`: Çıkarma modunu etkinleştir
- `archive.sau`: Arşiv dosyası adı (*.sau)
- `[extract_dir]`: Hedef dizin (isteğe bağlı, varsayılan: geçerli dizin)

**Örnek:**
```bash
./bin/tarsau -a archive.sau              # Geçerli dizine çıkar
./bin/tarsau -a archive.sau ./extracted  # ./extracted klasörüne çıkar
```

## .sau Dosya Formatı

Arşiv dosyası iki bölümden oluşur:

### 1. İçerik Bilgileri (Header)
```
[10 byte: boyut] | [dosya_adı,izinler,boyut] | [dosya_adı,izinler,boyut] ...
```

- İlk 10 bayt: Başlık bölümünün boyutu (ASCII formatında)
- Kayıtlar: '|' karakteri ile ayrılır
- Alanlar: Virgülle ayrılmış (dosya_adı, izinler, boyut)

### 2. Arşivlenmiş Dosyalar
- Dosya içeriği ASCII formatında, ardışık olarak yerleştirilir
- Ayırıcı kullanılmaz

## Kısıtlamalar

- Giriş dosyaları: ASCII metin dosyaları olmalı
- Maksimum dosya sayısı: 32
- Maksimum toplam boyut: 200 MB
- Arşiv dosyası formatı: .sau uzantısı

## Hata Yönetimi

Program aşağıdaki hatalarda uygun mesajlar gösterir:

```
"[dosya] giriş dosyasının formatı uyumsuzdur!" - Metin olmayan dosya
"Arşiv dosyası uygunsuz veya bozuk!" - Geçersiz arşiv dosyası
"Maksimum 32 dosya arşivlenebilir!" - Çok fazla dosya
"Toplam dosya boyutu 200 MB'ı geçiyor!" - Boyut limiti aşıldı
```

## Proje Yapısı

```
sistem_programlama/
├── src/
│   ├── tarsau.c       # Ana program
│   ├── archive.c      # Arşivleme fonksiyonları
│   ├── extract.c      # Çıkarma fonksiyonları
│   └── utils.c        # Yardımcı fonksiyonlar
├── include/
│   ├── archive.h      # Arşivleme başlıkları
│   ├── extract.h      # Çıkarma başlıkları
│   └── utils.h        # Yardımcı başlıkları
├── Makefile           # Derleme yapılandırması
├── README.md          # Bu dosya
└── .gitignore         # Git yok sayma kuralları
```

## Temizleme

```bash
make clean
```

## Katkılar

Bu proje Bilgisayar Mühendisliği Sistem Programlama dersi için geliştirilmiştir.

## Lisans

Bu proje eğitim amaçlı olarak geliştirilmiştir.

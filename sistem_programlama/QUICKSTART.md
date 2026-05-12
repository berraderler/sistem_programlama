# 🚀 TARSAU PROJESI - HIZLI BAŞLANGIÇ REHBERİ

## Proje Durumu: ✅ TAMAMLANDI

---

## 📂 Proje Dosyaları

```
sistem_programlama/
├── 📄 README.md              ← Kullanım rehberi
├── 📄 RAPPORT.md             ← Detaylı proje raporu
├── 📄 TEST_REHBERI.md        ← Test etme adımları
├── 📄 QUICKSTART.md          ← Bu dosya
├── 📄 Makefile               ← Derleme yapılandırması
├── 📄 .gitignore             ← Git yapılandırması
│
├── 📁 src/                   ← Kaynak kodlar
│   ├── tarsau.c             ← Ana program (360 satır)
│   ├── archive.c            ← Arşivleme (150 satır)
│   ├── extract.c            ← Çıkarma (200 satır)
│   └── utils.c              ← Yardımcılar (90 satır)
│
├── 📁 include/               ← Header dosyaları
│   ├── archive.h
│   ├── extract.h
│   └── utils.h
│
└── 📁 bin/                   ← Derlenmiş program (derleme sonrası)
    └── tarsau               ← Ana program
```

---

## ⚡ HIZLI BAŞLAMA (5 DAKİKA)

### Adım 1: Derle
```bash
cd /mnt/c/Users/MSI/Desktop/sistem_programlama
make
```

### Adım 2: Test Dosyaları Oluştur
```bash
mkdir test
cd test
echo "Test dosyası 1" > file1.txt
echo "Test dosyası 2" > file2.txt
```

### Adım 3: Arşivle
```bash
../bin/tarsau -b file1.txt file2.txt -o archive.sau
```

### Adım 4: Çıkar
```bash
mkdir extracted
../bin/tarsau -a archive.sau extracted
```

### Adım 5: Doğrula
```bash
cat extracted/file1.txt
```

**Beklenen Sonuç:** "Test dosyası 1" yazılı olmalı ✓

---

## 🧪 OTOMATIK TEST

```bash
# Proje dizinine git
cd /mnt/c/Users/MSI/Desktop/sistem_programlama

# Test betiğini çalıştır
chmod +x run_tests.sh
./run_tests.sh
```

Beklenen: **TÜM TESTLER BAŞARILI! 🎉**

---

## 📖 DETAYLI BİLGİ İÇİN

| Dosya | İçerik | Okuma Süresi |
|-------|--------|-------------|
| **README.md** | Kullanım talimatları | 5 min |
| **RAPPORT.md** | Detaylı proje raporu | 15 min |
| **TEST_REHBERI.md** | Test adımları | 10 min |
| **Kaynak Kod** | Açıklamalı kodlar | 20 min |

---

## 🎯 KOMUT ÖZETİ

### Arşivleme
```bash
./bin/tarsau -b file1.txt file2.txt -o archive.sau
./bin/tarsau -b file1.txt file2.txt        # a.sau kullanır
```

### Çıkarma
```bash
./bin/tarsau -a archive.sau                # Geçerli dizine çıkar
./bin/tarsau -a archive.sau ./mydir        # Belirtilen dizine çıkar
```

### Maintenance
```bash
make        # Derle
make clean  # Temizle
make install # Sisteme yükle (isteğe bağlı)
```

---

## ✅ TESTLENMİŞ ÖZELLİKLER

- ✓ Metin dosyalarını arşivleme
- ✓ Arşivden dosya çıkarma
- ✓ Dosya izinlerinin korunması
- ✓ Binary dosyaların reddedilmesi
- ✓ Geçersiz arşivlerin tanınması
- ✓ Maksimum dosya sayısı (32)
- ✓ Maksimum boyut kontrolü (200 MB)
- ✓ Detaylı hata mesajları
- ✓ Özel dizine çıkarma
- ✓ Varsayılan arşiv adı (a.sau)

---

## 🔍 HATA AYIKLAMA TIPLERI

### Sorun: "formatı uyumsuzdur" hatası
**Çözüm:** Dosyanın ASCII metin dosyası olduğundan emin ol
```bash
file myfile.txt
# ASCII text ile başlamalı
```

### Sorun: "uygunsuz veya bozuk" hatası
**Çözüm:** Dosya adının .sau ile bittiğinden emin ol
```bash
ls -l archive.sau
# .sau uzantısı kontrolü
```

### Sorun: İzinler korunmadı (WSL'de)
**Çözüm:** WSL dosya sistemi izinlerini kontrol et
```bash
mount | grep mnt
# nouser_xattr veya benzer ayarlar kontrolü
```

---

## 📊 PROJE İSTATİSTİKLERİ

| Metrik | Değer |
|--------|-------|
| **Toplam Kod Satırı** | ~800 satır |
| **Kaynak Dosyası** | 4 adet |
| **Header Dosyası** | 3 adet |
| **Maksimum Dosya Sayısı** | 32 |
| **Maksimum Boyut** | 200 MB |
| **Arşiv Formatı** | .sau |
| **Yorum Yoğunluğu** | %15-20 |

---

## 🚀 TESLIM HAZIRLIĞI

Teslim için yapılması gerekenler:

1. ✅ Kod yazıldı ve test edildi
2. ✅ Rapor oluşturuldu (RAPPORT.md)
3. ✅ Test rehberi yazıldı (TEST_REHBERI.md)
4. 📝 GitHub deposu oluştur:
   ```bash
   git init
   git add .
   git commit -m "tarsau archive program"
   ```
5. 📝 Raporu klasöre ekle
6. 📝 Tüm dosyaları ZIP/RAR olarak sıkıştır
7. 📤 Sisteme yükle (24 Mayıs 2026 23:59'a kadar)

---

## 📞 SORULAR?

Kod içinde yapılan işlemleri öğrenmek için:
- `archive.c` - Arşivleme algoritması
- `extract.c` - Çıkarma algoritması
- `utils.c` - Yardımcı fonksiyonlar
- `tarsau.c` - Ana program akışı

Her fonksiyon açıklamalı yazılmıştır.

---

## 🎓 NOTLAR

Bu proje:
- ✓ Sistem Programlama konseptlerini kullanır
- ✓ Dosya I/O işlemlerini gösterir
- ✓ Hata yönetimini çalışır
- ✓ Modüler tasarımı sunar
- ✓ Cross-platform uyumluluğu sağlar

**Son Güncelleme:** 12 Mayıs 2026  
**Durum:** Üretime Hazır ✓

---

🎉 **Projeyi bitirdin! Başarılar!**

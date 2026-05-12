# TARSAU TEST REHBERI - ADIM ADIM

## 📋 İçerik
1. Ortam Hazırlığı
2. Derleme
3. Manuel Test Adımları
4. Otomatik Test Çalıştırma
5. Hata Senaryoları

---

## 1. ORTAM HAZIRLIĞI

### 1.1 Windows'ta WSL Kullanarak

```powershell
# PowerShell'i yönetici olarak açın
# Sonra WSL başlatın:
wsl

# Proje dizinine gidiyoruç
cd /mnt/c/Users/MSI/Desktop/sistem_programlama

# Linux ortamında çalışmaya devam edin
```

### 1.2 Doğrudan Linux/Unix'te

```bash
cd ~/sistem_programlama
# veya
cd /path/to/sistem_programlama
```

---

## 2. DERLEME

### 2.1 Projeyi Temizle ve Derle

```bash
# Eski derleme dosyalarını temizle
make clean

# Çıktı:
# ✓ Temizlik tamamlandı
```

### 2.2 Yeni Derleme

```bash
# Projeyi derle
make

# Çıktı örneği:
# ✓ Derlendi: src/utils.c
# ✓ Derlendi: src/archive.c
# ✓ Derlendi: src/extract.c
# ✓ Derlendi: src/tarsau.c
# ✓ Derleme tamamlandı: bin/tarsau
```

### 2.3 Derlemenin Başarılı Olduğunu Kontrol Et

```bash
# Programın var olup olmadığını kontrol et
ls -lh bin/tarsau

# Çıktı:
# -rwxr-xr-x 1 user user 45K May 12 14:35 bin/tarsau

# Programı çalıştırabilir mi kontrol et
./bin/tarsau

# Çıktı (kullanım mesajı):
# Kullanım:
#   ./bin/tarsau -b [options] file1 file2 ... fileN
#   ./bin/tarsau -a archive.sau [extract_dir]
```

---

## 3. MANUEL TEST ADIMLARI

### 3.1 Test Dosyaları Oluştur

```bash
# Test dizini oluştur
mkdir -p test_environment
cd test_environment

# Test dosyalarını oluştur
cat > file1.txt << 'EOF'
Bu birinci test dosyasıdır.
Dönem: Bahar 2025-2026
Ders: Sistem Programlama
İşletim Sistemi: Linux
EOF

cat > file2.txt << 'EOF'
İkinci test dosyası
Satır 2
Satır 3
Satır 4
Satır 5
EOF

cat > file3.txt << 'EOF'
Üçüncü dosya - Yapılandırma
username=student
password=1234
server=localhost
port=8080
EOF

# Doğrulama
echo "Test dosyaları oluşturuldu:"
ls -lh file*.txt
```

---

### 3.2 TEST 1: Basit Arşivleme

```bash
# Arşivi oluştur
../bin/tarsau -b file1.txt file2.txt -o myarchive.sau

# Beklenen Çıktı:
# Arşiv başarıyla oluşturuldu: myarchive.sau
```

**Doğrulama:**

```bash
# Arşiv dosyasının boyutunu kontrol et
ls -lh myarchive.sau

# Arşiv dosya içeriğini incele (ilk 200 karakter)
head -c 200 myarchive.sau | od -c | head -20
```

---

### 3.3 TEST 2: Varsayılan Arşiv Adı

```bash
# -o parametresi olmadan çalıştır
../bin/tarsau -b file1.txt file3.txt

# Beklenen Çıktı:
# Arşiv başarıyla oluşturuldu: a.sau

# Doğrula
ls -lh a.sau
```

---

### 3.4 TEST 3: Arşivden Çıkarma (Temel)

```bash
# Çıkış dizini oluştur
mkdir -p extracted

# Arşivden çıkar
../bin/tarsau -a myarchive.sau extracted

# Beklenen Çıktı:
# Arşiv başarıyla açıldı: myarchive.sau

# Çıkarılan dosyaları listele
echo "Çıkarılan dosyalar:"
ls -lh extracted/
```

---

### 3.5 TEST 4: Dosya İçeriğinin Doğrulanması

```bash
# Orijinal ve çıkarılan dosyaları karşılaştır
echo "=== ORIJINAL file1.txt ==="
cat file1.txt

echo ""
echo "=== ÇIKARILAN file1.txt ==="
cat extracted/file1.txt

# İkili karşılaştırma (tam doğrulama)
if cmp -s file1.txt extracted/file1.txt; then
    echo "✓ Dosyalar özdeş"
else
    echo "✗ Dosyalar farklı"
fi
```

---

### 3.6 TEST 5: Çoklu Dosya Arşivleme

```bash
# 3 dosyayı arşivle
../bin/tarsau -b file1.txt file2.txt file3.txt -o all_files.sau

# Beklenen Çıktı:
# Arşiv başarıyla oluşturuldu: all_files.sau

# Arşiv boyutunu kontrol et
echo "Arşiv boyutu:"
ls -lh all_files.sau

echo ""
echo "Orijinal dosyaların toplam boyutu:"
du -sh . | grep -v "^\." | head -1
```

---

### 3.7 TEST 6: Özel Çıkış Dizini

```bash
# Özel dizine çıkar
mkdir -p my_extracted_files
../bin/tarsau -a all_files.sau my_extracted_files

# Doğrula
echo "Çıkarılan dosyalar:"
ls -lh my_extracted_files/
```

---

### 3.8 TEST 7: Dosya İzinlerinin Korunması

```bash
# Orijinal dosyaya özel izin ata
chmod 600 file1.txt
ls -l file1.txt

# Yeni arşiv oluştur
rm -rf extracted2
mkdir extracted2
../bin/tarsau -b file1.txt -o perms_test.sau
../bin/tarsau -a perms_test.sau extracted2

# İzinleri kontrol et
echo "Orijinal dosya izinleri:"
ls -l file1.txt

echo ""
echo "Çıkarılan dosya izinleri:"
ls -l extracted2/file1.txt
```

---

## 4. HATA TEST SENARYOLARI

### 4.1 Hata: Binary Dosya Arşivleme

```bash
# Binary dosya oluştur (PNG başlığı)
printf '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a' > image.png

# Arşivleme dene
../bin/tarsau -b image.png

# Beklenen Çıktı:
# image.png giriş dosyasının formatı uyumsuzdur!

# Exit code kontrol et
echo "Exit Code: $?"
# Beklenen: 1 (hata)
```

---

### 4.2 Hata: Geçersiz Arşiv Dosyası Açma

```bash
# Sahte arşiv dosyası oluştur
echo "Bu bir arşiv değildir" > fake.sau

# Açmayı dene
../bin/tarsau -a fake.sau

# Beklenen Çıktı:
# Arşiv dosyası uygunsuz veya bozuk!
```

---

### 4.3 Hata: Yanlış Uzantı

```bash
# .txt uzantısıyla arşiv dosyası oluştur
cp myarchive.sau archive.txt

# Açmayı dene
../bin/tarsau -a archive.txt

# Beklenen Çıktı:
# Arşiv dosyası uygunsuz veya bozuk!
```

---

### 4.4 Hata: Olmayan Dosya

```bash
# Olmayan dosyayı arşivlemeye çalış
../bin/tarsau -b nonexistent.txt

# Beklenen Çıktı:
# nonexistent.txt dosyası açılamadı!
```

---

## 5. OTOMATIK TEST ÇALIŞTIRILMASI

### 5.1 Test Betiğini Çalıştır

```bash
# Proje root dizinine dön
cd ..

# Test betiğine çalıştırma izni ver
chmod +x run_tests.sh

# Testleri çalıştır
./run_tests.sh
```

### 5.2 Beklenen Çıktı

```
================================================
TARSAU ARŞIVLEME PROGRAMI - KAPSAMLI TEST
================================================

[DERLEME] Program derleniyoring...
✓ Derleme başarıyla tamamlandı

[HAZIRLIK] Test dosyaları oluşturuluyor...
✓ Test dosyaları oluşturuldu

[TEST 1] Basit Arşivleme (-b)...
✓ Arşiv dosyası oluşturuldu

[TEST 2] Varsayılan Arşiv Adı (a.sau)...
✓ Varsayılan ad kullanıldı (a.sau)

[TEST 3] Arşivden Çıkarma (-a)...
✓ Dosyalar başarıyla çıkarıldı

[TEST 4] Dosya İçeriği Doğrulama...
✓ Çıkarılan dosya orijinalı ile özdeş

[TEST 5] Çoklu Dosya Arşivleme...
✓ Çoklu dosya arşivi oluşturuldu (234 byte)

[TEST 6] Hatalı Dosya Formatı (Binary Dosya)...
✓ Binary dosya reddedildi (doğru hata mesajı)

[TEST 7] Geçersiz Arşiv Dosyası Açma...
✓ Geçersiz arşiv reddedildi

[TEST 8] Çıkarılan Dosyaların Doğrulanması...
✓ Tüm 4 dosya doğrulandı

[TEST 9] Özel Çıkış Dizini...
✓ Dosyalar belirtilen dizine çıkarıldı

[TEST 10] Arşiv Boyutu Hesaplaması...
✓ Arşiv boyutu mantıklı (Header + Verileri)
   Orijinal: 245 byte
   Arşiv: 285 byte

================================================
TEST SONUÇLARI
================================================
✓ Başarılı: 10
✗ Başarısız: 0
Toplam: 10

TÜM TESTLER BAŞARILI! 🎉
```

---

## 6. PERFORMANS TEST (İsteğe Bağlı)

### 6.1 Büyük Dosya Testi

```bash
# 50 MB test dosyası oluştur
dd if=/dev/urandom of=large_text.txt bs=1M count=50 2>/dev/null

# İçeriğini ASCII yap (seçenek: sadece metin karakterleri)
# Veya metin dosyası kullan
cat /etc/hosts /etc/hosts /etc/hosts ... > large_text.txt

# Arşivle
time ./bin/tarsau -b large_text.txt -o large.sau

# Çıkar
time ./bin/tarsau -a large.sau extracted_large/
```

---

## 7. DEĞERLENDİRME KONTROL LİSTESİ

Testleri tamamladıktan sonra kontrol etmeniz gerekenler:

- [ ] Derleme hatasız yapıldı
- [ ] Basit arşivleme çalışıyor
- [ ] Arşivden çıkarma çalışıyor
- [ ] Dosya içeriği özdeş
- [ ] Varsayılan ad (a.sau) kullanıldı
- [ ] Binary dosyalar reddedildi
- [ ] Geçersiz arşivler tanındı
- [ ] Dosya izinleri korundu
- [ ] Tüm testler başarılı
- [ ] Hata mesajları doğru gösterildi

---

## 8. SÖZLEŞMELİ HATA AYIKLAMA

### Problem: Program derlenmiyor

**Çözüm:**
```bash
# Gerekli paketleri yükle (Ubuntu/Debian)
sudo apt-get install build-essential gcc make

# Makefile'ı kontrol et
make --version

# Tekrar derle
make clean && make
```

### Problem: "formatı uyumsuzdur" hatası metin dosyasında

**Çözüm:**
```bash
# Dosya formatını kontrol et
file myfile.txt

# İçeriği hex olarak kontrol et (0-255 arası kontrol et)
od -x myfile.txt | head -5
```

### Problem: İzinler korunmuyor

**Çözüm:**
```bash
# Dosya sistem izin desteğini kontrol et
mount | grep -E "rw|ro"

# WSL'de izin sorunuysa:
# /etc/wsl.conf dosyasında ayarları kontrol et
```

---

## 9. SONUÇ

Tüm testler başarıyla geçtikten sonra:

1. ✓ Proje, tüm gereksinimleri karşılıyor
2. ✓ Kod kalitesi ve hata yönetimi iyi
3. ✓ Program üretime hazır

Başarılar! 🎉

---

**Son Güncelleme:** 12 Mayıs 2026  
**Test Editörü:** GitHub Copilot

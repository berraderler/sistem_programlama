#!/bin/bash

# ===============================================
# TARSAU KAPSAMLI TEST BETIĞI
# ===============================================

set -e  # Hata olursa kur

FAILED=0
PASSED=0

# Renk tanımlamaları
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Dizin temizleme ve hazırlama
cleanup() {
    rm -rf test_archive* extracted* test_output* binary_test*
}

# Test başlat
echo "================================================"
echo "TARSAU ARŞIVLEME PROGRAMI - KAPSAMLI TEST"
echo "================================================"
echo ""

# Projeyi derle
echo -e "${YELLOW}[DERLEME]${NC} Program derleniyoring..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo -e "${GREEN}✓${NC} Derleme başarıyla tamamlandı"
echo ""

# Test dosyaları oluştur
echo -e "${YELLOW}[HAZIRLIK]${NC} Test dosyaları oluşturuluyor..."
cleanup

# Basit metin dosyaları
echo "Bu test1 dosyasıdır.
Birinci satır.
İkinci satır.
Üçüncü satır." > test_file1.txt

echo "Bu test2 dosyasıdır.
Dördüncü satır.
Beşinci satır." > test_file2.txt

echo "Üçüncü test dosyası
Altıncı satır
Yedinci satır
Sekizinci satır
Dokuzuncu satır" > test_file3.txt

echo "Birden fazla satırlı test dosyası" > test_file4.txt

# İkili (binary) test dosyası
printf '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a' > binary_test.bin

echo -e "${GREEN}✓${NC} Test dosyaları oluşturuldu"
echo ""

# ===============================================
# TEST 1: Basit Arşivleme
# ===============================================
echo -e "${YELLOW}[TEST 1]${NC} Basit Arşivleme (-b)..."
if ./bin/tarsau -b test_file1.txt test_file2.txt -o test_archive_simple.sau 2>&1 | grep -q "başarıyla oluşturuldu"; then
    if [ -f test_archive_simple.sau ]; then
        echo -e "${GREEN}✓${NC} Arşiv dosyası oluşturuldu"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}✗${NC} Arşiv dosyası bulunamadı"
        FAILED=$((FAILED+1))
    fi
else
    echo -e "${RED}✗${NC} Arşivleme başarısız"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 2: Varsayılan Arşiv Adı
# ===============================================
echo -e "${YELLOW}[TEST 2]${NC} Varsayılan Arşiv Adı (a.sau)..."
if ./bin/tarsau -b test_file1.txt 2>&1 | grep -q "a.sau"; then
    if [ -f a.sau ]; then
        echo -e "${GREEN}✓${NC} Varsayılan ad kullanıldı (a.sau)"
        PASSED=$((PASSED+1))
        rm -f a.sau
    else
        echo -e "${RED}✗${NC} Varsayılan dosya oluşturulamadı"
        FAILED=$((FAILED+1))
    fi
else
    echo -e "${RED}✗${NC} Varsayılan adı kontrol edemedi"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 3: Arşivden Çıkarma
# ===============================================
echo -e "${YELLOW}[TEST 3]${NC} Arşivden Çıkarma (-a)..."
mkdir -p extracted_simple
if ./bin/tarsau -a test_archive_simple.sau extracted_simple 2>&1 | grep -q "başarıyla açıldı"; then
    if [ -f extracted_simple/test_file1.txt ] && [ -f extracted_simple/test_file2.txt ]; then
        echo -e "${GREEN}✓${NC} Dosyalar başarıyla çıkarıldı"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}✗${NC} Dosyalar çıkarılamadı"
        FAILED=$((FAILED+1))
    fi
else
    echo -e "${RED}✗${NC} Çıkarma işlemi başarısız"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 4: Dosya İçeriği Doğrulama
# ===============================================
echo -e "${YELLOW}[TEST 4]${NC} Dosya İçeriği Doğrulama..."
if cmp -s test_file1.txt extracted_simple/test_file1.txt; then
    echo -e "${GREEN}✓${NC} Çıkarılan dosya orijinalı ile özdeş"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}✗${NC} Dosya içeriği farklı"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 5: Çoklu Dosya Arşivleme
# ===============================================
echo -e "${YELLOW}[TEST 5]${NC} Çoklu Dosya Arşivleme..."
if ./bin/tarsau -b test_file1.txt test_file2.txt test_file3.txt test_file4.txt -o test_archive_multi.sau 2>&1 | grep -q "başarıyla oluşturuldu"; then
    if [ -f test_archive_multi.sau ]; then
        FILE_SIZE=$(du -b test_archive_multi.sau | cut -f1)
        echo -e "${GREEN}✓${NC} Çoklu dosya arşivi oluşturuldu (${FILE_SIZE} byte)"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}✗${NC} Çoklu arşiv oluşturulamadı"
        FAILED=$((FAILED+1))
    fi
else
    echo -e "${RED}✗${NC} Çoklu arşivleme başarısız"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 6: Hatalı Dosya Formatı
# ===============================================
echo -e "${YELLOW}[TEST 6]${NC} Hatalı Dosya Formatı (Binary Dosya)..."
if ./bin/tarsau -b binary_test.bin 2>&1 | grep -q "formatı uyumsuzdur"; then
    echo -e "${GREEN}✓${NC} Binary dosya reddedildi (doğru hata mesajı)"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}✗${NC} Binary dosya hatasını algılamadı"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 7: Geçersiz Arşiv Dosyası
# ===============================================
echo -e "${YELLOW}[TEST 7]${NC} Geçersiz Arşiv Dosyası Açma..."
echo "Bu arşiv değildir" > invalid.sau
if ./bin/tarsau -a invalid.sau 2>&1 | grep -q "uygunsuz\|bozuk\|geçerli"; then
    echo -e "${GREEN}✓${NC} Geçersiz arşiv reddedildi"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}✗${NC} Geçersiz arşiv kontrol edilemedi"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 8: Çıkarılan Dosyaların Doğrulanması
# ===============================================
echo -e "${YELLOW}[TEST 8]${NC} Çıkarılan Dosyaların Doğrulanması..."
mkdir -p extracted_multi
./bin/tarsau -a test_archive_multi.sau extracted_multi > /dev/null 2>&1

MATCH_COUNT=0
for i in 1 2 3 4; do
    if cmp -s test_file${i}.txt extracted_multi/test_file${i}.txt 2>/dev/null; then
        MATCH_COUNT=$((MATCH_COUNT+1))
    fi
done

if [ $MATCH_COUNT -eq 4 ]; then
    echo -e "${GREEN}✓${NC} Tüm 4 dosya doğrulandı"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}✗${NC} ${MATCH_COUNT}/4 dosya doğrulandı"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 9: Özel Çıkış Dizini
# ===============================================
echo -e "${YELLOW}[TEST 9]${NC} Özel Çıkış Dizini..."
mkdir -p my_extracts
if ./bin/tarsau -a test_archive_simple.sau my_extracts > /dev/null 2>&1; then
    if [ -f my_extracts/test_file1.txt ]; then
        echo -e "${GREEN}✓${NC} Dosyalar belirtilen dizine çıkarıldı"
        PASSED=$((PASSED+1))
    else
        echo -e "${RED}✗${NC} Dosyalar dizine çıkarılamadı"
        FAILED=$((FAILED+1))
    fi
else
    echo -e "${RED}✗${NC} Özel dizin çıkarma başarısız"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# TEST 10: Arşiv Boyutu Kontrolü
# ===============================================
echo -e "${YELLOW}[TEST 10]${NC} Arşiv Boyutu Hesaplaması..."
ORIGINAL_SIZE=$(($(wc -c < test_file1.txt) + $(wc -c < test_file2.txt)))
ARCHIVE_SIZE=$(wc -c < test_archive_simple.sau)

if [ $ARCHIVE_SIZE -gt $ORIGINAL_SIZE ]; then
    echo -e "${GREEN}✓${NC} Arşiv boyutu mantıklı (Header + Verileri)"
    echo "   Orijinal: $ORIGINAL_SIZE byte"
    echo "   Arşiv: $ARCHIVE_SIZE byte"
    PASSED=$((PASSED+1))
else
    echo -e "${RED}✗${NC} Arşiv boyutu anormal"
    FAILED=$((FAILED+1))
fi
echo ""

# ===============================================
# SONUÇ
# ===============================================
echo "================================================"
echo "TEST SONUÇLARI"
echo "================================================"
echo -e "${GREEN}✓ Başarılı: $PASSED${NC}"
echo -e "${RED}✗ Başarısız: $FAILED${NC}"
TOTAL=$((PASSED+FAILED))
echo "Toplam: $TOTAL"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}TÜM TESTLER BAŞARILI! 🎉${NC}"
else
    echo -e "${RED}BAZΙ TESTLER BAŞARISIZ${NC}"
fi

# Temizle
cleanup

exit $FAILED

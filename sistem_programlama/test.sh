#!/bin/bash

# Derleme talimatları

echo "=== tarsau Arşivleme Programı Derleme Rehberi ==="
echo ""

# 1. Temizle
echo "1. Önceki derleme dosyaları temizleniyor..."
make clean

# 2. Derle
echo ""
echo "2. Program derleniyoring..."
make

# 3. Test et
echo ""
echo "3. Temel test yapılıyor..."

# Dosyaları arşivle
echo "   - Dosyalar arşivleniyor..."
./bin/tarsau -b test1.txt test2.txt -o test_archive.sau

# Arşivi kontrol et
if [ -f test_archive.sau ]; then
    echo "   ✓ Arşiv dosyası oluşturuldu"
    ls -lh test_archive.sau
else
    echo "   ✗ Arşiv oluşturulamadı"
fi

# Arşivden çıkar
echo ""
echo "   - Arşivden çıkarılıyor..."
mkdir -p extracted
./bin/tarsau -a test_archive.sau extracted

# Çıkarılan dosyaları kontrol et
if [ -f extracted/test1.txt ]; then
    echo "   ✓ Dosyalar başarıyla çıkarıldı"
    echo "   Dosya içeriği:"
    head -1 extracted/test1.txt
else
    echo "   ✗ Dosyalar çıkarılamadı"
fi

echo ""
echo "=== Derleme Test Tamamlandı ==="

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = 

TARGET = tarsau
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Varsayılan hedef
all: $(BINDIR)/$(TARGET)

# Dizinleri oluştur
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

# Ana hedefi oluştur
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✓ Derleme tamamlandı: $(BINDIR)/$(TARGET)"

# Nesne dosyalarını oluştur
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "✓ Derlendi: $<"

# Temizleme
clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "✓ Temizlik tamamlandı"

# Install (isteğe bağlı)
install: $(BINDIR)/$(TARGET)
	@mkdir -p /usr/local/bin
	@cp $(BINDIR)/$(TARGET) /usr/local/bin/
	@chmod +x /usr/local/bin/$(TARGET)
	@echo "✓ $(TARGET) yüklendi: /usr/local/bin/$(TARGET)"

# Uninstall
uninstall:
	@rm -f /usr/local/bin/$(TARGET)
	@echo "✓ $(TARGET) kaldırıldı"

.PHONY: all clean install uninstall

# Autor: Petr Kaška (xkaska01)
# Název projektu: FLP - Rozhodovací stromy 🌲
# Datum: 26.2.2024

TARGET = flp-fun
ZIP_FILE = xkaska01.zip

SRCDIR = .

CC = ghc
CFLAGS = -Wall

SRCS = $(wildcard $(SRCDIR)/*.hs)

all: $(TARGET)
	make clear

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET)

clear:
	rm -f $(SRCDIR)/*.hi
	rm -f $(SRCDIR)/*.o

run1:
	./flp-fun -1 input/tree_file input/data_file	

run2:
	./flp-fun -2 input/training_file

zip:
	zip -r $(ZIP_FILE) $(SRCDIR)/*.hs Makefile

prof:
	ghc -prof -fprof-auto -rtsopts $(SRCS)
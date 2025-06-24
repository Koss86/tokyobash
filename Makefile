.SILENT:
CFLAGS = -Wall

install: tokyobash config
	mkdir -p ~/.config/tokyobash/
	mv tokyobash ~/.config/tokyobash/tokyobash  
	cp -n config ~/.config/tokyobash/config

tokyobash: tokyobash.o tokyobashlib.o
	gcc $(CFLAGS) -O3 tokyobash.o tokyobashlib.o -o tokyobash
	rm tokyobash.o tokyobashlib.o

tokyobash.o: src/tokyobash.c
	gcc $(CFLAGS) -c src/tokyobash.c

tokyobashlib.o: src/lib/tokyobashlib.c
	gcc $(CFLAGS) -c src/lib/tokyobashlib.c

.SILENT: gcc clang install

gcc: src/tokyobash.c src/lib/tokyobashlib.c

	gcc -O3 -o tokyobash src/tokyobash.c src/lib/tokyobashlib.c -Isrc/include

clang: src/tokyobash.c src/lib/tokyobashlib.c

	clang -O3 -o tokyobash src/tokyobash.c src/lib/tokyobashlib.c -Isrc/include

install: tokyobash config

	mkdir -p ~/.config/tokyobash/
	mv tokyobash ~/.config/tokyobash/tokyobash
	cp -n config ~/.config/tokyobash/config
	

.SILENT: make

make: src/tokyobash.c src/lib/libtokyobash.c
	gcc -O3 -o tokyoBash src/tokyobash.c src/lib/libtokyobash.c

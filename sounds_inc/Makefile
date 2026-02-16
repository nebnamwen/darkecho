all: *.c

defines.c: manifest.txt defines.pl
	perl -n defines.pl manifest.txt > defines.c

load.c: manifest.txt load.pl
	perl -n load.pl manifest.txt > load.c

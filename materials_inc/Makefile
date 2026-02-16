all: *.c

defines.c: manifest.txt defines.pl
	perl -n defines.pl manifest.txt > defines.c

assign.c: manifest.txt assign.pl
	perl -n assign.pl manifest.txt > assign.c

colors.c: manifest.txt colors.pl
	perl -n colors.pl manifest.txt > colors.c

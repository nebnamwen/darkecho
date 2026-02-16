CFLAGS = $(shell sdl2-config --cflags)
LDLIBS = $(shell sdl2-config --libs)

all: audio_test map_test

audio_test: audio_test.c sounds_*.c
	gcc $(CFLAGS) -o audio_test audio_test.c $(LDLIBS)

sounds_defines.c: sounds_manifest.txt sounds_defines.pl
	perl -n sounds_defines.pl sounds_manifest.txt > sounds_defines.c

sounds_load.c: sounds_manifest.txt sounds_load.pl
	perl -n sounds_load.pl sounds_manifest.txt > sounds_load.c

map_test: map_test.c map.c materials_*.c
	gcc $(CFLAGS) -o map_test map_test.c $(LDLIBS)

materials_defines.c: materials_manifest.txt materials_defines.pl
	perl -n materials_defines.pl materials_manifest.txt > materials_defines.c

materials_assign.c: materials_manifest.txt materials_assign.pl
	perl -n materials_assign.pl materials_manifest.txt > materials_assign.c

materials_colors.c: materials_manifest.txt materials_colors.pl
	perl -n materials_colors.pl materials_manifest.txt > materials_colors.c

CFLAGS = $(shell sdl2-config --cflags)
LDLIBS = $(shell sdl2-config --libs)

all: audio_test map_test

audio_test: audio_test.c sounds_defines.c sounds_load.c
	gcc $(CFLAGS) -o audio_test audio_test.c $(LDLIBS)

sounds_defines.c: sounds_manifest.txt sounds_defines.pl
	perl -n sounds_defines.pl sounds_manifest.txt > sounds_defines.c

sounds_load.c: sounds_manifest.txt sounds_load.pl
	perl -n sounds_load.pl sounds_manifest.txt > sounds_load.c

map_test: map_test.c map.c
	gcc $(CFLAGS) -o map_test map_test.c $(LDLIBS)

CFLAGS = $(shell sdl2-config --cflags)
LDLIBS = $(shell sdl2-config --libs)

.PHONY: all sounds_inc materials_inc

all: sounds_inc materials_inc audio_test map_test

sounds_inc:
	$(MAKE) -C sounds_inc

materials_inc:
	$(MAKE) -C materials_inc

audio_test: audio_test.c sounds_inc/*.c
	gcc $(CFLAGS) -o audio_test audio_test.c $(LDLIBS)

map_test: map_test.c map.c materials_inc/*.c
	gcc $(CFLAGS) -o map_test map_test.c $(LDLIBS)

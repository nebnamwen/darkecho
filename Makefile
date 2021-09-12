audio_test: audio_test.c sounds_defines.c sounds_load.c
	gcc `sdl2-config --cflags --libs` -o audio_test audio_test.c

sounds_defines.c: sounds_manifest.txt sounds_defines.pl
	perl -n sounds_defines.pl sounds_manifest.txt > sounds_defines.c

sounds_load.c: sounds_manifest.txt sounds_load.pl
	perl -n sounds_load.pl sounds_manifest.txt > sounds_load.c

#include "SDL.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "math.h"

typedef struct {
  uint32_t length;
  int16_t *samples;
} WAV_sound;

#include "sounds_defines.c"

WAV_sound WAV_sounds[SND_MAX];

void LoadSound(int index, const char* filename) {
  SDL_AudioSpec wav_spec;
  uint32_t wav_length;
  uint8_t *wav_buffer;

  SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length);
  // handle errors somehow

  int16_t *wav_samples = (int16_t *)wav_buffer;
  wav_length = sizeof(uint8_t) * wav_length / sizeof(int16_t);
  printf("wav_length: %i\n", wav_length);

  WAV_sounds[index].length = wav_length;
  WAV_sounds[index].samples = wav_samples;
}

void LoadAllSounds() {
#include "sounds_load.c"
}

int main(int argc, char *argv[]) {

  int SamplesPerSecond = 44100;
  uint32_t RunningSampleIndex = 0;
  int BytesPerSample = sizeof(int16_t) * 2;

  int BufferSamples = 512;

  int16_t SampleOut[4096];
  void *SoundBuffer = (void *)SampleOut;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_Window *sdl_win;
  sdl_win = SDL_CreateWindow("Beep", 0, 0, 100, 50, 0);

  SDL_AudioSpec AudioSettings = {0};

  AudioSettings.freq = SamplesPerSecond;
  AudioSettings.format = AUDIO_S16;
  AudioSettings.channels = 2;
  AudioSettings.samples = BufferSamples;

  SDL_OpenAudio(&AudioSettings, 0);

  printf("samples: %i\n", AudioSettings.samples);
  int TargetQueueBytes = AudioSettings.samples * BytesPerSample;

  LoadAllSounds();

  SDL_PauseAudio(0);

  int quit = 0;
  SDL_Event e;

  int which_sound = SND_CLICK;
  uint32_t click_time = 0;

  while (! quit) {
    while (SDL_PollEvent(&e)){
      switch (e.type) {
      case SDL_QUIT:
        quit = 1;
        break;

      case SDL_KEYDOWN:
	if (e.key.repeat == 0) {
	  switch (e.key.keysym.sym) {
	  case SDLK_q:
	    quit = 1;
	    break;

#define TRIGGER_SOUND(SND_INDEX) which_sound = SND_INDEX; \
	    click_time = RunningSampleIndex;

	  case SDLK_SPACE:
	    TRIGGER_SOUND(SND_CLICK);
	    break;

	  case SDLK_a:
	    TRIGGER_SOUND(SND_GLOCK_2A);
	    break;
	  }
	}
	break;
      }
    }

    int BytesToWrite = TargetQueueBytes - SDL_GetQueuedAudioSize(1);
    printf("%i\n", BytesToWrite);

    for(int SampleIndex = 0;
	SampleIndex < BytesToWrite / BytesPerSample;
	++SampleIndex)
      {
	int16_t SampleValue = 0;
	if (RunningSampleIndex >= click_time &&
	    RunningSampleIndex < click_time + WAV_sounds[which_sound].length) {
	  SampleValue = WAV_sounds[which_sound].samples[RunningSampleIndex - click_time];
	}
	SampleOut[SampleIndex*2] = SampleValue;
	SampleOut[SampleIndex*2 + 1] = SampleValue;

	RunningSampleIndex++;
      }
    SDL_QueueAudio(1, SoundBuffer, BytesToWrite);

    SDL_Delay(500 * TargetQueueBytes / (BytesPerSample * SamplesPerSecond));
  }

  SDL_CloseAudio();
  SDL_DestroyWindow(sdl_win);
  SDL_Quit();
}

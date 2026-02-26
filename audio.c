typedef struct {
  uint32_t length;
  int16_t *samples;
} WAV_sound;

#include "sounds_inc/defines.c"

WAV_sound WAV_sounds[SND_MAX];

void LoadSound(int index, const char* filename) {
  SDL_AudioSpec wav_spec;
  uint32_t wav_length;
  uint8_t *wav_buffer;

  SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length);
  // handle errors somehow

  int16_t *wav_samples = (int16_t *)wav_buffer;
  wav_length = sizeof(uint8_t) * wav_length / sizeof(int16_t);
  // fprintf(stderr, "wav_length: %i\n", wav_length);

  WAV_sounds[index].length = wav_length;
  WAV_sounds[index].samples = wav_samples;
}

void LoadAllSounds() {
#include "sounds_inc/load.c"
}

typedef struct {
  WAV_sound sound;
  uint32_t onset;
  float pan;
} sound_instance;

typedef struct snode {
  sound_instance contents;
  struct snode *next;
} list_of_sound_instance;

list_of_sound_instance *sound_instances = NULL;

void push_sound_instance(WAV_sound sound, uint32_t onset, float pan) {
  sound_instance new_instance;
  new_instance.sound = sound;
  new_instance.onset = onset;
  new_instance.pan = pan;

  list_of_sound_instance *new_node = (list_of_sound_instance *)malloc(sizeof(list_of_sound_instance));
  new_node->contents = new_instance;
  new_node->next = sound_instances;
  sound_instances = new_node;
}

void clear_sound_instances(uint32_t now) {
  list_of_sound_instance *current_pointer;
  list_of_sound_instance *prev_pointer;
  list_of_sound_instance current_node;
  sound_instance instance;

  current_pointer = sound_instances;
  prev_pointer = NULL;
  
  while (current_pointer != NULL) {
    current_node = *current_pointer;
    instance = current_node.contents;
    if (now > instance.onset + instance.sound.length) {
      if (prev_pointer == NULL) {
	sound_instances = current_node.next;
      }
      else {
	prev_pointer->next = current_node.next;
      }
      list_of_sound_instance *next_pointer = current_node.next;
      free(current_pointer);
      current_pointer = next_pointer;
    }
    else {
      prev_pointer = current_pointer;
      current_pointer = current_node.next;
    }
  }
}

int BytesPerSample = sizeof(int16_t) * 2;
int SamplesPerFrame;

int16_t SampleOut[4096];
void *SoundBuffer = (void *)SampleOut;

void init_audio() {
  SDL_AudioSpec AudioSettings = {0};

  AudioSettings.freq = SAMPLES_PER_SECOND;
  AudioSettings.format = AUDIO_S16;
  AudioSettings.channels = 2;
  AudioSettings.samples = 512;

  SDL_OpenAudio(&AudioSettings, 0);

  // fprintf(stderr, "samples: %i\n", AudioSettings.samples);
  SamplesPerFrame = AudioSettings.samples;

  LoadAllSounds();

  SDL_PauseAudio(0);
}

int queue_audio() {
  int BytesToWrite = SamplesPerFrame * BytesPerSample - SDL_GetQueuedAudioSize(1);

  int SampleIndex;
  for(SampleIndex = 0;
      SampleIndex < BytesToWrite / BytesPerSample;
      ++SampleIndex) {
    int16_t LeftSampleValue = 0;
    int16_t RightSampleValue = 0;
    list_of_sound_instance *current_node = sound_instances;
    while (current_node != NULL) {
      sound_instance current_instance = current_node->contents;
      if (RunningSampleIndex >= current_instance.onset &&
	  RunningSampleIndex < current_instance.onset + current_instance.sound.length) {
	LeftSampleValue += (-current_instance.pan + 1) * current_instance.sound.samples[RunningSampleIndex - current_instance.onset];
	RightSampleValue += (current_instance.pan + 1) * current_instance.sound.samples[RunningSampleIndex - current_instance.onset];
      }
      current_node = current_node->next;
    }

    SampleOut[SampleIndex*2] = LeftSampleValue;
    SampleOut[SampleIndex*2 + 1] = RightSampleValue;

    RunningSampleIndex++;
  }
  
  SDL_QueueAudio(1, SoundBuffer, BytesToWrite);

  clear_sound_instances(RunningSampleIndex);

  return SampleIndex;
}

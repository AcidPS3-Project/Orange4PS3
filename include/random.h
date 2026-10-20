#ifndef RANDOM_H
#define RANDOM_H

#include <pngdec/pngdec.h>

extern int text_speech;
extern int text_type;
extern int dialogue;
extern int max_dial;

extern bool shuffling;

typedef struct
{
    short *pcm;
    int pcm_size;
    int freq;
    int stereo;
    int voice;
} SoundEffect;

SoundEffect sfx_cylinder;
SoundEffect sfx_grab;
SoundEffect sfx_gunshot;
SoundEffect sfx_gunshot2;
SoundEffect sfx_incinerate;
SoundEffect sfx_set;
SoundEffect sfx_shove;
SoundEffect sfx_trigger;
SoundEffect sfx_triggerback;

#define BGM_LIST 4
#define MUS_DIR "/dev_hdd0/game/ORANGEPS3/USRDIR/mus/"

#define TOTAL_SHEETS 40
pngData spritesheet[TOTAL_SHEETS];
u32 sprite_off[TOTAL_SHEETS];

const char *bgm[BGM_LIST];
int bgm_start[BGM_LIST];
int bgm_end[BGM_LIST];

int LoadSFX();
void init_sfx();
void init_bgm();

int RandRange(int min, int max);
int max(int min, int max);

void PlayBGM(int id, int inf);
void PlaySFX(SoundEffect *sfx);
void StopSFX(SoundEffect *sfx);

void Load_PNG();

void UpdateFPS();
extern int fps;
void Initialize();

float lerp(int x, int y, float amount);

#endif
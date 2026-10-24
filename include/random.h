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

#define TOTAL_SHEETS 100

#define ORANGE_PLAYER 0
#define ORANGE_USUAL 1
#define ORANGE_CRYING 2
#define ORANGE_COCKY 3
#define ORANGE_LOBOTOMIZED 4
#define ORANGE_CYCLOPE 5
#define ORANGE_WTF 6

#define COM_ACT_NONE 0
#define COM_ACT_SHOOT 1
#define COM_ACT_SHOOT_OTHER 2
#define COM_ACT_SHUFFLE 3

extern int com_action;

extern bool controlling;
extern bool can_shuffle;
extern bool can_shoot_other;
extern bool com_can_shuffle;

extern float fade_alpha;
extern float tfade_alpha;
extern bool tfade;

extern bool lost_game;
extern bool won_game;

extern int player_anim;
extern int opponent_anim;
extern int opponent;

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
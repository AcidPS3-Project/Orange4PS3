#ifndef DRAWING_H
#define DRAWING_H

#include "libraries.h"

extern u32 *texture_pointer;

#define SCR_INIT 0
#define SCR_LOGO 1
#define SCR_MENU 2
#define SCR_CUTS 3
#define SCR_GAME 4
#define SCR_ENDS 5

typedef struct
{
    const int *frames;
    int count;
    int position;
    int timer;
    int delay;
    int loop;
    int current_frame;
} FaceAnimation;

extern const int anim_lookdown[15];
extern const int anim_lookother[6];
extern const int anim_scared[6];
extern const int anim_click[7];
extern const int anim_shoot[5];
extern const int anim_dead[9];
extern const int anim_killed[9];

extern const int anim_idle2[1];
extern const int anim_scared2[4];
extern const int anim_dead2[7];
extern const int anim_lookdown2[3];
extern const int anim_click2[8];
extern const int anim_shoot2[2];

extern FaceAnimation playerFace;
extern FaceAnimation opponentFace;

void SetFaceAnimation(FaceAnimation *a, const int *frames, int count, int delay, int loop);
void UpdateFaceAnimation(FaceAnimation *a);

extern int GameSCR;

void DrawRectangle2D(float x, float y, float w, float h, u32 rgba, float l);
void DrawBackground2D(u32 rgba, float l);

void DrawSprite2D(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float layer, u32 color);
void DrawSpriteUI(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, u32 color);
void DrawFull(float x, float y, int wid, int hei, float layer, u32 color);
void DrawSpriteUIR(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, float angle, u32 color);
void DrawSprite2DR(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float angle, float layer,  u32 color);
void SetTexture(int id);
void drawScene();
void LoadTexture();

#endif
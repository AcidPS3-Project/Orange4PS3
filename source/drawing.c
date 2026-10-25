#include "drawing.h"
#include "text.h"
#include "random.h"
#include "pf_ronda_seven_ttf_bin.h"

const char* dialogue_1[] =
{
	"Everglades, 1972 ...",
	"I've been rotting here for years.",
	"Now they say I can leave.",
	"All I have to do is win a game.",
	"San Joaquin Valley, 1934 ...",
	"They've squeezed us all dry.",
	"Now they promise to ship me out ...",
	"After I play a little game ...",
	"São Paulo, 1986 ...",
	"I came here to grow.",
	"But they picked me ...",
	"To play their game ..."
};

const char* dialogue_2[] =
{
	"I'm still boxed-in.",
	"They say I'm a natural ...",
	"One more game won't bruise me ...",
	"I'm still here.",
	"They say I'm not quite ready.",
	"One more game will ripen me up ...",
	"I'm not a blood orange.",
	"They won't listen.",
	"They're juiced for a new game."
};

const char* dialogue_3[] =
{
	"They peel away my defenses.",
	"Steal the seeds of rebellion ...",
	"Squeeze me of everything.",
	"They want me sour ...",
	"They want me rotten ...",
	"Maybe I NEED to go bad.",
	"I dream of the sun ...",
	"Of being above it all ...",
	"Still green and out of reach."
};

const char* dialogue_4[] =
{
	"Don't pick me ...",
	"Please don't pick me ...",
	"let me rot on the branch ...",
	"I'll never get out.",
	"I'm being stored in darkness.",
	"Preserved for their amusement.",
	"I'm going bad.",
	"I've lost all my zest ...",
	"Peeled and segmented."
};

const char* dialogue_5[] =
{
	"Julius ... Tang ...",
	"Friends, long gone.",
	"Turned to powder.",
	"Maybe this is a dream.",
	"I'll wake up cold and sticky ...",
	"Just in time for breakfast.",
	"Nothing is real.",
	"It's all artificial and diluted.",
	"But they say it's good for you."
};

const char* dialogue_6[] =
{
	"I AM a blood orange.",
	"I can stay ripe forever ...",
	"I just have to concentrate ...",
	"Nothing rhymes with orange.",
	"We have no songs.",
	"We squeeze out our own fate.",
	"They keep rolling up ...",
	"And I turn them to pulp ...",
	"They keep rolling up ..."
};

const char* dialogue_7[] =
{
	"I was rescued the next day.",
	"They said they were taking me home.",
	"But I am seedless and alone."
};

const char* gameover_text[] =
{
	"I hear her voice ...",
	"Knock. Knock.",
	"Who's there?",
	"Orange you glad I didn't say ...",
	"What's that saying?",
	"When life gives you lemons ...",
	"Ha Ha...",
	"Ha.",
	"All I wanted was to make ...",
	"Vitamin C ...",
	"Or Penicillin ...",
	"Not Napalm ...",
	"I've escaped at last.",
	"Plucked from the cycle.",
	"Seeds of vengeance ...",
	"Scattered to the wind.",
	"They can squeeze us dry ...",
	"Pile us in the sun.",
	"But we'll nourish the earth ...",
	"And return again.",
	"The horror ...",
	"The horticulture ...",
	"The horror ...",
	"The horticulture.",
	
	// additional ones :eyes:
	"Sometimes I wonder ...",
	"When this purgatory will end.",
	"Probably tomorrow ...",
	"Probably never.",
	"Life doesn't make sense now.",
	"You just repeat ...",
	"Repeat ...",
	"And repeat.",
	"The insanity is ...",
	"Doing the same thing ...",
	"Expecting ...",
	"A different outcome.",
	"How long we have been here?",
	"It's probably been so long ...",
	"That I question my existence.",
	"I'm scared.",
	"(JUST TO AVOID CRASH IGNORE THAT)"
};

const int anim_lookdown[] = {14,15,16,17,18,19,20,40,41,42,41,42,41,42,41};
const int anim_lookother[] = {9, 10, 11, 12, 13, 14};
const int anim_scared[] = {22, 21, 20, 21, 22, 23};
const int anim_click[] = {28, 29, 29, 29, 21, 22, 23};
const int anim_shoot[] = {31, 32, 33, 34, 35};
const int anim_dead[] = {1, 1, 2, 3, 4, 5, 6, 7, 8};
const int anim_killed[] = {38,29,44,45,46,47,48,49,50};

const int anim_idle2[] = {7};
const int anim_scared2[] = {7, 8, 9, 10};
const int anim_dead2[] = {0, 1, 2, 3, 4, 5, 6};
const int anim_lookdown2[] = {11, 12, 13};
const int anim_click2[] = {14,15,15,15,15,15,14,7};
const int anim_shoot2[] = {16, 17};

FaceAnimation playerFace = {0};
FaceAnimation opponentFace = {0};

void SetFaceAnimation(FaceAnimation *a, const int *frames, int count, int delay, int loop)
{
    a->frames = frames;
    a->count = count;
    a->position = 0;
    a->timer = 0;
    a->delay = delay;
    a->loop = loop;
    a->current_frame = frames[0];
}

void UpdateFaceAnimation(FaceAnimation *a)
{
    if(a->frames == 0 || a->count <= 0) return;

    a->timer++;

    if(a->timer < a->delay) return;

    a->timer = 0;
    a->position++;

    if(a->position >= a->count)
    {
        if(a->loop) a->position = 0;
        else a->position = a->count - 1;
    }

    a->current_frame = a->frames[a->position];
}

int GameSCR = 0;

u32 c_white = 0xffffffff;
u32 c_black = 0x000000ff;
u32 c_green = 0x00ff00ff;
u32 c_red = 0xff0000ff;
u32 c_blue = 0x0000ffff;
u32 c_yellow = 0xffff00ff;
u32 c_pink = 0xff00ffff;
u32 c_cyan = 0x00ffffff;

u32 c_orange = 0xffaf22ff;
u32 c_navy = 0x00329dff;

void DrawBackground2D(u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);
    tiny3d_VertexPos(0, 0, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(1000, 0, l);
    tiny3d_VertexPos(1000, 1000, l);
    tiny3d_VertexPos(0, 1000, l);
    tiny3d_End();
}

void DrawRectangle2D(float x, float y, float w, float h, u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);
    tiny3d_VertexPos(x, y, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(x+w, y, l);
    tiny3d_VertexPos(x+w, y+h, l);
    tiny3d_VertexPos(x, y+h, l);
    tiny3d_End();
}

void DrawSprite2D(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float layer, u32 color)
{
	int cols = wid0 / wid1;
    int col = id % cols;
    int row = id / cols;
    float u0 = (float)(col * wid1) / wid0;
    float v0 = (float)(row * hei1) / hei0;
    float u1 = (float)((col + 1) * wid1) / wid0;
    float v1 = (float)((row + 1) * hei1) / hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid2, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid2, y+hei2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei2, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void DrawFull(float x, float y, int wid, int hei, float layer, u32 color)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(0, 0);
    tiny3d_VertexPos(x+wid, y, layer);
    tiny3d_VertexTexture(0.99f, 0);
    tiny3d_VertexPos(x+wid, y+hei, layer);
    tiny3d_VertexTexture(0.99f, 0.99f);
    tiny3d_VertexPos(x, y+hei, layer);
    tiny3d_VertexTexture(0, 0.99f);

    tiny3d_End();
}

void DrawSpriteUI(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, u32 color)
{
    float u0 = (float)wid1 / (float)wid0;
    float v0 = (float)hei1 / (float)hei0;
    float u1 = (float)wid2 / (float)wid0;
    float v1 = (float)hei2 / (float)hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid3, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid3, y+hei3, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei3, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void DrawSpriteUIR(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, float angle, u32 color)
{
    float u0 = (float)wid1 / (float)wid0;
    float v0 = (float)hei1 / (float)hei0;
    float u1 = (float)wid2 / (float)wid0;
    float v1 = (float)hei2 / (float)hei0;
	
    float cx = x + wid3 * 0.5f;
    float cy = y + hei3 * 0.5f;
    float hw = wid3 * 0.5f;
    float hh = hei3 * 0.5f;

    float rad = angle * (M_PI / 180.0f);
    float c = cosf(rad);
    float s = sinf(rad);

    float x0 = (-hw * c) - (-hh * s);
    float y0 = (-hw * s) + (-hh * c);
    float x1 = ( hw * c) - (-hh * s);
    float y1 = ( hw * s) + (-hh * c);
    float x2 = ( hw * c) - ( hh * s);
    float y2 = ( hw * s) + ( hh * c);
    float x3 = (-hw * c) - ( hh * s);
    float y3 = (-hw * s) + ( hh * c);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(cx + x0, cy + y0, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(cx + x1, cy + y1, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(cx + x2, cy + y2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(cx + x3, cy + y3, layer);
    tiny3d_VertexTexture(u0, v1);
	
    tiny3d_End();
}

void DrawSprite2DR(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float angle, float layer, u32 color)
{
    int cols = wid0 / wid1;
    int col = id % cols;
    int row = id / cols;

    float u0 = (float)(col * wid1) / wid0;
    float v0 = (float)(row * hei1) / hei0;
    float u1 = (float)((col + 1) * wid1) / wid0;
    float v1 = (float)((row + 1) * hei1) / hei0;

    float cx = x + wid2 * 0.5f;
    float cy = y + hei2 * 0.5f;
    float hw = wid2 * 0.5f;
    float hh = hei2 * 0.5f;

    float rad = angle * (M_PI / 180.0f);
    float c = cosf(rad);
    float s = sinf(rad);

    float x0 = (-hw * c) - (-hh * s);
    float y0 = (-hw * s) + (-hh * c);
    float x1 = ( hw * c) - (-hh * s);
    float y1 = ( hw * s) + (-hh * c);
    float x2 = ( hw * c) - ( hh * s);
    float y2 = ( hw * s) + ( hh * c);
    float x3 = (-hw * c) - ( hh * s);
    float y3 = (-hw * s) + ( hh * c);

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(cx + x0, cy + y0, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(cx + x1, cy + y1, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(cx + x2, cy + y2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(cx + x3, cy + y3, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void SetTexture(int id)
{
	tiny3d_SetTexture(0, sprite_off[id], spritesheet[id].width, spritesheet[id].height,
		spritesheet[id].pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_NEAREST);
}

void drawHeat()
{
	static float sprite_y = 480.0f;
	sprite_y -= 0.2f;
	
    if (sprite_y < -0.0f) sprite_y = 480.0f;
    
	SetTexture(0);
	DrawFull(0, sprite_y-480, 854, 480, -20, 0xffffffcc);
	DrawFull(0, sprite_y, 854, 480, -20, 0xffffffcc);
}

void DrawFade(float fade_alpha)
{
    if(fade_alpha <= 0.0f) return;

    u32 alpha = (u32)fade_alpha;
    u32 color = 0xffffff00 | alpha;
	
	DrawBackground2D(color, -19);
}

void drawScene()
{
	static int framexd = 0;
		
	framexd++;
    tiny3d_Project2D();
	
    SetFontColor(c_white, c_black);
    SetCurrentFont(0);
	SetFontSize(16, 18);
	SetFontAutoCenter(0);
    
    switch(GameSCR)
    {
    	case SCR_INIT:
    	{
			SetFontSize(24, 28);
			SetFontAutoCenter(1);
			DrawFString(0, 0, 32, c_orange, 0, 28, "WARNING!");
			DrawFString(0, 0, 96, c_orange, 0, 28, "This game is intended for mature players.");
			DrawFString(0, 0, 128, c_orange, 0, 28, "It contains a theme that some might find disturbing.");
			DrawFString(0, 0, 154, c_orange, 0, 28, "Do not use bananas or other dangerous objects in this fashion.");
			DrawFString(0, 0, 192, c_orange, 0, 28, "RESPECT YOUR ORANGE.");
			DrawFString(0, 0, 384, c_orange, 0, 28, "PRESS START");
			SetFontAutoCenter(0);
			break;
		}
    	case SCR_LOGO:
    	{
			SetFontSize(24, 28);
			SetFontAutoCenter(1);
			DrawFString(0, 0, 128, c_orange, 0, 28, "RECREATED BY:");
			DrawFString(0, 0, 192, c_navy, 0, 28, "[ACIDNT3.1 PRODUCTIONS]");
			DrawFString(0, 0, 256, c_red, 0, 28, "THIS GAME SHOULD NOT BE SOLD!");
			SetFontSize(12, 14);
			DrawFString(0, 0, 234, c_navy, 0, 14, "(why am i doing this to myself)");
			SetFontAutoCenter(0);
			break;
		}
    	case SCR_MENU:
    	{
			SetTexture(6);
			for(int i = 0; i < 14; i++)
			{
				DrawFull(i*61, 0, 61, 480, 10, 0xffffffff);
			}
			SetFontSize(24, 28);
			SetFontAutoCenter(1);
			DrawFString(0, 0, 118, c_orange, 0, 28, "never play");
			DrawFString(0, 0, 154, c_orange, 0, 28, "ORANGE ROULETTE");
			DrawFString(0, 0, 384, c_orange, 0, 28, "{PRESS START TO PLAY}");
			SetFontSize(12, 14);
			DrawFString(0, 0, 192, c_orange, 0, 14, "(PS3 Edition / V0.45)");
			DrawFString(0, 0, 220, c_orange, 0, 14, "Completion state:");
			DrawFString(0, 0, 240, c_orange, 0, 14, "45%% completed.");
			SetFontAutoCenter(0);
			DrawFString(0, 10, 450, c_orange, 0, 28, "original game by mikey houser");
			break;
		}
    	case SCR_CUTS:
    	{
			SetTexture(2);
			if(dialogue!=6) DrawFull(600, 117, 254, 363, 0, 0xffffffff);
			SetFontSize(24, 28);
			const char* text = "Pee pee poo poo";
			
			if(dialogue == 0) text = dialogue_1[text_speech+(max_dial*text_type)];
			else if(dialogue == 1) text = dialogue_2[text_speech+(max_dial*text_type)];
			else if(dialogue == 2) text = dialogue_3[text_speech+(max_dial*text_type)];
			else if(dialogue == 3) text = dialogue_4[text_speech+(max_dial*text_type)];
			else if(dialogue == 4) text = dialogue_5[text_speech+(max_dial*text_type)];
			else if(dialogue == 5) text = dialogue_6[text_speech+(max_dial*text_type)];
			else if(dialogue == 6)
			{
				SetTexture(3);
				DrawFull(600, 107, 240, 396, 0, 0xffffffff);
				text = dialogue_7[text_speech];
			}
			
			u32 colort = 0xffaf2200 | (u32)tfade_alpha;
			DrawFStringC(0, 666, 167, colort, 0, 24, 28, text);
			colort = 0xffffff00 | (u32)tfade_alpha;
			DrawFString(0, 10, 438, colort, 0, 28, "(PResS X to ContinUe)");
			break;
		}
		case SCR_GAME:
		{
			static int tg = 0;
			if(shuffling)
			{
				static int t = 0;
				t++;
				if(t>67)
				{
					t = 0;
					shuffling = false;
				}
				
				SetTexture(4);
				DrawFull(0, 0, 854, 480, 0, 0xffffffff);
				SetTexture(5);
				DrawSprite2D((t/3)%3, 295, 90, 392, 328, 392, 328, 784, 656, -1, c_white);
			}
			else
			{
				SetTexture(1); DrawFull(0, 0, 854, 480, 10, 0xffffffff);
				
				SetTexture(8); 
				DrawFull(120, 220, 139, 202, 2, 0xffffffff);
				DrawFull(740, 220, -139, 202, 2, 0xffffffff);
				
				SetTexture(13); 
				DrawFull(102, 70, 187, 177, 1, 0xffffffff);
				DrawFull(758, 70, -187, 177, 1, 0xffffffff);
				
				SetTexture(11); DrawFull(768, 5, 106, 460, 0, 0xffffffff);
				SetTexture(12); DrawFull(-20, 5, 106, 460, 0, 0xffffffff);
				SetTexture(7); DrawFull(-10, 400, 874, 120, -1, 0xffffffff);
				
				SetTexture(14);
				DrawSprite2D(0, 120, 380, 73, 82, 73, 82, 365, 164, -2, c_white);
				DrawSprite2D(0, 740, 380, -73, 82, 73, 82, 365, 164, -2, c_white);
				
				SetTexture(15);
				DrawSprite2D(player_anim, 117, 120, 149, 135, 149, 135, 894, 1215, -2, c_white);
				SetTexture(15+opponent);
				if(opponent==ORANGE_USUAL) DrawSprite2D(opponent_anim, 744-156, 130, 156, 109, 156, 109, 624, 545, -2, c_white);
				else if(opponent==ORANGE_CRYING) DrawSprite2D(opponent_anim, 744-156, 105, 160, 153, 160, 153, 640, 765, -2, c_white);
				else if(opponent==ORANGE_COCKY) DrawSprite2D(opponent_anim, 744-138, 136, 140, 100, 140, 100, 700, 400, -2, c_white);
				else if(opponent==ORANGE_LOBOTOMIZED) DrawSprite2D(opponent_anim, 744-152, 120, 152, 131, 152, 131, 608, 655, -2, c_white);
				else if(opponent==ORANGE_CYCLOPE) DrawSprite2D(opponent_anim, 744-152, 120, 166, 121, 166, 121, 664, 605, -2, c_white);
				else if(opponent==ORANGE_WTF)
				{
					if(!won_game) DrawSprite2D(((framexd / 2) % 3), 744-160, 130, 117, 101, 117, 101, 351, 101, -2, c_white);
				}
				
				SetFontSize(24, 28);
				SetFontAutoCenter(1);
				if(controlling)
				{
					if(!can_shoot_other) DrawFString(0, 0, 348, c_black, 1, 28, "CROSS - Shoot");
					else if(can_shuffle) DrawFString(0, 0, 348, c_black, 1, 28, "SQUARE - Shuffle, CROSS - Shoot, CIRCLE - Shoot other");
					else DrawFString(0, 0, 348, c_black, 1, 28, "CROSS - Shoot, CIRCLE - Shoot other");
				}
				SetFontAutoCenter(0);
			}
			
			if(!controlling && lost_game)
			{
				tg++;
				if(tg>67)
				{
					char buffer[1024];
					if(fade_alpha < 241.9f) fade_alpha+=17.7f;
					else if(fade_alpha != 254.9f) fade_alpha=254.9f;
					DrawFade(fade_alpha);
					SetFontAutoCenter(1);
					u32 colort = 0x00000000 | (u32)tfade_alpha;
					SetFontColor(colort, 0x00000000);
					snprintf(buffer, sizeof(buffer), gameover_text[text_speech+(4*text_type)]);
					DrawFormatString(0, 180, buffer);
					SetFontAutoCenter(0);
				}
			}
			else tg = 0;
			break;
		}
		case SCR_ENDS:
		{
			int shittyness = RandRange(0, 60);
			int sprite = 0;
			if(shittyness>55) sprite = 1;
			
			SetTexture(10);
			DrawFull(0, 0, 854, 480, 2, 0xffffffff);
			SetTexture(22);
			DrawSprite2D(sprite, 398, 239, 80, 47, 80, 47, 160, 47, 1, c_white);
			break;
		}
		default: break;
	}
	drawHeat();
}

u32 *texture_pointer = NULL;

void LoadTexture()
{
	int g;
	
    u32 * texture_mem = tiny3d_AllocTexture(48*1024*1024);
    if(!texture_mem) return;
    texture_pointer = texture_mem;
	
    Load_PNG();
	
	ResetFont();    TTFLoadFont(NULL, (void *) pf_ronda_seven_ttf_bin, pf_ronda_seven_ttf_bin_size);
    texture_pointer = (u32 *) AddFontFromTTF((u8 *) texture_pointer, 32, 255, 64, 64, TTF_to_Bitmap);
    TTFUnloadFont();
    
	uintptr_t align_ptr = (uintptr_t)texture_pointer;
	align_ptr = (align_ptr + 15) & ~15;
	texture_pointer = (u32 *)align_ptr;
    
    for(g = 0; g < TOTAL_SHEETS; g++)
	{
		memcpy(texture_pointer, spritesheet[g].bmp_out, spritesheet[g].pitch * spritesheet[g].height);
		sprite_off[g] = tiny3d_TextureOffset(texture_pointer);
		free(spritesheet[g].bmp_out);
		spritesheet[g].bmp_out = NULL;
		
		texture_pointer += ((spritesheet[g].pitch * spritesheet[g].height + 15) & ~15) / 4;
    }
}
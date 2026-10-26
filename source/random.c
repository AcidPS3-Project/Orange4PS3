#include "random.h"
#include "assets.h"
#include "libraries.h"
#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"
#include "spu_soundmodule.bin.h"

float lerp(int x, int y, float amount)
{
	// gamemaker 1.4 function, hehe
	return x + amount * (y - x);
}

int fps = 0;
static int frames = 0;
static u64 last_time = 0;

int com_action = COM_ACT_NONE;

int face_x1 = 0;
int face_x2 = -15;

int text_speech = 0;
int text_type = 0;
int dialogue = 0;
int max_dial = 3;
int opponent = ORANGE_PLAYER;

bool controlling = true;
bool can_shuffle = true;
bool can_shoot_other = true;
bool com_can_shuffle = true;

int player_anim = 22;
int opponent_anim = 7;

bool shuffling = false;

float fade_alpha = 0.0f;
float tfade_alpha = 0.0f;
bool tfade = false;

bool lost_game = false;
bool won_game = false;

void UpdateFPS()
{
    u64 now = sysGetSystemTime();
    frames++;

    if(now - last_time >= 1000000ULL)
    {
        fps = frames;
        frames = 0;
        last_time = now;
    }
}

int sex = 2147483647;

void init_bgm()
{
	for(int z = 0; z < BGM_LIST; z++)
	{
		bgm[z] = "";
		bgm_start[z] = 0;
		bgm_end[z] = sex;
	}
	
	bgm[0] = MUS_DIR "jerad.mp3";
	bgm[1] = MUS_DIR "jerad2.mp3";
	bgm[2] = MUS_DIR "tony.mp3";
	bgm[3] = MUS_DIR "tony2.mp3";
}

int LoadSFX(SoundEffect *sfx, const void *data, int size)
{
    sfx->pcm_size = size * 20;
	sfx->pcm = memalign(32, sfx->pcm_size);

    if(!sfx->pcm) return 0;
    if(DecodeAudio((void *)data,size,sfx->pcm,&sfx->pcm_size,&sfx->freq,&sfx->stereo) != 0)
    {
        free(sfx->pcm);
        sfx->pcm = NULL;
        return 0;
    }

    return 1;
}

void init_sfx()
{
	LoadSFX(&sfx_cylinder, cylinder_bin, cylinder_bin_size);
	LoadSFX(&sfx_grab, grab_bin, grab_bin_size);
	LoadSFX(&sfx_gunshot, gunshot_bin, gunshot_bin_size);
	LoadSFX(&sfx_gunshot2, gunshot2_bin, gunshot2_bin_size);
	LoadSFX(&sfx_incinerate, incinerate_bin, incinerate_bin_size);
	LoadSFX(&sfx_set, set_bin, set_bin_size);
	LoadSFX(&sfx_shove, shove_bin, shove_bin_size);
	LoadSFX(&sfx_trigger, trigger_bin, trigger_bin_size);
	LoadSFX(&sfx_triggerback, triggerback_bin, triggerback_bin_size);
}

int RandRange(int min, int max)
{
	int rndom;
	rndom = rand() % (max-min+1) + min;
	return rndom;
}

int max(int min, int max)
{
	int biggest;
	biggest = (min > max) ? min : max;
	
	return biggest;
}

void PlayBGM(int id, int inf)
{
	StopAudio();
	FILE *fp = fopen(bgm[id], "rb");
	
	if(!fp)
    {
        printf("Error! No music found.\n");
        return;
    }
	
    PlayAudiofd(fp, 0, inf ? AUDIO_INFINITE_TIME : AUDIO_ONE_TIME);
}

void PlaySFX(SoundEffect *sfx)
{
	int voice = SND_GetFirstUnusedVoice();
    if (voice < 1) return;
	
    SND_SetVoice(voice, sfx->stereo ? VOICE_STEREO_16BIT : VOICE_MONO_16BIT,
        sfx->freq, 0, sfx->pcm, sfx->pcm_size, 255, 255, NULL);
	
	sfx->voice = voice;
}

void StopSFX(SoundEffect *sfx)
{
    if(sfx->voice >= 1)
    {
        SND_StopVoice(sfx->voice);
        sfx->voice = -1;
    }
}

void Load_PNG()
{
	pngLoadFromBuffer(heat_bin, heat_bin_size, &spritesheet[0]);
	pngLoadFromBuffer(bg_bin, bg_bin_size, &spritesheet[1]);
	pngLoadFromBuffer(prisonorange_bin, prisonorange_bin_size, &spritesheet[2]);
	pngLoadFromBuffer(prisonorange2_bin, prisonorange2_bin_size, &spritesheet[3]);
	pngLoadFromBuffer(bananagunbg_bin, bananagunbg_bin_size, &spritesheet[4]);
	pngLoadFromBuffer(bananacylinder_bin, bananacylinder_bin_size, &spritesheet[5]);
	pngLoadFromBuffer(titlebg_bin, titlebg_bin_size, &spritesheet[6]);
	pngLoadFromBuffer(table_bin, table_bin_size, &spritesheet[7]);
	pngLoadFromBuffer(orangebody_bin, orangebody_bin_size, &spritesheet[8]);
	pngLoadFromBuffer(bananagun_bin, bananagun_bin_size, &spritesheet[9]);
	pngLoadFromBuffer(endbg_bin, endbg_bin_size, &spritesheet[10]);
	pngLoadFromBuffer(sidewall_bin, sidewall_bin_size, &spritesheet[11]);
	pngLoadFromBuffer(sidewall2_bin, sidewall2_bin_size, &spritesheet[12]);
	pngLoadFromBuffer(orange_bin, orange_bin_size, &spritesheet[13]);
	pngLoadFromBuffer(orangelhand_bin, orangelhand_bin_size, &spritesheet[14]);
	pngLoadFromBuffer(orange_f1_bin, orange_f1_bin_size, &spritesheet[15]);
	pngLoadFromBuffer(orange_f2_bin, orange_f2_bin_size, &spritesheet[16]);
	pngLoadFromBuffer(orange_f3_bin, orange_f3_bin_size, &spritesheet[17]);
	pngLoadFromBuffer(orange_f4_bin, orange_f4_bin_size, &spritesheet[18]);
	pngLoadFromBuffer(orange_f5_bin, orange_f5_bin_size, &spritesheet[19]);
	pngLoadFromBuffer(orange_f6_bin, orange_f6_bin_size, &spritesheet[20]);
	pngLoadFromBuffer(orange_f7_bin, orange_f7_bin_size, &spritesheet[21]);
	pngLoadFromBuffer(orange_fend_bin, orange_fend_bin_size, &spritesheet[22]);
	pngLoadFromBuffer(end_bin, end_bin_size, &spritesheet[23]);
	//pngLoadFromBuffer(_bin, _bin_size, &spritesheet[]);
}

void Initialize()
{
	printf("Initialization...\n");
	u32 spu;
	sysSpuImage spu_image;
	sysSpuInitialize(1, 5);
	sysSpuRawCreate(&spu, NULL);
	sysSpuImageImport(&spu_image, spu_soundmodule_bin, 0);
	sysSpuRawImageLoad(spu, &spu_image);
	SND_Init(spu);
}
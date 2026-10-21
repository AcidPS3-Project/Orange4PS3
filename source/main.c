#include <tiny3d.h>

#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"

#include "drawing.h"
#include "random.h"
#include "core.h"

bool bullets[] = {true, false, false, false, false, false};
size_t bulletn = sizeof(bullets) / sizeof(bullets[0]);

bool old_control_left = false;
bool old_control_right = false;
bool old_control_up = false;
bool old_control_down = false;

int main(int argc, const char* argv[])
{
	padInfo padinfo;
	int ip;
	static padData oldpad;
	padData paddata;
	
	srand(time(NULL));
	tiny3d_Init(6*1024*1024);
	tiny3d_UserViewport(1, 0, 0, (float)(Video_Resolution.width / 854.0f), (float)(Video_Resolution.height / 480.0f),
		(float)(Video_Resolution.width / 1920.0f), (float)(Video_Resolution.height / 1080.0f));
	
	sysModuleLoad(SYSMODULE_PNGDEC);
	
	Initialize();
	
	init_bgm();
	init_sfx();
	
	ioPadInit(7);
    LoadTexture();
    
    while(1)
    {
		sysUtilCheckCallback();
		UpdateFPS();
		
        tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);
		tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);
		tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
            TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
            TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
		
		ioPadGetInfo(&padinfo);
		for(ip = 0; ip < MAX_PADS; ip++)
		{
			if(padinfo.status[ip])
			{
				ioPadGetData(ip, &paddata);
				
				bool control_left = paddata.BTN_LEFT || paddata.ANA_L_H < 80;
				bool control_right = paddata.BTN_RIGHT || paddata.ANA_L_H > 176;

				bool control_up = paddata.BTN_UP || paddata.ANA_L_V < 80;
				bool control_down = paddata.BTN_DOWN || paddata.ANA_L_V > 176;
				
				switch(GameSCR)
				{
					case SCR_INIT:
					{
						if(paddata.BTN_START && !oldpad.BTN_START) GameSCR = SCR_LOGO;
						break;
					}
					case SCR_LOGO:
					{
						static int t = 0;
						t++;
						if(t>120)
						{
							GameSCR = SCR_MENU;
							PlayBGM(3, 1);
						}
						break;
					}
					case SCR_MENU:
					{
						if(paddata.BTN_START && !oldpad.BTN_START)
						{
							text_speech = 0;
							text_type = RandRange(0, 2);
							max_dial = 4;
							tfade = true;
							GameSCR = SCR_CUTS;
							PlayBGM(2, 1);
						}
						break;
					}
					case SCR_CUTS:
					{
						if(tfade)
						{
							if(tfade_alpha < 241.9f) tfade_alpha+=10.7f;
							else if(tfade_alpha != 254.9f) tfade_alpha=254.9f;
						}
						else
						{
							if(tfade_alpha > 9.0f) tfade_alpha-=10.7f;
							else if(tfade_alpha != 0.0f)
							{
								tfade_alpha=0.0f;
								tfade = true;
								if(text_speech>=max_dial-1)
								{
									GameSCR = SCR_GAME;
									text_speech = 0;
									shuffle(bullets, bulletn);
									controlling = true;
									can_shuffle = true;
									PlayBGM(0, 1);
								}
								else text_speech++;
							}
						}
						
						if(paddata.BTN_CROSS && !oldpad.BTN_CROSS && tfade) tfade = false;
						break;
					}
					case SCR_GAME:
					{
						if(!shuffling && controlling)
						{
							if(paddata.BTN_CROSS && !oldpad.BTN_CROSS)
							{
								text_type = RandRange(0, 5);
								if(bullets[0])
								{
									PlaySFX(&sfx_gunshot);
									controlling = false;
								}
								else
								{
									PlaySFX(&sfx_trigger);
									shoot_gun(bullets, bulletn);
								}
							}
							if(paddata.BTN_SQUARE && !oldpad.BTN_SQUARE && can_shuffle)
							{
								can_shuffle = false;
								shuffle(bullets, bulletn);
							}
						}
						else if(!controlling)
						{
							if(fade_alpha >= 250.0f)
							{
								if(tfade)
								{
									if(tfade_alpha < 241.9f) tfade_alpha+=10.7f;
									else if(tfade_alpha != 254.9f) tfade_alpha=254.9f;
								}
								else
								{
									if(tfade_alpha > 9.0f) tfade_alpha-=10.7f;
									else if(tfade_alpha != 0.0f)
									{
										tfade_alpha=0.0f;
										tfade = true;
										if(text_speech>=3)
										{
											text_speech = 0;
											fade_alpha = 0.0f;
											shuffle(bullets, bulletn);
											controlling = true;
											can_shuffle = true;
										}
										else text_speech++;
									}
								}
								
								if(paddata.BTN_CROSS && !oldpad.BTN_CROSS && tfade) tfade = false;
							}
							else if(paddata.BTN_SELECT && !oldpad.BTN_SELECT) text_type = RandRange(6, 9);
						}
					}
					default: break;
				}
				
				old_control_left = control_left;
				old_control_right = control_right;
				old_control_up = control_up;
				old_control_down = control_down;
				
				oldpad = paddata;
			}
		}
		
		drawScene();
		
		tiny3d_Flip();
    }

    return 0;
}
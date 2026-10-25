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

int bullets_shooted = 0;

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
									if(dialogue==6) GameSCR = SCR_ENDS;
									else
									{
										opponent = dialogue + 1;
										
										GameSCR = SCR_GAME;
										text_speech = 0;
										bullets_shooted = 0;
										shuffle(bullets, bulletn);
										controlling = true;
										can_shuffle = true;
										com_can_shuffle = true;
										won_game = false;
										can_shoot_other = true;
										lost_game = false;
										PlayBGM(0, 1);
										
										SetFaceAnimation(&playerFace, anim_scared, sizeof(anim_scared) / sizeof(anim_scared[0]), 5, 1);
										SetFaceAnimation(&opponentFace, anim_idle2, sizeof(anim_idle2) / sizeof(anim_idle2[0]), 5, 1);
									}
								}
								else text_speech++;
							}
						}
						
						if(paddata.BTN_CROSS && !oldpad.BTN_CROSS && tfade) tfade = false;
						break;
					}
					case SCR_GAME:
					{
						if(!shuffling)
						{
							UpdateFaceAnimation(&playerFace);
							UpdateFaceAnimation(&opponentFace);
							player_anim = playerFace.current_frame;
							opponent_anim = opponentFace.current_frame;
						}
						
						if(!shuffling && controlling)
						{
							if(paddata.BTN_CROSS && !oldpad.BTN_CROSS)
							{
								can_shoot_other = true;
								text_type = RandRange(0, 5);
								controlling = false;
								bullets_shooted++;
								if(bullets[0])
								{
									SetFaceAnimation(&playerFace, anim_dead, sizeof(anim_dead) / sizeof(anim_dead[0]), 1, 0);
									SetFaceAnimation(&opponentFace, anim_shoot2, sizeof(anim_shoot2) / sizeof(anim_shoot2[0]), 5, 0);
									PlaySFX(&sfx_gunshot);
									lost_game = true;
								}
								else
								{
									SetFaceAnimation(&playerFace, anim_click, sizeof(anim_click) / sizeof(anim_click[0]), 5, 0);
									PlaySFX(&sfx_trigger);
									shoot_gun(bullets, bulletn);
								}
							}
							if(paddata.BTN_SQUARE && !oldpad.BTN_SQUARE && can_shuffle && can_shoot_other)
							{
								can_shuffle = false;
								shuffle(bullets, bulletn);
							}
							if(paddata.BTN_CIRCLE && !oldpad.BTN_CIRCLE && can_shoot_other)
							{
								can_shoot_other = false;
								if(bullets[0])
								{
									SetFaceAnimation(&playerFace, anim_killed, sizeof(anim_killed) / sizeof(anim_killed[0]), 6, 0);
									SetFaceAnimation(&opponentFace, anim_dead2, sizeof(anim_dead2) / sizeof(anim_dead2[0]), 1, 0);
									controlling = false;
									printf("I lost. Ouchie\n");
									text_type = RandRange(0, 2);
									dialogue++;
									max_dial = 3;
									PlayBGM(2, 1);
									PlaySFX(&sfx_gunshot);
									won_game = true;
								}
								else
								{
									SetFaceAnimation(&playerFace, anim_click, sizeof(anim_click) / sizeof(anim_click[0]), 5, 0);
									SetFaceAnimation(&opponentFace, anim_scared2, sizeof(anim_scared2) / sizeof(anim_scared2[0]), 5, 0);
									PlaySFX(&sfx_trigger);
									shoot_gun(bullets, bulletn);
								}
							}
						}
						else if(!controlling && lost_game)
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
											lost_game = false;
											text_speech = 0;
											fade_alpha = 0.0f;
											bullets_shooted = 0;
											shuffle(bullets, bulletn);
											controlling = true;
											can_shuffle = true;
											can_shoot_other = true;
											com_can_shuffle = true;
											
											SetFaceAnimation(&playerFace, anim_scared, sizeof(anim_scared) / sizeof(anim_scared[0]), 5, 1);
											SetFaceAnimation(&opponentFace, anim_idle2, sizeof(anim_idle2) / sizeof(anim_idle2[0]), 5, 1);
										}
										else text_speech++;
									}
								}
								
								if(paddata.BTN_CROSS && !oldpad.BTN_CROSS && tfade) tfade = false;
							}
							else if(paddata.BTN_SELECT && !oldpad.BTN_SELECT) text_type = RandRange(6, 9);
						}
						else if(!controlling && !lost_game && !won_game)
						{
							static int wait_time = 0;
							static bool can_shoot_you = true;
							wait_time++;
							if(wait_time > RandRange(200, 380))
							{
								int chance = RandRange(0, 60);
								if(chance<15 && bullets_shooted>3 && com_can_shuffle && can_shoot_you)
								{
									com_can_shuffle = false;
									shuffle(bullets, bulletn);
								}
								else if(chance>35 && can_shoot_you)
								{
									printf("Shooting you...\n");
									can_shoot_you = false;
									text_type = RandRange(0, 5);
									bullets_shooted++;
									if(bullets[0])
									{
										SetFaceAnimation(&playerFace, anim_dead, sizeof(anim_dead) / sizeof(anim_dead[0]), 1, 0);
										SetFaceAnimation(&opponentFace, anim_shoot2, sizeof(anim_shoot2) / sizeof(anim_shoot2[0]), 5, 0);
										PlaySFX(&sfx_gunshot);
										lost_game = true;
									}
									else
									{
										SetFaceAnimation(&playerFace, anim_click, sizeof(anim_click) / sizeof(anim_click[0]), 5, 0);
										PlaySFX(&sfx_trigger);
										shoot_gun(bullets, bulletn);
									}
								}
								else
								{
									printf("Shooting myself...\n");
									bullets_shooted++;
									can_shoot_you = true;
									if(bullets[0])
									{
										SetFaceAnimation(&playerFace, anim_killed, sizeof(anim_killed) / sizeof(anim_killed[0]), 6, 0);
										SetFaceAnimation(&opponentFace, anim_dead2, sizeof(anim_dead2) / sizeof(anim_dead2[0]), 1, 0);
										printf("I lost. Ouchie\n");
										text_type = RandRange(0, 2);
										dialogue++;
										max_dial = 3;
										PlayBGM(2, 1);
										PlaySFX(&sfx_gunshot);
										won_game = true;
									}
									else
									{
										SetFaceAnimation(&opponentFace, anim_click2, sizeof(anim_click2) / sizeof(anim_click2[0]), 5, 0);
										SetFaceAnimation(&playerFace, anim_click, sizeof(anim_click) / sizeof(anim_click[0]), 5, 0);
										PlaySFX(&sfx_trigger);
										shoot_gun(bullets, bulletn);
										controlling = true;
									}
								}
								wait_time = 0;
							}
						}
						else if(won_game)
						{
							static int t = 0;
							t++;
							if(t>120)
							{
								GameSCR = SCR_CUTS;
								t = 0;
							}
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
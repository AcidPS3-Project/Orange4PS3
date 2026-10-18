#include <tiny3d.h>

#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"

#include "drawing.h"
#include "random.h"

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
	tiny3d_UserViewport(1, 0, 0, (float)(Video_Resolution.width / 916.0f), (float)(Video_Resolution.height / 582.0f),
		(float)(Video_Resolution.width / 1920.0f), (float)(Video_Resolution.height / 1080.0f));
	
	sysModuleLoad(SYSMODULE_PNGDEC);
	
	Initialize();
	
	init_bgm();
	init_sfx();
	
	ioPadInit(7);
    LoadTexture();
    
    //PlayBGM(0, 0);

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
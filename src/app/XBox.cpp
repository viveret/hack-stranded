#include "../app/XBox.hpp"


#include "../engine/Engine.hpp"

XBOXController::XBOXController()
{
	this->Reset();
}

void XBOXController::Reset()
{
    this->InUse=false;
//    this->LAnol[0]=0;this->LAnol[1]=0;this->RAnol[0]=0;this->RAnol[1]=0;
    this->LAnolDown=false;this->RAnolDown=false;this->STATE_LAnolDown=false;this->STATE_RAnolDown=false;
	this->LBump=false;this->RBump=false;
    this->LTrig=0;this->RTrig=0;
    this->A=false;this->STATE_A=false;this->B=false;this->STATE_B=false;this->X=false;this->STATE_X=false;this->Y=false;this->STATE_Y=false;
	this->Pad_L=false;this->Pad_R=false;this->Pad_U=false;this->Pad_D=false;
    this->Start=false;this->Select=false;this->STATE_Start=false;this->STATE_Select=false;
}	   


#ifdef USE_XBOX
#pragma comment (lib, "Xinput.lib")
#include <XInput.h>

void XBOXController::Check()
{
    DWORD isController;//Is it connected?
    _XINPUT_STATE ConState;//all states of controller
    ZeroMemory(&ConState,sizeof(_XINPUT_STATE));
    isController=XInputGetState(0,&ConState);
    if(isController==ERROR_SUCCESS)
    {
		//PXINPUT_KEYSTROKE WhatsDown;
		//XInputGetKeystroke(0,XINPUT_FLAG_GAMEPAD,WhatsDown);
        if(!this->InUse)
        {
            this->InUse=true;
			Engine.Application.Main_Log.Write_Log("Switched To Using Xbox Controller");
        }

        this->LTrig = As_Float(ConState.Gamepad.bLeftTrigger);
        this->RTrig = As_Float(ConState.Gamepad.bRightTrigger);

       
        if(abs(ConState.Gamepad.sThumbLX)>XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
			this->LAnol.x=(float)ConState.Gamepad.sThumbLX/XINPUT_ANALOG_MAX;
        }
		else{this->LAnol.x=0;}
        if(abs(ConState.Gamepad.sThumbLY)>XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
			this->LAnol.y=(float)ConState.Gamepad.sThumbLY/XINPUT_ANALOG_MAX;
        }
		else{this->LAnol.y=0;}																																																																															 
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)!=0 && !this->STATE_LAnolDown){this->STATE_LAnolDown=true;this->LAnolDown=true;Engine.Application.Key_Log.Write_Log("XBox Left Stick Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)==0 && this->STATE_LAnolDown){this->STATE_LAnolDown=false;this->LAnolDown=false;Engine.Application.Key_Log.Write_Log("XBox Left Stick Released");}
        if(abs(ConState.Gamepad.sThumbRX)>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
			this->RAnol.x=(float)ConState.Gamepad.sThumbRX/XINPUT_ANALOG_MAX;
        }
		else{this->RAnol.x=0;}
        if(abs(ConState.Gamepad.sThumbRY)>XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
			this->RAnol.y=(float)ConState.Gamepad.sThumbRY/XINPUT_ANALOG_MAX;
        }
		else{this->RAnol.y=0;}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)!=0 && !this->STATE_RAnolDown){this->STATE_RAnolDown=true;this->RAnolDown=true;Engine.Application.Key_Log.Write_Log("XBox Right Stick Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)==0 && this->STATE_RAnolDown){this->STATE_RAnolDown=false;this->RAnolDown=false;Engine.Application.Key_Log.Write_Log("XBox Right Stick Released");}

		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_A)!=0 && !this->STATE_A){this->STATE_A=true;this->A=true;Engine.Application.Key_Log.Write_Log("XBox A Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_A)==0 && this->STATE_A){this->STATE_A=false;this->A=false;Engine.Application.Key_Log.Write_Log("XBox A Released");}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_B)!=0 && !this->STATE_B){this->STATE_B=true;this->B=true;Engine.Application.Key_Log.Write_Log("XBox B Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_B)==0 && this->STATE_B){this->STATE_B=false;this->B=false;Engine.Application.Key_Log.Write_Log("XBox B Released");}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_X)!=0 && !this->STATE_X){this->STATE_X=true;this->X=true;Engine.Application.Key_Log.Write_Log("XBox X Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_X)==0 && this->STATE_X){this->STATE_X=false;this->X=false;Engine.Application.Key_Log.Write_Log("XBox X Released");}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)!=0 && !this->STATE_Y){this->STATE_Y=true;this->Y=true;Engine.Application.Key_Log.Write_Log("XBox Y Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)==0 && this->STATE_Y){this->STATE_Y=false;this->Y=false;Engine.Application.Key_Log.Write_Log("XBox Y Released");}

		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)!=0 && !this->STATE_Select){this->STATE_Select=true;this->Select=true;Engine.Application.Key_Log.Write_Log("XBox Select Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)==0 && this->STATE_Select){this->STATE_Select=false;this->Select=false;Engine.Application.Key_Log.Write_Log("XBox Select Released");}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_START)!=0 && !this->STATE_Start){this->STATE_Start=true;this->Start=true;Engine.Application.Key_Log.Write_Log("XBox Start Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_START)==0 && this->STATE_Start){this->STATE_Start=false;this->Start=false;Engine.Application.Key_Log.Write_Log("XBox Start Released");}

		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)!=0 && !this->LBump){this->LBump=true;Engine.Application.Key_Log.Write_Log("XBox Left Bumper Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)==0 && this->LBump){this->LBump=false;Engine.Application.Key_Log.Write_Log("XBox Left Bumper Released");}
		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)!=0 && !this->RBump){this->RBump=true;Engine.Application.Key_Log.Write_Log("XBox Right Bumper Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)==0 && this->RBump){this->RBump=false;Engine.Application.Key_Log.Write_Log("XBox Right Bumper Released");}

		if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)!=0 && !this->Pad_L){this->Pad_L=true;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Left Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)==0 && this->Pad_L){this->Pad_L=false;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Left Released");}
        if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)!=0 && !this->Pad_R){this->Pad_R=true;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Right Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)==0 && this->Pad_R){this->Pad_R=false;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Right Released");}
        if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)!=0 && !this->Pad_U){this->Pad_U=true;Engine.Application.Key_Log.Write_Log("XBox Dir PAd Up Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)==0 && this->Pad_U){this->Pad_U=false;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Up Released");}
        if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)!=0 && !this->Pad_D){this->Pad_D=true;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Down Pressed");}else if((ConState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)==0 && this->Pad_D){this->Pad_D=false;Engine.Application.Key_Log.Write_Log("XBox Dir Pad Down Released");}
    }
    else if(this->InUse)
    {
        this->InUse=false;
        std::stringstream ConRep;ConRep<<"NOT Using Xbox Controller";
		Engine.Application.Main_Log.Write(ConRep.str().c_str());
    }
}

void XBOXController::Do(float dt)
{
	if(this->InUse)
	{
		int xMove= (int)(this->RAnol.x*1.25f * dt), yMove=- (int)(this->RAnol.y*1.25f * dt);
		//if(APP.MouseX+xMove>0 && APP.MouseX+xMove<APP.Screenw){APP.MouseX+=xMove;}
		//if(APP.MouseY+yMove>0 && APP.MouseY+yMove<APP.Screenh){APP.MouseY+=yMove;}
		POINT mousePos;
		GetCursorPos(&mousePos);
		SetCursorPos(mousePos.x+xMove,mousePos.y+yMove);
	}
}



#endif

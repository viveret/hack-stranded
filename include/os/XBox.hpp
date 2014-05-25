#ifndef SURVIVOR_ENGINE_INPUT_XBOX
#define SURVIVOR_ENGINE_INPUT_XBOX

#include "../Core.hpp"

#include <string>
#include "../math/Math.hpp"

#define XINPUT_ANALOG_MAX 32767


class XBOXController
{
public:

	XBOXController();

    bool InUse;
    Vector2f LAnol,RAnol;
    bool LAnolDown,STATE_LAnolDown,RAnolDown,STATE_RAnolDown;
    bool LBump,RBump;
    float LTrig,RTrig;
    bool A,STATE_A,B,STATE_B,X,STATE_X,Y,STATE_Y;
	bool Pad_L,Pad_R,Pad_U,Pad_D;
    bool Start,STATE_Start,Select,STATE_Select;

    void Reset();
    void StartChecking();
    void StopChecking();
    void Check();
	void Do(float dt);
};


#endif

#include <Windows.h>
#include <SE/module/Module.hpp>

#ifdef _DEBUG
#pragma comment(lib, "../../bin/Survivor_Engine_Debug.lib")
#else
#pragma comment(lib, "../../bin/Survivor_Engine_Release.lib")
#endif

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	return S_Engine::Main( 1, &lpCmdLine );
}
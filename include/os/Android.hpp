#if !defined(SURVIVOR_ENGINE_OS_ANDROID) && defined(_ANDROID)

#include <SE/setl/Array.hpp>
#include <string>

namespace SE_OS_Android
{
	SE_Raw_Buffer Load_File( std::string File );

	std::string Get_Install_Dir();

	void Set_Android( void* State );
}



#endif

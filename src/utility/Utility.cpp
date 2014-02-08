#include <bass.h>
void Set_Master_Volume(float volume)
{
	if(volume>1)
	{
		//BASS_SetVolume(volume/99.f);
	}
	else
	{
		//BASS_SetVolume(volume);
	}
}

#include <string>
std::string File_Ext( std::string File )
{
	if( File.find('.') != File.npos )
		return File.substr( File.find_last_of('.') + 1, File.length() - File.find_last_of('.') - 1 );
	else
		return std::string();
}

#include <string>

bool dirExists(const std::string& dirName_in)
{
#ifdef _WIN32
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!
#endif

  return false;    // this is not a directory!
}




#ifndef _WIN32

#include <cstdio>
int itoa( int n, char* Buf, int rad )
{
	if( rad == 16 )
		sprintf( Buf, "%x", n );
	else
		sprintf( Buf, "%i", n );

	return 0;
}

int atoi( int n, char* Buf, int rad )
{
	return 0;
}


#endif



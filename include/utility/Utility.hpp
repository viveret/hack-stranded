/*

general purpose functions ie. master volum/opengl stuff

*/


#ifndef SURVIVOR_ENGINE_UTILITY
#define SURVIVOR_ENGINE_UTILITY

void Set_Master_Volume(float volume);
bool dirExists(const std::string& dirName_in);

std::string File_Ext( std::string File );

#ifndef _WIN32

int itoa( int n, char* Buf, int rad );
int atoi( int n, char* Buf, int rad );

#endif


#endif

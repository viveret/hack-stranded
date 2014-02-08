#include "File_Stats.hpp"
using namespace SE_Filesys;

File_Type_Desc::File_Type_Desc()
{
	this->Func_New = NULL;
	this->Func_Init = NULL;
	this->Func_Clean = NULL;
	this->Func_Stream = NULL;
}

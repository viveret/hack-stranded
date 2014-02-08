/*#include "File.h"

#include <time.h>
#include "App.h"
extern myApp APP;
void File::Open(const char* Dest)
{
    File::Opened=true;
    File::OutFile.open(Dest);
    File::InFile.open(Dest);
}

void File::Write(const char* stuff)
{
    if(File::Opened)
    {
        File::OutFile<<">"<<APP.Time->tm_hour<<":"<<APP.Time->tm_min<<":"<<APP.Time->tm_sec<<"--\n\t";
        File::OutFile<<stuff;
        File::OutFile<<"\n";
		File::OutFile.flush();
    }
}

std::string File::Read()
{
    std::stringstream Buf;
    if(File::Opened)
    {
            Buf<<File::InFile.rdbuf();
    }
    if(File::Opened==false){Buf<<"FAIL";}
    std::string Ret(Buf.str());
    return Ret;
}

void File::Close()
{
    File::Opened=false;
    File::OutFile.close();
    File::InFile.close();
}

void File::Flush()
{
    File::OutFile.flush();
}
				  */
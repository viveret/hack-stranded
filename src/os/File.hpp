#ifndef SURVIVOR_ENGINE_FILE
#define SURVIVOR_ENGINE_FILE

#include <fstream>
#include <sstream>
#include <string>

class File
{
    public:
    bool Opened;
    std::ofstream OutFile;
    std::ifstream InFile;
    void Close();
    void Write(const char* Write);
    std::string Read();
    void Flush();
    void Open(const char* Dest);
};

#endif // FILE_H_INCLUDED

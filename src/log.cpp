#include "log.h"
#include <iostream>
#include <memory>

using namespace std;

Log::Log():
    _out(std::cout.rdbuf())
{
}

Log::~Log()
{
    if(this->_file.is_open())
    {
        this->_file.flush();
        this->_file.close();
    }
}

void Log::Log_Init(const string &path)
{
#ifdef DEBUG
    string logfile = path + "/log.txt";
    Log::getInstance()._file.open(logfile, std::ios_base::out | std::ios_base::trunc);

    if(!Log::getInstance()._file.is_open())
    {
        cerr << "Create log file failed!"<<endl;
        Log::getInstance()._out.rdbuf(std::cout.rdbuf());
    }
    else
        Log::getInstance()._out.rdbuf(Log::getInstance()._file.rdbuf());
#endif
}

void Log::Log_Info(const string &strText, const char *file, const char * func, const int line)
{
    Log::getInstance()._out << "[Info]" << file << ":" << func << ":" << line << ":" << strText << endl;
}

void Log::Log_Warning(const string &strText, const char *file, const char * func, const int line)
{
    Log::getInstance()._out << "[Warning]" << file << ":" << func << ":" << line << ":" << strText << endl;
}

void Log::Log_Error(const string &strText, const char *file, const char * func, const int line)
{
    Log::getInstance()._out << "[Error]" << file << ":" << func << ":" << line << ":" << strText << endl;
}

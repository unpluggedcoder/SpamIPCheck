#ifndef LOG_H
#define LOG_H

#include <string>
#include <ostream>
#include <fstream>
#include <memory>

class noncopyable
  {
   protected:
#if defined(__cplusplus) && __cplusplus >= 201103L
    noncopyable() = default;
    ~noncopyable() = default;
#else
    noncopyable() {}
      ~noncopyable() {}
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
        noncopyable( const noncopyable& ) = delete;
        noncopyable& operator=( const noncopyable& ) = delete;
#else
    private:  // emphasize the following members are private
      noncopyable( const noncopyable& );
      noncopyable& operator=( const noncopyable& );
#endif
  };


template<typename T>
class Singleton: public noncopyable
{
public:
    static T & getInstance()
    {
        static T t;
        //assert(! detail::singleton_wrapper< T >::m_is_destroyed);
        use(_instance);
        return static_cast<T &>(t);
    }
private:
    static void use(T const &) {}
    static T& _instance;
};
template<class T>
T & Singleton<T>::_instance = Singleton<T>::getInstance();

class Log: public Singleton<Log>
{
public:
    Log();
    ~Log();

    void Log_Init(const std::string& path);
    void Log_Info(const std::string & strText,const char * file, const char * func, const int line);
    void Log_Warning(const std::string & strText,const char * file, const char * func, const int line);
    void Log_Error(const std::string & strText,const char * file, const char * func, const int line);

private:
    //static writelog(const std::string &line);
    std::ofstream _file;
    std::ostream _out;
};

#ifdef DEBUG
#define LOG_INFO(msg) Log::getInstance().Log_Info(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_WARNING(msg) Log::getInstance().Log_Warning(msg, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERROR(msg) Log::getInstance().Log_Error(msg, __FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_INFO(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#endif
#endif // LOG_H

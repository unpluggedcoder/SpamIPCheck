#ifndef TRMSERVER_H
#define TRMSERVER_H

#include <string>
#include <QMap>
#include "global.h"

using namespace std;

class trmServer
{
public:
    trmServer();
    typedef QMap<QString,QString> ResultType;

    static QString ServerName(){ return _servername;}

    static METHOD_TYPE HttpType(){ return HTTP_POST;}
    static SERVER_TYPE _Type;
    static QString GetRequestUrl(){ return _url;}
    static QString GetPostData(QString ip){ return _prefix + ip;}

    void SetResult(ResultType &result);
    std::shared_ptr<QString>  GetResult();
    CHECK_RESULT _currentresult; // use LISTED or NOT_LISTED only
private:
    static QString _servername;
    static QString _url;
    static QString _prefix;

    ResultType   _replycontent;
};

#endif // TRMSERVER_H

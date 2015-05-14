#ifndef ANTISPAMSERVER_H
#define ANTISPAMSERVER_H

#include <string>
#include <QMap>
#include "global.h"

using namespace std;

class antispamServer
{
public:
    antispamServer();
    typedef QMap<QString,QString> ResultType;

    static QString ServerName() { return _servername;}

    static METHOD_TYPE HttpType(){ return HTTP_GET;}
    static SERVER_TYPE _Type;
    static QString GetRequestUrl(QString ip){ return _url + ip;}

    void SetResult(ResultType &result);
    std::shared_ptr<QString>  GetResult();
    CHECK_RESULT  _currentresult; // use LISTED or NOT_LISTED only
private:
    static QString _servername;
    static QString _url;

    ResultType    _replycontent;
};

Q_DECLARE_METATYPE(antispamServer::ResultType);
#endif // ANTISPAMSERVER_H

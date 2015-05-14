#ifndef DNSBLSERVER_H
#define DNSBLSERVER_H

#include <string>
#include <map>
#include "global.h"
#include <QString>
#include <QPair>
#include <QMap>

using namespace std;

class dnsblServer
{
public:
    dnsblServer();
    typedef QPair<QString,QString> ResultType;  // server - imgUrl

    static QString ServerName() { return _servername;}

    static METHOD_TYPE HttpType(){ return HTTP_GET;}
    static bool InitToken(QString &html);
    static bool GetRequestUrl(QString ip, vector<QString> &vec);
    static SERVER_TYPE _Type;
    static QString   _InitUrl;
    static QString   _InitData;
    static QString   _cb;
    static QString   _token;

    void SetResult(ResultType &result);
    std::shared_ptr<QString>  GetResult();
    int GetListedCount()const {return _listedNum;}
    CHECK_RESULT    _currentresult;   // use LISTED or NOT_LISTED only

    static bool     _bInit;
private:
    static QString  _servername;
    std::shared_ptr<QString>        _resultString;
    int         _receivecount;

    int         _listedNum;
    QMap<QString,CHECK_RESULT>  _serversresult;  // results from each server, key - host name
};

Q_DECLARE_METATYPE(dnsblServer::ResultType);
#endif // DNSBLSERVER_H

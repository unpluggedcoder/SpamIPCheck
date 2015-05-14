#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <string>
#include "antispamserver.h"
#include "dnsblserver.h"
#include "trmServer.h"

using namespace std;

class Client: public QObject
{
    Q_OBJECT

public:
    explicit Client(QString &ip, QString &domain, QObject *parent = 0);

    void SetResult(QPair<QString, QString> result);
    void SetResult(QMap<QString, QString> result, servertype<antispamServer> nouse);
    void SetResult(QMap<QString, QString> result, servertype<trmServer> nouse);

    std::shared_ptr<QString> GetDnsblResult();
    std::shared_ptr<QString> GetAntiResult();
    std::shared_ptr<QString> GetTrmResult();
    int GetListedCount() const;
    QString GetDomain() const ;
    QString GetIP() const {return _ip;}

    bool operator <(const Client& lhs) const;
private:
    QString _ip;
    QString _domain;  // optional

    dnsblServer     _dnsblserver;
    antispamServer  _antiserver;
    trmServer       _trmserver;
};

#endif // CLIENT_H

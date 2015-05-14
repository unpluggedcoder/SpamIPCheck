#ifndef REPLYWORKER_H
#define REPLYWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <memory>
#include "global.h"

class antispamServer;
class trmServer;
class ReplyWorker : public QObject
{
    Q_OBJECT
public:
    explicit ReplyWorker(QObject *parent = 0);

signals:
    void PostMsg(std::shared_ptr<MsgInfo> msginfo);
    void AddResult(QString ip, QPair<QString, QString> result); // key:servername value:color, one of dnsblServer g_vecServList
    void AddResult(QString ip, QMap<QString, QString> result, servertype<antispamServer> nouse);
    void AddResult(QString ip, QMap<QString, QString> result, servertype<trmServer> nouse);
    void IncreaseReplyCount();
public slots:
    void Finished(QNetworkReply* reply);

protected:
    void ParseDnsblInit(QNetworkReply *reply);
    void ParseDnsblReply(QNetworkReply *reply);
    void ParseAntiReply(QNetworkReply *reply);
    void ParseTrendReply(QNetworkReply *reply);
};

#endif // REPLYWORKER_H

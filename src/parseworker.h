#ifndef PARSEWORKER_H
#define PARSEWORKER_H

#include <QObject>
#include <QHostInfo>
#include <memory>
#include "msginfo.h"

class ParseWorker : public QObject
{
    Q_OBJECT
public:
    explicit ParseWorker(QObject *parent = 0);

signals:
    void PostMsg(std::shared_ptr<MsgInfo> msg);
    void HttpGet(QString url);
    void HttpPost(QString url, QString data);
    void HttpsPost(QString url, QString data);
    void AddClient(QString ip, QString domain);

public slots:
    void RunfromFile(QString filepath);
    void RunOnce(QString ip);
    void PostRequest(QString ip);
    // IP parse
    void onHostLookup(const QHostInfo &host);

protected:
    void Reslove(QString &IPorDomain);

};

#endif // PARSEWORKER_H

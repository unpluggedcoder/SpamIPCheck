#ifndef REQUESTWORKER_H
#define REQUESTWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <memory>

class RequestWorker : public QObject
{
    Q_OBJECT
public:
    explicit RequestWorker(QObject *parent = 0);

signals:
    void Finished(QNetworkReply*);

public slots:
    void Init();
    void HttpGet(QString url);
    void HttpPost(QString url, QString data);
    void HttpsPost(QString url, QString data);

protected slots:
    void NetMgrFinished(QNetworkReply* reply);

private:
    std::shared_ptr<QNetworkAccessManager>  _networkaccessmgr;
};

#endif // REQUESTWORKER_H

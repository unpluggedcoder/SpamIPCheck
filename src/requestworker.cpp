#include "requestworker.h"
#include <QUrl>
#include <QSslConfiguration>

RequestWorker::RequestWorker(QObject *parent) :
    QObject(parent)
{
}

void RequestWorker::Init()
{
    // make object in its own thread
    _networkaccessmgr = std::make_shared<QNetworkAccessManager>();

    QObject::connect(_networkaccessmgr.get(), SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(NetMgrFinished(QNetworkReply*)));
}

void RequestWorker::HttpGet(QString url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QByteArray("Connection"), QByteArray("keep-alive"));
    request.setRawHeader(QByteArray("User-Agent"), QByteArray("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36"));

    _networkaccessmgr->get(request);
}

void RequestWorker::HttpPost(QString url, QString data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(QByteArray("Connection"), QByteArray("keep-alive"));
    request.setRawHeader(QByteArray("User-Agent"), QByteArray("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36"));

    QByteArray bytedata;
    bytedata.append(data);
    _networkaccessmgr->post(request,bytedata);
}

void RequestWorker::HttpsPost(QString url, QString data)
{
    QNetworkRequest sslrequest(url);
    sslrequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    sslrequest.setRawHeader(QByteArray("Connection"), QByteArray("keep-alive"));
    sslrequest.setRawHeader(QByteArray("User-Agent"), QByteArray("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.110 Safari/537.36"));

    //QSslConfiguration sslConfiguration(QSslConfiguration::defaultConfiguration());
    //sslConfiguration.setProtocol(QSsl::TlsV1_0);
    //sslrequest.setSslConfiguration(sslConfiguration);

    QByteArray bytedata;
    bytedata.append(data);
    _networkaccessmgr->post(sslrequest,bytedata);

}

void RequestWorker::NetMgrFinished(QNetworkReply *reply)
{
    emit Finished(reply);
}

#include "parseworker.h"
#include "global.h"
#include <QHostAddress>
#include <QHostInfo>
#include "dnsblserver.h"
#include "antispamserver.h"
#include "trmServer.h"
#include <QFile>

ParseWorker::ParseWorker(QObject *parent) :
    QObject(parent)
{
}

void ParseWorker::RunfromFile(QString filepath)
{
    // filepath "file:///D:/******/***.txt"
    filepath.replace(0,strlen("file:///"),"");
    QFile file(filepath);

    if(!file.exists())
    {
        EMIT_ERROR(filepath + " is not vaild path for an exist file");
        return;
    }

    if(file.open(QIODevice::ReadOnly))
    {
        QString line = file.readLine();
        while(line.length())
        {
            line = line.trimmed();
            Reslove(line);

            line = file.readLine();
        }
    }
    else
    {
        EMIT_ERROR(file.errorString() + ": " + filepath);
    }
}

void ParseWorker::RunOnce(QString ip)
{
    Reslove(ip);;
}

/*
 *
 // To find the IP address of qt.nokia.com
 QHostInfo::lookupHost("qt.nokia.com",
                       this, SLOT(printResults(QHostInfo)));

 // To find the host name for 4.2.2.1
 QHostInfo::lookupHost("4.2.2.1",
                       this, SLOT(printResults(QHostInfo)));

    bool QHostAddress::setAddress ( const QString & address );
    Returns true and sets the address if the address was successfully parsed; otherwise returns false.
*/
void ParseWorker::onHostLookup(const QHostInfo &host)
{
    // 1.If no error occured, emit AddClient signal
    if (host.error() != QHostInfo::NoError)
    {
        EMIT_ERROR(host.errorString() + ": " +host.hostName())
        return;
    }
    QString ip = host.addresses().first().toString();
    QString hostname = host.hostName();
    emit AddClient(ip, hostname);

    //PostRequest(ip);
}

void ParseWorker::Reslove(QString &IPorDomain)
{
    QHostInfo::lookupHost(IPorDomain, this, SLOT(onHostLookup(QHostInfo)));
}

void ParseWorker::PostRequest(QString ip)
{
    QString url, data;
    std::vector<QString> urllist;
    urllist.reserve(g_vecServList.size());

    // dnsbl.info
    if(dnsblServer::GetRequestUrl(ip, urllist))
    {
        int size = urllist.size();
        for(int i = 0; i < size; ++i)
            emit HttpGet(urllist[i]);
    }

    // anti-spam.org.cn
    url = antispamServer::GetRequestUrl(ip);
    emit HttpGet(url);

    // TrendMicro.com

    url = trmServer::GetRequestUrl();
    data = trmServer::GetPostData(ip);
    emit HttpsPost(url, data);
}

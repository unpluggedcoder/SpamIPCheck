#include "replyworker.h"
#include "antispamserver.h"
#include "dnsblserver.h"
#include "trmServer.h"
#include <QNetworkReply>
#include <QNetworkRequest>

ReplyWorker::ReplyWorker(QObject *parent) :
    QObject(parent)
{
}

void ReplyWorker::Finished(QNetworkReply *reply)
{
    bool isFirst = false;
    if(reply->error() == QNetworkReply::NoError)
    {
        QUrl url = reply->url();
        QString hostname = url.host();

        SERVER_TYPE servertype;
        if(hostname.contains(dnsblServer::ServerName(), Qt::CaseInsensitive))
            servertype = DNSBL_INFO;
        else if(hostname.contains(antispamServer::ServerName(), Qt::CaseInsensitive))
            servertype = ANTI_SPAM_ORG_CN;
        else if(hostname.contains(trmServer::ServerName(), Qt::CaseInsensitive))
            servertype = TRENDMICRO_COM;

        switch(servertype)
        {
        case DNSBL_INFO:
        {
            QString strurl = url.toString();
            if(strurl.contains("dnsbl-database-check.php", Qt::CaseInsensitive))
            {
                ParseDnsblInit(reply); // get _cb and _token from first init
                isFirst = true;
            }
            else
                ParseDnsblReply(reply);
        }
            break;
        case ANTI_SPAM_ORG_CN:
            ParseAntiReply(reply);
            break;
        case TRENDMICRO_COM:
            ParseTrendReply(reply);
            break;
        default:
            // emit error msginfo
            EMIT_ERROR("Couldn`t resolve server name: " + hostname)
            break;
        }
    }
    else
    {
        //emit error msginfo
        QString error = reply->errorString();
        EMIT_ERROR(error)
    }
    if(!isFirst)
        emit IncreaseReplyCount();
    reply->close();
    reply->deleteLater();
}

void ReplyWorker::ParseDnsblInit(QNetworkReply *reply)
{
    QString html = reply->readAll();

    bool bRet = dnsblServer::InitToken(html);

    if(!bRet)
    {
        EMIT_ERROR("Init DNSBL.info server failed!")
    }
}

void ReplyWorker::ParseDnsblReply(QNetworkReply *reply)
{
    // get ip
    QString url = reply->url().toString();
    static QRegExp regexp("\\?bl=([^&]*)&ip=([^&]*)&cb", Qt::CaseInsensitive);
    int pos = url.indexOf(regexp);
    Q_ASSERT(pos != -1);
    QString ip, host;
    if(pos != -1)
    {
        host = regexp.cap(1);
        ip = regexp.cap(2);
    }
    else
    {
        EMIT_ERROR( url + ", Couldn`t get ip and host from url")
        return;
    }

    if(reply->hasRawHeader("Location"))
    {
        QString imageUrl = reply->rawHeader("Location");
        emit AddResult(ip,QPair<QString, QString>(host,imageUrl));
        return;
    }
    else
    {// ERROR result
        EMIT_ERROR("Couldn`t get result from Header")
        return;
    }
}

void ReplyWorker::ParseAntiReply(QNetworkReply *reply)
{
    // get ip
    QNetworkRequest req = reply->request();
    QString reqstring = req.url().toString();
    static QRegExp ipregexp("search/(.*)", Qt::CaseInsensitive);
    int ip_pos = reqstring.indexOf(ipregexp); // http://www.anti-spam.org.cn/search/8.8.8.8
    Q_ASSERT(ip_pos != -1);
    QString ip;
    if(ip_pos != -1)
        ip = ipregexp.cap(1);
    else
    {
        EMIT_ERROR( req.url().toString() + ", ouldn`t get ip from url")
        return;
    }

    QString html = reply->readAll();

    static const QString pattern("<span class=\"leftMz\">[\\s]*([^<]*)[\\s]*</span>[\\s]*<span>[\\s]*([^<]*)[\\s]*</span>");
    static QRegExp regexp(pattern);
    QMap<QString, QString> mapres;
    int pos = 0;
    while(-1 != (pos = html.indexOf(regexp, pos)))
    {
        QString key = regexp.cap(1);
        QString value = regexp.cap(2);
        key = key.trimmed();
        value = value.trimmed();
        mapres[key] = value;
        pos += + regexp.cap(0).length();
    }
    if(!mapres.contains(QStringLiteral("IP状态")))
        mapres[ERROR_LABEL] = "Parse server`s reply failed!";
    emit AddResult(ip, mapres, servertype<antispamServer>());
}

/*
 * <dt>IP:</dt>
   <dd>8.8.8.8</dd>
   <dt>Reputation:</dt>
   <dd>Unlisted in the spam sender list</dd>
   <dt>Listed in:</dt>
   <dd>None</dd>
 */
void ReplyWorker::ParseTrendReply(QNetworkReply *reply)
{
    QString html = reply->readAll();
    static const QString key_pattern("<dt>[\\s]*([^<]*)[\\s]*</dt>");
    static const QString value_pattern("<dd[\\s]*[^>]*>[\\s]*([^<]*)[\\s]*</dd>");
    static const QString listed_pattern("<dd[\\s]*[^>]*>[\\s]*<a[^>]*>([^<]*)</a>");
    static QRegExp key_regexp(key_pattern);
    static QRegExp value_regexp(value_pattern);
    static QRegExp listed_regexp(listed_pattern);

    QMap<QString, QString> mapres;
    int pos = 0, count = 0;
    while(-1 != (pos = html.indexOf(key_regexp, pos)) && count < 3)
    {
        QString key = key_regexp.cap(1);
        key = key.trimmed();

        int old_pos = pos;
        int p1 = html.indexOf(value_regexp, pos);
        int p2 = html.indexOf(listed_regexp, pos);
        if(-1 != p1)
        {
            QString value = value_regexp.cap(1);
            value = value.trimmed();
            mapres[key] = value;
            pos = p1 + value_regexp.cap(0).length();
        }
        else if(-1 != p2)
        {
            QString value = listed_regexp.cap(1);
            value = value.trimmed();
            mapres[key] = value;
            pos = p2 + listed_regexp.cap(0).length();
        }
        else
            pos = old_pos;
        ++count;
    }

    QString ip;
    if(mapres.contains("IP:"))
    {
        ip = mapres["IP:"];
        emit AddResult(ip, mapres, servertype<trmServer>());
    }
    else
    {
        QString hostname = reply->url().host();
        EMIT_ERROR(hostname + QString(", Unknown format msg from server!"))
    }
}

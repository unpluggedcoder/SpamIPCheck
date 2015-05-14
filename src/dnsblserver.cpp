#include "dnsblserver.h"
#include <QRegExp>

vector<QString> g_vecServList = {
    "b.barracudacentral.org",
    "bl.emailbasura.org",
    "bl.spamcop.net",
    "blackholes.five-ten-sg.com",
    "blacklist.woody.ch",
    "bogons.cymru.com",
    "cbl.abuseat.org",
    "combined.abuse.ch",
    "cdl.anti-spam.org.cn",
    "bl.spamcannibal.org",
    "db.wpbl.info",
    "combined.rbl.msrbl.net",
    "dnsbl-1.uceprotect.net",
    "dnsbl-2.uceprotect.net",
    "dnsbl-3.uceprotect.net",
    "dnsbl.ahbl.org",
    "dnsbl.cyberlogic.net",
    "dnsbl.inps.de",
    "dnsbl.sorbs.net",
    "drone.abuse.ch",
    "drone.abuse.ch",
    "duinv.aupads.org",
    "dul.dnsbl.sorbs.net",
    "dul.ru",
    "dyna.spamrats.com",
    "dynip.rothen.com",
    "http.dnsbl.sorbs.net",
    "images.rbl.msrbl.net",
    "ips.backscatterer.org",
    "ix.dnsbl.manitu.net",
    "korea.services.net",
    "misc.dnsbl.sorbs.net",
    "noptr.spamrats.com",
    "ohps.dnsbl.net.au",
    "omrs.dnsbl.net.au",
    "orvedb.aupads.org",
    "osps.dnsbl.net.au",
    "osrs.dnsbl.net.au",
    "owfs.dnsbl.net.au",
    "owps.dnsbl.net.au",
    "pbl.spamhaus.org",
    "phishing.rbl.msrbl.net",
    "probes.dnsbl.net.au",
    "proxy.bl.gweep.ca",
    "proxy.block.transip.nl",
    "psbl.surriel.com",
    "rbl.interserver.net",
    "rbl.megarbl.net",
    "rdts.dnsbl.net.au",
    "relays.bl.gweep.ca",
    "relays.bl.kundenserver.de",
    "relays.nether.net",
    "residential.block.transip.nl",
    "ricn.dnsbl.net.au",
    "rmst.dnsbl.net.au",
    "sbl.spamhaus.org",
    "short.rbl.jp",
    "smtp.dnsbl.sorbs.net",
    "socks.dnsbl.sorbs.net",
    "spam.abuse.ch",
    "spam.dnsbl.sorbs.net",
    "spam.rbl.msrbl.net",
    "spam.spamrats.com",
    "spamlist.or.kr",
    "t3direct.dnsbl.net.au",
    "spamrbl.imp.ch",
    "tor.ahbl.org",
    "tor.dnsbl.sectoor.de",
    "torserver.tor.dnsbl.sectoor.de",
    "ubl.lashback.com",
    "ubl.unsubscore.com",
    "virbl.bit.nl",
    "virus.rbl.jp",
    "virus.rbl.msrbl.net",
    "web.dnsbl.sorbs.net",
    "wormrbl.imp.ch",
    "xbl.spamhaus.org",
    "zen.spamhaus.org",
    "zombie.dnsbl.sorbs.net",
};

QString dnsblServer::_servername = QString("DNSBL.info");
SERVER_TYPE dnsblServer::_Type = DNSBL_INFO;
QString dnsblServer::_InitUrl = "http://www.dnsbl.info/dnsbl-database-check.php";
QString dnsblServer::_InitData = "IP=8.8.8.8&Lookup IP Address.x=58&Lookup IP Address.y=18&Lookup IP Address=Lookup IP Address";

QString dnsblServer::_cb = "";
QString dnsblServer::_token = "";
bool dnsblServer::_bInit = false;

dnsblServer::dnsblServer()
{
    _currentresult = UNKNOWN;
    _listedNum = 0;
    _receivecount = 0;
    _resultString = std::make_shared<QString>();
}

bool dnsblServer::InitToken(QString &html)
{
    QString pattern("&(?:amp;)cb=([0-9]+)&(?:amp;)token=([0-9a-zA-Z]+)");
    QRegExp regexp(pattern);

    int pos = html.indexOf(regexp);
    if(-1 != pos)
    {
         _cb = regexp.cap(1);
         _token = regexp.cap(2);
         _bInit = true;
         return true;
    }
    return false;
}

bool dnsblServer::GetRequestUrl(QString ip, vector<QString> &vec)
{
    if(!_bInit)
        return false;

    int size = g_vecServList.size();
    for(int i = 0; i < size; ++i)
    {
        QString buffer = QString("http://www1.dnsbl.info/test.php?bl=%1&ip=%2&cb=%3&token=%4").arg(g_vecServList[i]).arg(ip).arg(_cb).arg(_token);
        vec.push_back(buffer);
    }
    return true;
}


static std::vector<QString> fontcolor = {
    "green",
    "red",
    "lightblue",
    "gray"
};


void dnsblServer::SetResult(ResultType &result)
{
    // http://cdn.dnsbl.info/images/ball_green_15x15.gif
    QString imageUrl = result.second;

    CHECK_RESULT ret = UNKNOWN;
    if(imageUrl.contains("green", Qt::CaseInsensitive))
        ret = NOT_LISTED;
    else if (imageUrl.contains("red", Qt::CaseInsensitive))
    {
        ret = LISTED;
        ++_listedNum;
    }
    else if (imageUrl.contains("blue", Qt::CaseInsensitive))
        ret = TIMEOUT;
    else if (imageUrl.contains("gray", Qt::CaseInsensitive))
        ret = OFFLINE;
    else
        ret = SERV_ERROR;
    _serversresult[result.first] = ret;

    _currentresult = ret > _currentresult ? ret : _currentresult;
    ++_receivecount;

    if(1 == _receivecount%4)
        _resultString->append("<tr>");
    QString item = QString("<font color=\"%1\">%2 </font>").arg(fontcolor[ret-1]).arg(result.first);
    _resultString->append(item);
    if(0 == _receivecount%4)
        _resultString->append("</tr>");
}


std::shared_ptr<QString> dnsblServer::GetResult()
{
//    std::shared_ptr<QString> result(new QString(""));
//    int count = 1;
//    QMap<QString,CHECK_RESULT>::iterator it = _serversresult.begin();
//    for(; it != _serversresult.end(); ++it)
//    {
//        if(1 == count%4)
//            result->append(QString("<tr>"));//<td width=\"25%\"></td>

//        result->append(res);
//        if(0 == count%4 || (it + 1) == _serversresult.end())
//            result->append(QString("</tr>"));
//        ++count;
//    }
    return _resultString;
}

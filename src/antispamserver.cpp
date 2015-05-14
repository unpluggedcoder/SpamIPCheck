#include "antispamserver.h"

QString antispamServer::_servername = QString("Anti-Spam.org.cn");
QString antispamServer::_url = "http://anti-spam.org.cn/search/";
SERVER_TYPE antispamServer::_Type = ANTI_SPAM_ORG_CN;

antispamServer::antispamServer()
{
    _currentresult = UNKNOWN;
}

void antispamServer::SetResult(ResultType &result)
{
    _replycontent = result;
    if(_replycontent.contains(QStringLiteral("IP状态")))
    {
        QString status = _replycontent[QStringLiteral("IP状态")];
        if(status.contains(QStringLiteral("已列入")))
            _currentresult = LISTED;
        else if(status.contains(QStringLiteral("未列入")))
            _currentresult = NOT_LISTED;
    }
    else
    {
        // if error occured, the result map would contain an "ERROR_LABEL" key, and the value specified the erro info.
        // And the server _currentresult must be ERROR
        _currentresult = SERV_ERROR;
        _replycontent[ERROR_LABEL] = "Couldn`t parse server`s reply!";
    }
}

std::shared_ptr<QString> antispamServer::GetResult()
{
    std::shared_ptr<QString> result(new QString(""));
    int count = 1;
    ResultType::iterator it = _replycontent.begin();
    for(; it != _replycontent.end(); ++it)
    {
        if(1 == count%2)
            result->append("<tr>");
        QString res = QString("<td width=\"50%%\">%1: %2</td>").arg(it.key()).arg(it.value());
        result->append(res);
        if(0 == count%2 || (it + 1) == _replycontent.end())
            result->append("</tr>");
        ++count;
    }
    return result;
}

#include "trmServer.h"

QString trmServer::_servername = QString("TrendMicro.com");
QString trmServer::_url = "https://ers.trendmicro.com/reputations";
QString trmServer::_prefix = "_method=POST&data[Reputation][ip]=";
SERVER_TYPE trmServer::_Type = TRENDMICRO_COM;

trmServer::trmServer()
{
    _currentresult = UNKNOWN;
}

void trmServer::SetResult(ResultType &result)
{
    _replycontent = result;
    if(_replycontent.contains("Listed in:"))
    {
        QString& status = _replycontent["Listed in:"];
        if(!status.compare("None", Qt::CaseInsensitive))
            _currentresult = NOT_LISTED;
        else
            _currentresult = LISTED;
    }
    else
    {
        // if error occured, the result map would contain an "ERROR_LABEL" key, and the value specified the erro info.
        // And the server _currentresult must be ERROR
        _currentresult = SERV_ERROR;
        _replycontent[ERROR_LABEL] = "Couldn`t parse server`s reply!";
    }
}

std::shared_ptr<QString> trmServer::GetResult()
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


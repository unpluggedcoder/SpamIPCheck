#include "client.h"


Client::Client(QString &ip, QString &domain, QObject *parent)
    :_ip(ip),_domain(domain)
{

}

void Client::SetResult(QPair<QString, QString> result)
{
    _dnsblserver.SetResult(result);
}

void Client::SetResult(QMap<QString, QString> result, servertype<antispamServer> nouse)
{
    _antiserver.SetResult(result);
}

void Client::SetResult(QMap<QString, QString> result, servertype<trmServer> nouse)
{
    _trmserver.SetResult(result);
}

std::shared_ptr<QString> Client::GetDnsblResult()
{
    return _dnsblserver.GetResult();
}

std::shared_ptr<QString> Client::GetAntiResult()
{
    return _antiserver.GetResult();
}

std::shared_ptr<QString> Client::GetTrmResult()
{
    return _trmserver.GetResult();
}

int Client::GetListedCount() const
{
    int num = _dnsblserver.GetListedCount();
    if(LISTED == _antiserver._currentresult)
        ++num;
    if(LISTED == _trmserver._currentresult)
        ++num;
    return num;
}

QString Client::GetDomain() const
{
    if(_ip == _domain)
        return QString("");
    else
        return QString("("+_domain + ")");
}

bool Client::operator <(const Client &lhs) const
{
//    int lsum = lhs._antiserver._currentresult + lhs._dnsblserver._currentresult + lhs._trmserver._currentresult;
//    int rsum = _antiserver._currentresult + _dnsblserver._currentresult + _trmserver._currentresult;

//    if(lsum == 0 && rsum == 0)
//        return _ip < lhs._ip;// not start yet

//    bool l_islisted = lhs._antiserver._currentresult > NOT_LISTED || lhs._dnsblserver._currentresult > NOT_LISTED
//                   || lhs._trmserver._currentresult > NOT_LISTED;

//    bool r_islisted = _antiserver._currentresult > NOT_LISTED || _dnsblserver._currentresult > NOT_LISTED
//                   || _trmserver._currentresult > NOT_LISTED;

    int rsum = this->GetListedCount();
    int lsum = lhs.GetListedCount();

    return rsum < lsum;
}

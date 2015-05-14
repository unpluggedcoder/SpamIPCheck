#include "resultmodel.h"
#include "global.h"

ResultModel::ResultModel(Controller *ctrler, QObject *parent) :
    QAbstractListModel(parent)
{
    _pCtrl = ctrler;
}

int ResultModel::rowCount(const QModelIndex &parent) const
{
    return _list.size();
}

QVariant ResultModel::data(const QModelIndex &index, int role) const
{
    int idx = index.row();
    int sum = _list.size();
    if (idx < 0 || idx > sum)
        return QVariant();

    switch(role)
    {
    case IPRole:{
        return _list[idx]->GetIP();
    }
    case DomainRole:{
        return _list[idx]->GetDomain();
    }
    case ListedCountRole:{
        return _list[idx]->GetListedCount();
    }
    case DnsblRole:{
        QString result = QString(*_list[idx]->GetDnsblResult());
        return result;
    }
    case AntiRole:{
        QString result = QString(*_list[idx]->GetAntiResult());
        return result;
    }
    case TrmRole:{
        QString result = QString(*_list[idx]->GetTrmResult());
        return result;
    }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> ResultModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IPRole] = "ip";
    roles[DomainRole] = "domain";
    roles[ListedCountRole] = "listedNum";
    roles[DnsblRole] = "dnsblResult";
    roles[AntiRole] = "antiResult";
    roles[TrmRole] = "trmResult";
    return roles;
}

void ResultModel::updateList(std::shared_ptr<Client> client)
{
    beginInsertRows(QModelIndex(), _list.size(), _list.size());
    _list.push_back(client);
    endInsertRows();
}

void ResultModel::updateData(QString ip)
{
    size_t idx = 0;
    for(; idx < _list.size(); ++idx)
        if(_list[idx]->GetIP() == ip)
            break;

    emit dataChanged(index(idx), index(idx));
}

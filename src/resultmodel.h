#ifndef RESULTMODEL_H
#define RESULTMODEL_H

#include <QAbstractListModel>
#include "controller.h"
#include <vector>

class ResultModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ResultModel(Controller *ctrler, QObject *parent = 0);

    enum ResultRoles {
             IPRole = Qt::UserRole + 1,
             DomainRole,
             ListedCountRole,
             DnsblRole,
             AntiRole,
             TrmRole
         };

    // overload functions
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QHash<int,QByteArray> roleNames() const;
signals:

public slots:
    void updateList(std::shared_ptr<Client> client);
    void updateData(QString ip);
private:
    Controller *_pCtrl;
    vector<std::shared_ptr<Client>>  _list;
};

#endif // RESULTMODEL_H

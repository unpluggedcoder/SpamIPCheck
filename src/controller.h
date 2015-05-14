#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <map>
#include <memory>
#include <QPair>
#include <QThreadPool>
#include "client.h"
#include "global.h"

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool errorstatus READ getErrorStatus WRITE setErrorStatus NOTIFY onError)

public:
    explicit Controller(QObject *parent = 0)
        :QObject(parent)
    {
        _error = false;
        _receiveCount = 0;
        _maxCount = 0;
    }

    ~Controller()
    {
    }

signals:
    void HttpPost(QString url, QString data);
    void RunfromFile(QString file);
    void RunOnce(QString ip);
    void InitWorker();
    void onError();
    void showMsg(QString msg);
    void updateValue(qint64 value);
    void updateMax(qint64 max);
    void updateList(std::shared_ptr<Client> client);
    void dataChanged(QString ip);
    void run(QString ip);// start Http(s) POST/GET

public slots:
    // UI slots
    void SingleCheck(QString ip);
    void ImportFile(QString file);
    void onOutputMsg(std::shared_ptr<MsgInfo> msg);
    void IncreaseValue();
    // error status
    bool getErrorStatus(){return _error;}
    void setErrorStatus(bool status){
        _error = status;
        emit onError();
    }
    void ResetErrorStatus(){setErrorStatus(false);}


    // Process singals from _ipParseThread
    void AddClient(QString ip, QString domain);

    // Process singals from _replyParesThread
    void AddResult(QString ip, QPair<QString, QString> result); // key:servername, one of dnsblServer g_vecServList
    void AddResult(QString ip, QMap<QString, QString> result, servertype<antispamServer> nouse);
    void AddResult(QString ip, QMap<QString, QString> result, servertype<trmServer> nouse);

public:
    typedef QMap<QString, std::shared_ptr<Client>> ModelType;
    void Init();
    void Quit();
    ModelType  _datamodel;// key: ip string; All data stores in this datamodel

private:
    std::shared_ptr<QThread> _requestThread;
    std::shared_ptr<QThread> _replyParesThread;
    std::shared_ptr<QThread> _ipParseThread;
    bool   _error;
    qint64 _receiveCount;
    qint64 _maxCount;
};

typedef servertype<antispamServer>  antiServerType;
typedef servertype<trmServer>       trmServerType;
typedef std::shared_ptr<Client>     clientType;

Q_DECLARE_METATYPE(antiServerType);
Q_DECLARE_METATYPE(trmServerType);
Q_DECLARE_METATYPE(clientType);
#endif // CONTROLLER_H

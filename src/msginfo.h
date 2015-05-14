#ifndef MSGINFO_H
#define MSGINFO_H

#include <QObject>
#include <memory>
#include <QMetaType>
#include <iostream>

enum MSG_TYPE
{
    MSG_INFO = 0,
    MSG_WARNING,
    MSG_ERROR
};

class MsgInfo : public QObject
{
    Q_OBJECT
public:
    explicit MsgInfo(MSG_TYPE type, QString msg,QObject *parent = 0);
    QString Message();
    MSG_TYPE Type(){return _type;}

signals:

public slots:


private:
    QString     _msg;
    MSG_TYPE    _type;
};

Q_DECLARE_METATYPE(std::shared_ptr<MsgInfo>);

#endif // MSGINFO_H

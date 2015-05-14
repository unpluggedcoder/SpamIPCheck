#include "msginfo.h"
#include <vector>
static std::vector<QString> TypeStringList =
{
    "",
    "[WARNING]",
    "[ERROR]"
};

MsgInfo::MsgInfo(MSG_TYPE type, QString msg, QObject *parent) :
    QObject(parent),_msg(msg)
{
    _type = type;
}

QString MsgInfo::Message()
{
    QString msg = QString("<font color=\"red\">%1</font>%2<br>").arg(TypeStringList[_type]).arg(_msg);
    return msg;
}

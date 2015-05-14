#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
#include "msginfo.h"

enum METHOD_TYPE
{
    HTTP_POST = 0,
    HTTP_GET
};

enum CHECK_RESULT
{
    UNKNOWN = 0,    // No get reply yet
    NOT_LISTED,     // IP clean
    LISTED,
    TIMEOUT,
    OFFLINE,        // Server unreachable
    SERV_ERROR           // other error occured
};

enum SERVER_TYPE
{
    DNSBL_INFO = 0,
    ANTI_SPAM_ORG_CN,
    TRENDMICRO_COM
};

#define ERROR_LABEL "Error"
#define FUNC_INFO(x) QString(Q_FUNC_INFO) + QString(x)

#define EMIT_INFO(msg) \
    std::shared_ptr<MsgInfo> msgInfo(new MsgInfo(MSG_INFO, FUNC_INFO(": ") + QString(msg))); emit PostMsg(msgInfo);

#define EMIT_WARNING(msg) \
    std::shared_ptr<MsgInfo> msgInfo(new MsgInfo(MSG_WARNING, FUNC_INFO(": ") + QString(msg))); emit PostMsg(msgInfo);

#define EMIT_ERROR(msg) \
    std::shared_ptr<MsgInfo> msgInfo(new MsgInfo(MSG_ERROR, FUNC_INFO(": ") + QString(msg))); emit PostMsg(msgInfo);

template<typename ServerType>
class servertype
{
    typedef ServerType type;
    // empty class template, used to identify the sever type
};

extern std::vector<QString> g_vecServList;

class Global
{
public:
    Global();

};

#endif // GLOBAL_H

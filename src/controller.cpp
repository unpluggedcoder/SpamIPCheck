#include "controller.h"
#include "requestworker.h"
#include "replyworker.h"
#include "parseworker.h"

void Controller::Init()
{
    onOutputMsg(std::make_shared<MsgInfo>(MSG_INFO,QString(QString("Spam IP Check    ver.") + QString(__DATE__))));

    _ipParseThread = std::make_shared<QThread>();
    _requestThread = std::make_shared<QThread>();
    _replyParesThread = std::make_shared<QThread>();
    //_error = std::make_shared<bool>();
    //*_error = false;

    ParseWorker *parseworker = new ParseWorker;
    parseworker->moveToThread(_ipParseThread.get());
    QObject::connect(_ipParseThread.get(), SIGNAL(finished()), parseworker, SLOT(deleteLater()));

    RequestWorker *requestworker = new RequestWorker;
    requestworker->moveToThread(_requestThread.get());
    QObject::connect(_requestThread.get(), SIGNAL(finished()), requestworker, SLOT(deleteLater()));

    ReplyWorker *replyworker = new ReplyWorker;
    replyworker->moveToThread(_replyParesThread.get());
    QObject::connect(_replyParesThread.get(), SIGNAL(finished()), replyworker, SLOT(deleteLater()));

    // controller file or ip -> parse worker
    QObject::connect(this, SIGNAL(RunfromFile(QString)),
                     parseworker, SLOT(RunfromFile(QString)));
    QObject::connect(this, SIGNAL(RunOnce(QString)),
                     parseworker, SLOT(RunOnce(QString)));
    QObject::connect(this, SIGNAL(run(QString)), parseworker, SLOT(PostRequest(QString)));

    // parse worker -> controller
    QObject::connect(parseworker, SIGNAL(PostMsg(std::shared_ptr<MsgInfo>)),
                     this, SLOT(onOutputMsg(std::shared_ptr<MsgInfo>)));
    QObject::connect(parseworker, SIGNAL(AddClient(QString,QString)),
                     this, SLOT(AddClient(QString,QString)));

    // url & data -> net request worker
    QObject::connect(this, SIGNAL(InitWorker()), requestworker, SLOT(Init()));
    QObject::connect(this, SIGNAL(HttpPost(QString,QString)), requestworker, SLOT(HttpPost(QString,QString)));
    QObject::connect(parseworker, SIGNAL(HttpPost(QString,QString)), requestworker, SLOT(HttpPost(QString,QString)));
    QObject::connect(parseworker, SIGNAL(HttpGet(QString)), requestworker, SLOT(HttpGet(QString)));
    QObject::connect(parseworker, SIGNAL(HttpsPost(QString,QString)), requestworker, SLOT(HttpsPost(QString,QString)));

    // net request -> reply worker
    QObject::connect(requestworker, SIGNAL(Finished(QNetworkReply*)),
                     replyworker, SLOT(Finished(QNetworkReply*)));

    // reply -> controller
    QObject::connect(replyworker, SIGNAL(PostMsg(std::shared_ptr<MsgInfo>)),
                     this, SLOT(onOutputMsg(std::shared_ptr<MsgInfo>)));
    QObject::connect(replyworker, SIGNAL(AddResult(QString,QPair<QString,QString>)),
                     this, SLOT(AddResult(QString,QPair<QString,QString>)));
    QObject::connect(replyworker, SIGNAL(AddResult(QString,QMap<QString,QString>,servertype<antispamServer>)),
                     this, SLOT(AddResult(QString,QMap<QString,QString>,servertype<antispamServer>)));
    QObject::connect(replyworker, SIGNAL(AddResult(QString,QMap<QString,QString>,servertype<trmServer>)),
                     this, SLOT(AddResult(QString,QMap<QString,QString>,servertype<trmServer>)));
    QObject::connect(replyworker, SIGNAL(IncreaseReplyCount()), this, SLOT(IncreaseValue()));

    _replyParesThread->start();
    _requestThread->start();
    _ipParseThread->start();
    emit InitWorker();

    // Post init query for get _cb and _token
    QString url = QString(dnsblServer::_InitUrl);
    QString data = QString(dnsblServer::_InitData);
    emit HttpPost(url, data);
    //QThread::sleep(5);
    //emit RunOnce("baidu.com");
    //emit RunfromFile("j:/Qt.txt");
}

void Controller::Quit()
{
    _ipParseThread->quit();
    _ipParseThread->wait();

    _requestThread->quit();
    _requestThread->wait();

    _replyParesThread->quit();
    _replyParesThread->wait();
}

void Controller::SingleCheck(QString ip)
{// doesn`t check string format in controller
    emit RunOnce(ip);
}

void Controller::ImportFile(QString file)
{
    emit RunfromFile(file);
}

void Controller::onOutputMsg(std::shared_ptr<MsgInfo> msg)
{
    QString Msg = msg->Message();
    qDebug()<< Msg;

    if(msg->Type() == MSG_ERROR)
        setErrorStatus(true);

    emit this->showMsg(msg->Message());
}

void Controller::IncreaseValue()
{
    ++_receiveCount;
    emit this->updateValue(_receiveCount);

    if(_receiveCount >= _maxCount)
    {
        _receiveCount = _maxCount = 0;
    }
}

void Controller::AddClient(QString ip, QString domain)
{
    if(_datamodel.contains(ip))
        return;

    std::shared_ptr<Client> client(new Client(ip, domain));
    _datamodel[ip] = client;
    _maxCount += g_vecServList.size();
    emit this->updateMax(_maxCount);
    emit updateList(_datamodel[ip]);
    emit run(ip);
}

void Controller::AddResult(QString ip, QPair<QString, QString> result)
{
    Client* pClient = _datamodel[ip].get();
    pClient->SetResult(result);
    emit dataChanged(ip);
}

void Controller::AddResult(QString ip, QMap<QString, QString> result, servertype<antispamServer> nouse)
{
    Client* pClient = _datamodel[ip].get();
    pClient->SetResult(result, nouse);
    emit dataChanged(ip);
}

void Controller::AddResult(QString ip, QMap<QString, QString> result, servertype<trmServer> nouse)
{
    Client* pClient = _datamodel[ip].get();
    pClient->SetResult(result, nouse);
    emit dataChanged(ip);
}

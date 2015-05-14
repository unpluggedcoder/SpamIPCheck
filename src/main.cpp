#include <QtGui/QGuiApplication>
#include <QtNetwork>
#include <QtNetwork/qnetworkreply.h>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>
#include "qtquick2applicationviewer.h"
#include "controller.h"
#include "resultmodel.h"
#include "log.h"

int main(int argc, char *argv[])
{
    const std::string currDir = QDir::currentPath().toStdString();
    Log::getInstance().Log_Init(currDir);
    QGuiApplication app(argc, argv);

    qRegisterMetaType<std::shared_ptr<MsgInfo> >("std::shared_ptr<MsgInfo>");
    qRegisterMetaType<QMap<QString,QString> >("QMap<QString,QString>");
    qRegisterMetaType<QPair<QString,QString> >("QPair<QString,QString>");
    qRegisterMetaType<servertype<antispamServer> >("servertype<antispamServer>");
    qRegisterMetaType<servertype<trmServer> >("servertype<trmServer>");
    qRegisterMetaType<std::shared_ptr<Client> >("std::shared_ptr<Client>");

    QtQuick2ApplicationViewer viewer;
    Controller ctrl;

    QQmlEngine *engine =  viewer.engine();
    QQmlContext *context = engine->rootContext();
    context->setContextProperty("Controller", &ctrl);

    ResultModel resultModel(&ctrl);
    QObject::connect(&ctrl, SIGNAL(updateList(std::shared_ptr<Client>)), &resultModel, SLOT(updateList(std::shared_ptr<Client>)));
    QObject::connect(&ctrl, SIGNAL(dataChanged(QString)), &resultModel, SLOT(updateData(QString)));
    context->setContextProperty("ResultModel", &resultModel);

    viewer.setMainQmlFile("qrc:///qml/SpamIPCheck/main.qml");
    QList<QQmlError> errors = viewer.errors();
    for(QQmlError error : errors)
    {
        std::string msg = error.toString().toStdString();
        LOG_ERROR(msg);
    }
    viewer.showExpanded();
    ctrl.Init();

    //return app.exec();
    int ret = app.exec();
    ctrl.Quit();
    return ret;
}


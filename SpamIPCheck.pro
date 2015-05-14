# Add more folders to ship with the application, here
folder_01.source = qml/SpamIPCheck
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/client.cpp \
    src/dnsblserver.cpp \
    src/antispamserver.cpp \
    src/trmServer.cpp \
    src/global.cpp \
    src/requestworker.cpp \
    src/replyworker.cpp \
    src/controller.cpp \
    src/parseworker.cpp \
    src/msginfo.cpp \
    src/resultmodel.cpp \
    src/log.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    src/client.h \
    src/dnsblserver.h \
    src/antispamserver.h \
    src/trmServer.h \
    src/global.h \
    src/requestworker.h \
    src/replyworker.h \
    src/controller.h \
    src/parseworker.h \
    src/msginfo.h \
    src/resultmodel.h \
    src/log.h

CONFIG += c++11

QT += network quick qml

# If build in *nix system, need set the boost path
#win32:INCLUDEPATH += C:\Boost.MinGW\include\boost-1_55\
#debug{
#DEFINES += DEBUG
#}

win32: {
LIBS += -LC:/OpenSSL-Win32/lib/VC
Debug: { LIBS += -llibeay32MTd -lssleay32MTd }
Release: { LIBS += -leay32MT -lssleay32MT }
}

unix: {
LIBS += -leay32 -lssleay32
}

RESOURCES += \
    resource.qrc

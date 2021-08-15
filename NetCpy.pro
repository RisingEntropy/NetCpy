QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    clientui.cpp \
    core.cpp \
    main.cpp \
    mainwindow.cpp \
    server.cpp \
    serverthread.cpp \
    serverui.cpp

HEADERS += \
    client.h \
    clientui.h \
    core.h \
    mainwindow.h \
    server.h \
    serverthread.h \
    serverui.h

FORMS += \
    clientui.ui \
    mainwindow.ui \
    serverui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

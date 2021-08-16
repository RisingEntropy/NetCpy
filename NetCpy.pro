QT       += core gui
QT       += network
TRANSLATIONS = zh_CN.ts
RC_ICONS = icon.ico
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
    serverui.cpp \
    toolkit.cpp \
    uploadprogress.cpp

HEADERS += \
    client.h \
    clientui.h \
    core.h \
    mainwindow.h \
    server.h \
    serverthread.h \
    serverui.h \
    toolkit.h \
    uploadprogress.h

FORMS += \
    clientui.ui \
    mainwindow.ui \
    serverui.ui \
    toolkit.ui \
    uploadprogress.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lxlnt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lxlntd

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

RESOURCES += \
    translation.qrc

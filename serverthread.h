#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include<QThread>
#include <QMutex>
#include <QTcpSocket>
#include <QDir>
#include "core.h"
class ServerThread : public QThread {
    Q_OBJECT
private:
    qintptr socketDecriptor;
    int waitDuration = -1;
    QTcpSocket *socket;
    QString basePath;
    QMutex mutex;
    QDir dir;//not used now
    QObject *parent;
    qintptr desc;
    template<typename T> void read(T * dst,int size);
    template<typename T> void write(T * src,int size);
    template<typename T> void writeFlush(T *src,int size);
    bool checkLogInfo(QString name,unsigned int pwshash);
    bool checkVersion(int version_code);
    void transFile(head_file_info* info);
    void execCommand(command_info * info);
public:
    explicit ServerThread(QString base,qintptr desc = 0,QObject *parent = nullptr);
    void run();
    void setWaitDuration(int v);
signals:
//    void progressUpdate(QString fileName,int progress);
    void loginSuccessful(QString userName);
};

#endif // SERVERTHREAD_H

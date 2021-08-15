#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QThread>
#include "core.h"
class Client : public QObject {
    Q_OBJECT
private:
    int waitReadyDuration = 30000;
    QObject *parent;
    QTcpSocket *socket;
    QString host;
    QString userName;
    int pwdHash;
    int port;
    int needPassword;
    QVector<QString> files;
    QVector<command_info> cmds;
    template<typename T>void read(T * dst,int size);
    template<typename T>void write(T * src,int size);
    template<typename T>void writeFlush(T *src,int size);
    void transFile(QString path);
    void transCmd(int pos);
private slots:
    void connected();
public:
    Client(QString host,int port,QString userName,unsigned int pwdHash,QObject *parent = nullptr);
    Client(QString host,int port,QString userName,QObject *parent = nullptr);
    ~Client();
    void setWaitReadyDuration(int v);
    void startTrans();
    void addFile(QString path);
    void addCommand(command_info info);
    void clear();
    void finish();
signals:
    void startTransferring();
    void connectSuccessful();
    void transferringFinish();
    void serverBusy();
    void versionInsuitable();
    void passwordWrong();
    void loginSuccessful();
    void transferProgressUpdate(int fileTransferred,int totFiles);
    void fileTransferProgressUpdate(QString fileName,int percent);
};

#endif // CLIENT_H

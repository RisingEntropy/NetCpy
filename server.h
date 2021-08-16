#ifndef SERVER_H
#define SERVER_H

#include<QObject>
#include <QTcpServer>
#include <QThreadPool>
class Server : public QTcpServer {
    Q_OBJECT
private:
    int maxThread = 10;
    QString basePath;
    QObject * parent;
    QThreadPool threads;
    bool needPass;
public slots:
    void receive(QString msg);
public:
    explicit Server(QString base,bool needpass,QObject *parent = nullptr);
    void setMaxThread(int v);
    int getMaxThread();
    int currentActiveThread();
protected:
    void incomingConnection(qintptr socketDescriptor)override;
signals:
    void newConn();
    void connHandled();
    void resend(QString);
    void serverConnectionError(QString);
};

#endif // SERVER_H

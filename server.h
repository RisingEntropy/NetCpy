#ifndef SERVER_H
#define SERVER_H

#include<QObject>
#include <QTcpServer>
class Server : public QTcpServer {
    Q_OBJECT
private:
    int activeThread = 0;
    int maxThread = 10;
    QString basePath;
    QObject * parent;
private slots:
    void clientDisconnect();
public:
    explicit Server(QString base,QObject *parent = nullptr);
    void setMaxThread(int v);
    int getMaxThread();
    int currentActiveThread();
protected:
    void incomingConnection(qintptr socketDescriptor)override;
signals:
    void serverClosed();
    void serverStarted();
    void serverConnectionError(QString msg);
};

#endif // SERVER_H

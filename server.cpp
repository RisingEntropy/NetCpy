#include "server.h"
#include <QTcpSocket>
#include "core.h"
#include "serverthread.h"
Server::Server(QString base,QObject *parent) : QTcpServer(parent),basePath(base) {
}

void Server::setMaxThread(int v) {
    this->maxThread = v;
}
void Server::clientDisconnect() {
    this->activeThread--;
}
int Server::getMaxThread() {
    return this->maxThread;
}
int Server::currentActiveThread() {
    return this->activeThread;
}
void Server::incomingConnection(qintptr socketDescriptor) {
    emit serverStarted();
    qDebug()<<"New Connection";
    if(this->activeThread+1>this->maxThread) {
        QTcpSocket socket;
        if(!socket.setSocketDescriptor(socketDescriptor)) {
            emit serverConnectionError("get socket error when declining a client");
            return;
        }
        head_return_code * ret = make_return_code(busy_refuse,0);
        socket.write((char*)ret,sizeof(head_return_code));
        socket.flush();
        return;
    } else {
        qDebug()<<socketDescriptor;
        this->activeThread++;
        ServerThread * thread = new ServerThread(basePath,socketDescriptor,0);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(thread,&ServerThread::finished,this,&Server::clientDisconnect);
        qDebug()<<"aha";
        thread->start();
    }
    emit serverClosed();
}

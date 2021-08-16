#include "server.h"
#include <QThread>
#include <QTcpSocket>
#include "core.h"
#include "serverthread.h"
Server::Server(QString base,bool needpass,QObject *parent) : QTcpServer(parent),basePath(base) {
    this->threads.setMaxThreadCount(this->maxThread);
    this->needPass = needpass;
}

void Server::setMaxThread(int v) {
    this->maxThread = v;
    this->threads.setMaxThreadCount(v);
}
int Server::getMaxThread() {
    return this->maxThread;
}
int Server::currentActiveThread() {
    return this->threads.activeThreadCount();
}
void Server::incomingConnection(qintptr socketDescriptor) {
    emit newConn();
    if(this->threads.activeThreadCount()+1>this->maxThread) {
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
        ServerThread * thread = new ServerThread(basePath,socketDescriptor,needPass,0);
        connect(thread,&ServerThread::toLog,this,&Server::receive);
        thread->setAutoDelete(true);
        this->threads.start(thread);
    }
    emit connHandled();
}
void Server::receive(QString msg) {
    emit resend(msg);
}

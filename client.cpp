#include "client.h"
#include "core.h"
#include <QFile>
#include <QTimer>
#include <QHostAddress>
Client::Client(QString host,int port,QString userName,unsigned int pwdHash,QObject *parent):QThread(parent) {
    this->parent = parent;
    this->host = host;
    this->port = port;
    this->userName = userName;
    this->pwdHash = pwdHash;
    this->needPassword = 1;
}
Client::Client(QString host,int port,QString userName,QObject *parent):QThread(parent) {
    this->parent = parent;
    this->host = host;
    this->port = port;
    this->userName = userName;
    this->needPassword = 0;
}
Client::~Client() {
    finish();
}
template <typename T>
void Client::read(T * dst,int size) {
    while(socket->bytesAvailable()<size)socket->waitForReadyRead(10);
    socket->read((char*)dst,size);
}
template<typename T>
void Client::write(T * src,int size) {
    socket->write((char*)src,size);
}
template<typename T>
void Client::writeFlush(T * src,int size) {
    socket->write((char*)src,size);
    socket->flush();
}
void Client::setWaitReadyDuration(int v) {
    this->waitReadyDuration = v;
}
void Client::addFile(QString path) {
    this->files.push_back(path);
}
void Client::addCommand(command_info info) {
    this->cmds.push_back(info);
}
void Client::clear() {
    this->files.clear();
    this->cmds.clear();
}
void Client::startTrans() {
    this->socket = new QTcpSocket(parent);
    connect(this->socket,&QTcpSocket::readyRead,this,&Client::connected);
    this->socket->connectToHost(QHostAddress(this->host),port);
}
void Client::run() {
    this->socket = new QTcpSocket(parent);
    this->socket->connectToHost(this->host,port);
    this->socket->waitForConnected();
    if(!socket->isValid()) {
        emit noServerFound();
        emit exceptionOccur();
        return;
    }
    connected();
}
bool Client::checkConnection() {
    return true;
}
void Client::connected() {
    emit startTransferring();
    head_return_code * ret = new head_return_code;
    read(ret,sizeof(head_return_code));
    if(ret->state==busy_refuse) {
        emit serverBusy();
        emit exceptionOccur();
        delete ret;
        return;
    }
    emit connectSuccessful();
    head_apply * apply;
    if(this->needPassword==1)apply = make_apply_pass(this->userName,this->pwdHash);
    else apply = make_apply_nopass(this->userName);
    writeFlush(apply,sizeof(head_apply));
    ret = new head_return_code;
    read(ret,sizeof(head_return_code));
    if(ret->state==version_unsuitable) {
        delete ret;
        delete apply;
        emit versionInsuitable();
        emit exceptionOccur();
        return;
    }
    if(ret -> state==password_wrong) {
        delete ret;
        delete apply;
        emit passwordWrong();
        emit exceptionOccur();
        return;
    }
    emit loginSuccessful();
    head_transfering_info *transinfo = make_trans_info(this->files.size()+this->cmds.size());
    writeFlush(transinfo,sizeof(head_transfering_info));
    for(int i = 0; i<this->cmds.size(); i++) {
        transCmd(i);
    }
    for(int i = 0; i<this->files.size(); i++) {
        if(i==files.size()-1)emit transferProgressUpdate(100);
        else emit transferProgressUpdate((int)(1.0*i/files.size()*100));
        transFile(files[i]);
    }
    ret = new head_return_code;
    read(ret,sizeof(head_return_code));
    emit transferringFinish();
    socket->close();
    delete transinfo;
    delete ret;

}
void Client::transCmd(int pos) {

}
void Client::transFile(QString path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    head_file_info * info = make_file_info(path);
    writeFlush(info,sizeof (head_file_info));
    head_datablock *data = new head_datablock;
    memset((char*)data,0,sizeof(head_datablock));
    unsigned long long tot_size = file.size();
    unsigned long long transed_size = 0;
    unsigned long long step = tot_size/100;
    unsigned long long stage = step;
    int percent = 0;
    while(transed_size+512<tot_size) {
        file.read(data->data,512);
        data->eof = 0;
        transed_size+=512;
        write(data,sizeof(head_datablock));
        if(transed_size>stage) {
            percent++;
            stage+=step;
            emit fileTransferProgressUpdate(file.fileName(),percent);
        }
        if(socket->bytesToWrite()>1024*128)socket->flush();//buffer size is no bigger than 512KB
    }
    file.read(data->data,tot_size-transed_size);
    data->eof = 1;
    data->valid_length = tot_size-transed_size;
    writeFlush(data,sizeof(head_datablock));
    socket->waitForBytesWritten();
    emit fileTransferProgressUpdate(file.fileName(),100);
    delete info;
    delete data;
    file.close();
}
void Client::


finish() {
    if(this->socket!=nullptr)delete this->socket;
    clear();
}

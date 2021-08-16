﻿#include "serverthread.h"
#include "core.h"
#include <QFile>
ServerThread::ServerThread(QString base,qintptr desc,bool needpass,QObject *prent) {
    this->desc = desc;
    this->basePath = base;
    this->parent = prent;
    this->needPass = needpass;
}
template<typename T>
void ServerThread::read(T *dst, int size) {
    if(socket->state()==QAbstractSocket::UnconnectedState||socket->state()==QAbstractSocket::ClosingState) {
        terminate();
    }
    while(socket->bytesAvailable()<size)socket->waitForReadyRead(10);
    socket->read((char*)dst,size);
}
template<typename T>
void ServerThread::write(T *src, int size) {
    socket->write((char*)src,size);
}
template<typename T>
void ServerThread::writeFlush(T *src, int size) {
    socket->write((char*)src,size);
    socket->flush();
}
bool ServerThread::checkLogInfo(QString name, unsigned int pwshash) {
    if(this->needPass)return checkLogin(name,pwshash);
    else return true;
}
bool ServerThread::checkVersion(int version_code) {
    return true;
}
void ServerThread::transFile(head_file_info *info) {
    QFile file(basePath+"/"+info->file_name);
    file.open(QIODevice::WriteOnly);
    head_datablock *data = new head_datablock;
    bool eof = false;
    while(!eof) {
        read(data,sizeof(head_datablock));
        if(data->eof==1) {
            eof = true;
            qDebug()<<"eof";
            file.write(data->data,data->valid_length);
            file.flush();

            break;
        } else {
            file.write(data->data,512);
        }
        if(file.bytesToWrite()>1024*512)file.flush();
    }
    file.flush();
    emit toLog(QString("A file transferrd:")+info->file_name);
    delete data;
    file.close();
    qDebug()<<info->file_name;
}
void ServerThread::execCommand(command_info *info) {

}
void ServerThread::run() {
    this->socket = new QTcpSocket;
    socket->setSocketDescriptor(this->desc);
    head_return_code *response = make_return_code(connected,0);
    writeFlush(response,sizeof (head_return_code));
    delete response;
    head_apply * apply = new head_apply;
    if(socket->state()==QAbstractSocket::UnconnectedState||socket->state()==QAbstractSocket::ClosingState) {
        delete socket;
        return;
    }
    read(apply,sizeof(head_apply));
    if(!checkVersion(apply->version)) {
        response = make_return_code(version_unsuitable,0);
        writeFlush(response,sizeof(head_apply));
        delete response;
        return;
    }
    if(apply->need_password) {
        if(!checkLogInfo(apply->user_name,apply->password_hash)) {
            response = make_return_code(password_wrong,0);
            writeFlush(response,sizeof(head_return_code));
            delete response;
            return;
        }
    }
    emit toLog(QString("A user login:")+apply->user_name);
    this->basePath+='/';
    this->basePath+=apply->user_name;
    QDir dir(basePath);
    if(!dir.exists())dir.mkdir(basePath);
    response = make_return_code(login_successful,0);
    writeFlush(response,sizeof(head_return_code));
    delete response;
    head_transfering_info * info = new head_transfering_info;
    head_file_info * opeInfo = new head_file_info;
    if(socket->state()==QAbstractSocket::UnconnectedState||socket->state()==QAbstractSocket::ClosingState) {
        delete opeInfo;
        delete info;
        delete socket;
        return;
    }
    read(info,sizeof(head_transfering_info));
    for(int i = 1; i<=info->operation_count; i++) {
        if(socket->state()==QAbstractSocket::UnconnectedState||socket->state()==QAbstractSocket::ClosingState) {
            delete opeInfo;
            delete info;
            delete socket;
            return;
        }
        read(opeInfo,sizeof(head_file_info));
        if(opeInfo->type==type_head_file_info) {
            transFile(opeInfo);
        } else {
            execCommand((command_info*)opeInfo);
        }
    }
    delete opeInfo;
    delete info;
    delete socket;
}

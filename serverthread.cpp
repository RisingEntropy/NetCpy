#include "serverthread.h"
#include "core.h"
#include <QFile>
ServerThread::ServerThread(QString base,qintptr desc,QObject *prent):QThread(prent) {
    qDebug()<<"ad";
    this->desc = desc;
    this->basePath = base;
    this->parent = prent;
}
template<typename T>
void ServerThread::read(T *dst, int size) {
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
    return true;
}
bool ServerThread::checkVersion(int version_code) {
    return true;
}
void ServerThread::transFile(head_file_info *info) {
    QFile file(basePath+"/"+info->file_name);
    file.open(QIODevice::WriteOnly);
    head_datablock *data = new head_datablock;
    bool eof = false;
    qDebug()<<"asdsdc";
    while(!eof) {
        read(data,sizeof(head_datablock));
        if(data->eof==1) {
            eof = true;
            file.write(data->data,data->valid_length);
        } else {
            file.write(data->data,512);
        }
    }
    delete data;
    file.close();
}
void ServerThread::execCommand(command_info *info) {

}
void ServerThread::run() {
    this->socket = new QTcpSocket;
    socket->setSocketDescriptor(this->desc);
    head_return_code *response = make_return_code(connected,0);
    qDebug()<<"1";
    writeFlush(response,sizeof (head_return_code));
    qDebug()<<"2";
    delete response;
    head_apply * apply = new head_apply;
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
    emit loginSuccessful(apply->user_name);
    this->basePath+='/';
    this->basePath+=apply->user_name;
    qDebug()<<this->basePath;
    QDir dir(basePath);
    if(!dir.exists())dir.mkdir(basePath);
    response = make_return_code(login_successful,0);
    writeFlush(response,sizeof(head_return_code));
    delete response;
    head_transfering_info * info = new head_transfering_info;
    head_file_info * opeInfo = new head_file_info;
    read(info,sizeof(head_transfering_info));
    for(int i = 1; i<=info->operation_count; i++) {
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

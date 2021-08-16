#include "uploadprogress.h"
#include "ui_uploadprogress.h"
#include <QMessageBox>
UploadProgress::UploadProgress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UploadProgress) {
    ui->setupUi(this);
}

UploadProgress::~UploadProgress() {
    delete ui;
}
void UploadProgress::updateFileProgress(QString name,int v) {
    ui->label_2->setText(tr("Current Transferring:")+name);
    ui->progressBar->setValue(v);
}
void UploadProgress::updateTotalProfress(int v) {
    ui->progressBar_2->setValue(v);
}
void UploadProgress::finished() {
    close();
}
void UploadProgress::noServer() {
    QMessageBox::warning(this,tr("error"),tr("No server founded"));
    close();
}
void UploadProgress::error(QAbstractSocket::SocketError msg) {
    if(msg==QAbstractSocket::HostNotFoundError)
        QMessageBox::warning(this,tr("error"),tr("Host not found"));
    close();

}
void UploadProgress::serverBusy() {
    QMessageBox::information(this,tr("server busy"),tr("Server busy,try later"));
    close();
}
void UploadProgress::passwordWrong() {
    QMessageBox::warning(this,tr("error"),tr("Password wrong!"));
    close();
}
void UploadProgress::versionError() {
    QMessageBox::warning(this,tr("error"),tr("client version is low,please update"));
}


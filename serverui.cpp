#include "serverui.h"
#include "ui_serverui.h"
#include <QFileDialog>
#include <QMessageBox>
#include "core.h"
ServerUI::ServerUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerUI) {
    ui->setupUi(this);
    ui->label_3->setText(QDir::homePath());
    this->basePath = QDir::homePath();
}

ServerUI::~ServerUI() {
    delete ui;
}

void ServerUI::on_pushButton_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this,"Choose working directory",QDir::homePath());
    ui->label_3->setText(dir);
    this->basePath = dir;
}


void ServerUI::on_pushButton_2_clicked() {
    ui->textEdit->append(tr("Server started"));
    if(ui->radioButton->isChecked()) {
        if(this->databse.isEmpty()) {
            QMessageBox::warning(this,tr("warning"),tr("data base is not chosen,no user can login!!!!!"));
        } else {
            loadDatabase(databse);
        }
        this->server = new Server(basePath,true,this);

    } else {
        this->server = new Server(basePath,false,this);
    }
    connect(server,&Server::newConn,this,&ServerUI::newConn);
    connect(server,&Server::connHandled,this,&ServerUI::connHandled);
    connect(server,&Server::serverConnectionError,this,&ServerUI::serverError);
    connect(server,&Server::resend,this,&ServerUI::toLog);
    ui->pushButton_3->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
    ui->label_5->setText(tr("status:Running"));
    server->setMaxThread(ui->spinBox_2->value());
    server->listen(QHostAddress::LocalHost,ui->spinBox->value());
}


void ServerUI::on_pushButton_3_clicked() {
    if(this->server->currentActiveThread()!=0) {
        QMessageBox::warning(this,tr("Error"),tr("Client connected!Unable to close server,please wait client(s) to finish"));
        return;
    }
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(true);
    this->server->close();
    ui->label->setText(tr("Status:Closed"));
    delete this->server;
    ui->textEdit->append(tr("Server closed "));
}
void ServerUI::newConn() {
    ui->textEdit->append(tr("new connection incoming\n"));
}
void ServerUI::connHandled() {
    ui->textEdit->append(tr("new connection handled\n"));
}

void ServerUI::serverError(QString msg) {
    ui->textEdit->append(tr("Error:")+msg+"\n");
}
void ServerUI::toLog(QString msg) {
    ui->textEdit->append(msg);
}


void ServerUI::on_radioButton_toggled(bool checked) {
    if(checked)ui->pushButton_4->setDisabled(false);
    else ui->pushButton_4->setDisabled(true);
}


void ServerUI::on_pushButton_4_clicked() {
    this->databse = QFileDialog::getOpenFileName(this,tr("Choose database"),QDir::homePath(),tr("excel file(*.xlsx)"));
}


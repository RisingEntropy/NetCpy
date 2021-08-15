#include "serverui.h"
#include "ui_serverui.h"
#include <QFileDialog>
#include <QMessageBox>
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
    this->server = new Server(basePath,0);
    connect(server,&Server::serverStarted,this,&ServerUI::serverStart);
    connect(server,&Server::serverClosed,this,&ServerUI::serverClose);
    connect(server,&Server::serverConnectionError,this,&ServerUI::serverError);
    ui->pushButton_3->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
    ui->label_5->setText(tr("status:Running"));
    this->server = new Server(this->basePath);
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
}
void ServerUI::serverStart() {
    ui->textEdit->append(tr("Server Start\n"));
}
void ServerUI::serverClose() {
    ui->textEdit->append(tr("Server Close\n"));
}

void ServerUI::serverError(QString msg) {
    ui->textEdit->append(tr("Error:")+msg+"\n");
}

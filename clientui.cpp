#include "clientui.h"
#include "ui_clientui.h"
#include <QFileDialog>
#include <QMessageBox>
ClientUI::ClientUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientUI) {
    ui->setupUi(this);
}

ClientUI::~ClientUI() {
    delete ui;
}

void ClientUI::on_radioButton_clicked() {
    if(ui->radioButton->isChecked()) {
        ui->lineEdit_4->setDisabled(false);
    } else {
        ui->lineEdit_4->setDisabled(true);
    }
}


void ClientUI::on_pushButton_2_clicked() {
    QStringList list = QFileDialog::getOpenFileNames(this,tr("choose Files"),QDir::homePath());
    this->files.append(list);
    for(auto x:list)
        ui->listWidget->addItem(x);
}

void ClientUI::done() {
    this->files.clear();
    ui->listWidget->clear();
    QMessageBox::information(this,tr("Finish"),tr("Finished!"));
    this->client->deleteLater();
    ui->pushButton->setText(tr("Transfer"));
    ui->pushButton->setDisabled(false);
}

void ClientUI::on_pushButton_clicked() {
    if(ui->lineEdit->text().isEmpty()) {
        QMessageBox::information(this,tr("Input host"),tr("Please Input host"));
        return;
    }
    if(ui->lineEdit_3->text().isEmpty()) {
        QMessageBox::information(this,tr("Input user"),tr("Please Input user"));
        return;
    }
    if(ui->radioButton->isChecked()) {
        if(ui->lineEdit_4->text().isEmpty()) {
            QMessageBox::information(this,tr("Input password"),tr("Please Input password"));
            return;
        }
        this->client = new Client(ui->lineEdit->text(),ui->spinBox->value(),ui->lineEdit_3->text(),hash(ui->lineEdit_4->text().toLatin1().data()));
    } else {
        this->client = new Client(ui->lineEdit->text(),ui->spinBox->value(),ui->lineEdit_3->text());
    }
    for(auto x:files) {
        client->addFile(x);
    }
    qDebug()<<"Client Trans";
    connect(client,&Client::transferringFinish,this,&ClientUI::done);
    client->startTrans();
    ui->pushButton->setText(tr("Transferring"));
    ui->pushButton->setDisabled(true);
}


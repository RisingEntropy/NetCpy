#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverui.h"
#include "clientui.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_server_clicked() {
    ServerUI * ser = new ServerUI;
    ser->show();
    close();
}



void MainWindow::on_pushButton_client_clicked() {
    ClientUI *cli = new ClientUI;
    cli->show();
    close();
}


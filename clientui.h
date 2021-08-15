﻿#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <QWidget>
#include "client.h"
namespace Ui {
class ClientUI;
}

class ClientUI : public QWidget {
    Q_OBJECT

public:
    explicit ClientUI(QWidget *parent = nullptr);
    ~ClientUI();

private slots:
    void on_radioButton_clicked();

    void on_pushButton_2_clicked();

    void done();
    void on_pushButton_clicked();

private:
    Ui::ClientUI *ui;
    QStringList files;
    Client * client;
};

#endif // CLIENTUI_H

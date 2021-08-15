#ifndef SERVERUI_H
#define SERVERUI_H

#include <QWidget>
#include "server.h"
namespace Ui {
class ServerUI;
}

class ServerUI : public QWidget {
    Q_OBJECT

public:
    explicit ServerUI(QWidget *parent = nullptr);
    ~ServerUI();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void serverStart();

    void serverClose();

    void serverError(QString msg);
private:
    Ui::ServerUI *ui;
    Server *server;
    QString basePath;
};

#endif // SERVERUI_H

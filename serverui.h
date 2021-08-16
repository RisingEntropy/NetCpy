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

public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void newConn();

    void connHandled();

    void serverError(QString msg);

    void toLog(QString msg);
private slots:
    void on_radioButton_toggled(bool checked);

    void on_pushButton_4_clicked();

private:
    Ui::ServerUI *ui;
    Server *server;
    QString basePath;
    QString databse;
};

#endif // SERVERUI_H

#ifndef UPLOADPROGRESS_H
#define UPLOADPROGRESS_H

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
class UploadProgress;
}

class UploadProgress : public QWidget {
    Q_OBJECT

public:
    explicit UploadProgress(QWidget *parent = nullptr);
    ~UploadProgress();

private:
    Ui::UploadProgress *ui;
public slots:
    void updateFileProgress(QString name,int v);
    void updateTotalProfress(int v);
    void finished();
    void noServer();
    void error(QAbstractSocket::SocketError msg);
    void serverBusy();
    void passwordWrong();
    void versionError();
};

#endif // UPLOADPROGRESS_H

#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QDialog>

namespace Ui {
class ToolKit;
}

class ToolKit : public QDialog {
    Q_OBJECT

public:
    explicit ToolKit(QWidget *parent = nullptr);
    ~ToolKit();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::ToolKit *ui;
};

#endif // TOOLKIT_H

#include "toolkit.h"
#include "ui_toolkit.h"
#include "core.h"
ToolKit::ToolKit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToolKit) {
    ui->setupUi(this);
}

ToolKit::~ToolKit() {
    delete ui;
}

void ToolKit::on_lineEdit_textChanged(const QString &arg1) {
    ui->lineEdit_2->setText(QString::number(hash(arg1.toLatin1().data())));
}


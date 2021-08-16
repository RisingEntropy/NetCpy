#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    if(translator.load(":/translation/zh_CN.qm")) {
        a.installTranslator(&translator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}

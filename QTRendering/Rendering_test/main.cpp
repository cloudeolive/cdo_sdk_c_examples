#include <QtGui/QApplication>
#include "mainwindow.h"
#include "cloudeoctrl.h"

CloudeoCtrl* gCtrl;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setVersion("undefined");
    w.show();
    gCtrl = new CloudeoCtrl(&w);
    return a.exec();
}

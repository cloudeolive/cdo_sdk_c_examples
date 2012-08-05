#include <QtGui/QApplication>
#include "cdosampleappwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CdoSampleAppWindow w;
    w.show();
    
    return a.exec();
}

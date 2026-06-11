//#include "mainwindow.h"
//#include "loginwindows.h"
//#include "homechats.h"
#include "aftwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    aftwindow w;
    w.show();
    return QApplication::exec();
}

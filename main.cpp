//#include "mainwindow.h"
#include "loginwindows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindows w;
    w.show();
    return QApplication::exec();
}

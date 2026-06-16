#include "aftwindow.h"
#include "connect.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connect::instance()->findServer();
    aftwindow w;
    w.show();
    return QApplication::exec();
}
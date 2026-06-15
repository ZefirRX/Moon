#include <QCoreApplication>
#include "server.h"
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server s;
    qDebug() << QSqlDatabase::drivers();
    return QCoreApplication::exec();
}

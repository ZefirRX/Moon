#pragma once
#include <QSqlDatabase>
#include <QString>

class Database
{
public:
    Database();
    bool open();
    bool registerUser(const QString& nickname, const QString& tag,const QString& password);
    bool checkoutLogin(const QString& tag, const QString& password, QString& nickname);
    QString hash;
    QStringList getAllNicknames();
private:
    QSqlDatabase db;
};
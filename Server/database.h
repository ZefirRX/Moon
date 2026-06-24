#pragma once
#include <QSqlDatabase>
#include <QString>
#include <QList>

class Database
{
public:
    Database();
    bool open();
    bool registerUser(const QString& nickname, const QString& tag,const QString& password);
    bool checkoutLogin(const QString& tag, const QString& password, QString& nickname);
    QString hash;
    bool saveMessage(const QString& sender, const QString& receiver, const QString& text, const QString& time);
    QList<QStringList> getHistory(const QString& user1, const QString& user2);
    QStringList getAllNicknames();
private:
    QSqlDatabase db;
};
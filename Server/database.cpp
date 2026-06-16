#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database()
{
}

bool Database::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if(!db.open())
    {
        qDebug() << "DB Error:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;

    query.exec( "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nickname TEXT UNIQUE,"
        "tag TEXT UNIQUE,"
        "password TEXT)");
    return true;
}

bool Database::registerUser(const QString& nickname, const QString& tag, const QString& password)
{
    QSqlQuery check;

    check.prepare("SELECT COUNT(*) FROM users "
        "WHERE nickname = ? OR tag = ?");
    check.addBindValue(nickname);
    check.addBindValue(tag);

    if(!check.exec())
        return false;
    check.next();
    if(check.value(0).toInt() > 0)
        return false;
    QSqlQuery insert;

    insert.prepare("INSERT INTO users(nickname, tag, password) "
        "VALUES(?,?,?)");
    insert.addBindValue(nickname);
    insert.addBindValue(tag);
    insert.addBindValue(password);
    return insert.exec();
}

bool Database::checkoutLogin(const QString& tag, const QString& password, QString& nickname)
{
    QSqlQuery query;
    query.prepare("SELECT nickname FROM users " "WHERE tag = ? AND password = ?");
    query.addBindValue(tag);
    query.addBindValue(password);
    if(!query.exec())
        return false;
    if(query.next())
    {
        nickname = query.value(0).toString();
        return true;
    }
    return false;
}

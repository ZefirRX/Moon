#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QStringList>

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

    query.exec("CREATE TABLE IF NOT EXISTS messages ("
               "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
               "sender   TEXT,"
               "receiver TEXT,"
               "text     TEXT,"
               "time     TEXT)");
    return true;
}

bool Database::saveMessage(const QString& sender, const QString& receiver, const QString& text, const QString& time)
{
    QSqlQuery query;
    query.prepare("INSERT INTO messages(sender, receiver, text, time) "
                  "VALUES(?,?,?,?)");
    query.addBindValue(sender);
    query.addBindValue(receiver);
    query.addBindValue(text);
    query.addBindValue(time);
    return query.exec();
}

QList<QStringList> Database::getHistory(const QString& user1, const QString& user2)
{
    QList<QStringList> result;
    QSqlQuery query;

    if(user2.isEmpty())
    {
        query.prepare("SELECT sender, text, time FROM messages "
                      "WHERE receiver = '' "
                      "ORDER BY id ASC");
    }
    else
    {
        query.prepare("SELECT sender, text, time FROM messages "
                      "WHERE (sender = ? AND receiver = ?) "
                      "OR (sender = ? AND receiver = ?) "
                      "ORDER BY id ASC");
        query.addBindValue(user1);
        query.addBindValue(user2);
        query.addBindValue(user2);
        query.addBindValue(user1);
    }

    if(!query.exec())
        return result;

    while(query.next())
    {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        result << row;
    }
    return result;
}

bool Database::registerUser(const QString& nickname, const QString& tag, const QString& password)
{
    QSqlQuery check;

    check.prepare("SELECT COUNT(*) FROM users "
        "WHERE tag = ?");
    check.addBindValue(tag);

    if(!check.exec())
        return false;
    check.next();
    if(check.value(0).toInt() > 0)
        return false;
    QSqlQuery insert;

    insert.prepare("INSERT INTO users(nickname, tag, password) "
        "VALUES(?,?,?)");
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    // Получаем шестнадцатеричную строку=
    QString BinaritiInSix = hash.toHex();
    insert.addBindValue(nickname);
    insert.addBindValue(tag);
    insert.addBindValue(BinaritiInSix);
    return insert.exec();
}

bool Database::checkoutLogin(const QString& tag, const QString& password, QString& nickname)
{
    QSqlQuery query;
    query.prepare("SELECT nickname FROM users " "WHERE tag = ? AND password = ?");
    query.addBindValue(tag);
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString BinaritiInSix = hash.toHex();
    query.addBindValue(BinaritiInSix);
    if(!query.exec())
        return false;
    if(query.next())
    {
        nickname = query.value(0).toString();
        return true;
    }
    return false;
}

QStringList Database::getAllNicknames()
{
    QStringList result;
    QSqlQuery query("SELECT nickname FROM users");
    while(query.next())
    {
        result << query.value(0).toString();
    }
    return result;
}
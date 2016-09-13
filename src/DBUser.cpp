#include "DBUser.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

#include "ConnectionPool.h"
#include "Common.h"

DBUser::DBUser()
{
}

DBUser::~DBUser()
{
}

int DBUser::checkUserAndPassword(const QString &user, const QString &password)
{
    int type = -1;
    if(user.isEmpty() || password.isEmpty())
    {
        return type;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return type;
    }

    //查询//
    QSqlQuery query(db);
    query.prepare("SELECT type FROM user WHERE name=:name and password=:password");
    query.bindValue(":name", user.trimmed());
    query.bindValue(":password", password);
    if(query.exec())
    {
        if(query.next())
        {
            type = query.value(0).toInt();
        }
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    //错误
    return type;
}

bool DBUser::modifyPassword(const QString &user, const QString &password)
{
    if(user.isEmpty() || password.isEmpty())
    {
        return false;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    //修改密码//
    QSqlQuery query(db);
    query.prepare("UPDATE user SET password=:password WHERE name=:name");
    query.bindValue(":password", password);
    query.bindValue(":name", user.trimmed());
    bool result = query.exec();
    if(!result)
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return result;
}

bool DBUser::modifyUserType(const QString &user, int type)
{
    if(user.isEmpty())
    {
        return false;
    }

    if(type != Administrator && type != OrdinaryUser)
    {
        return false;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    //修改用户类型//
    QSqlQuery query(db);
    query.prepare("UPDATE user SET type=:type WHERE name=:name");
    query.bindValue(":type", type);
    query.bindValue(":name", user.trimmed());
    bool result = query.exec();
    if(!result)
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return result;
}

bool DBUser::deleteUser(const QString &user)
{
    if(user.isEmpty())
    {
        return false;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    //删除//
    QSqlQuery query(db);
    query.prepare("DELETE FROM user WHERE name=:name");
    query.bindValue(":name", user.trimmed());
    bool result = query.exec();
    if(!result)
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return result;
}

bool DBUser::addUser(const QString &user, const QString &password, int type)
{
    if(user.isEmpty() || password.isEmpty())
    {
        return false;
    }

    if(type != Administrator && type != OrdinaryUser)
    {
        return false;
    }

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    //增加用户//
    QSqlQuery query(db);
    query.prepare("INSERT INTO user (name, password, type) "
                  "VALUES (:name, :password, :type)");
    query.bindValue(":name", user.trimmed());
    query.bindValue(":password", password);
    query.bindValue(":type", type);
    bool result = query.exec();
    if(!result)
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return result;
}

bool DBUser::getUserInfo(QVector<User> &vecUser)
{
    vecUser.clear();

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    //查询//
    QSqlQuery query("SELECT name,password,type FROM user", db);

    User user;
    while(query.next())
    {
        user.user = query.value(0).toString();
        user.password = query.value(1).toString();
        user.type = query.value(2).toInt() == 0 ? Administrator : OrdinaryUser;
        vecUser.push_back(user);
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return true;
}

bool DBUser::createConnection(QSqlDatabase &db)
{
    // 从数据库连接池里取得连接
    db = ConnectionPool::openConnection();
    //qDebug() << __FILE__ << __LINE__ << QString("connection: %2").arg(db.connectionName());

    if(!db.isValid())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                 qApp->tr("Unable to establish a database connection.\n"
                                       "This example needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}

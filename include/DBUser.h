#pragma once

#include <QString>

#include "Common.h"

class QSqlDatabase;

class DBUser
{
public:
    DBUser();
    ~DBUser();

    //检测用户名和密码,
    //若检测通过,返回用户类型(0:管理员 1:普通用户);若检测不通过,返回-1
    int checkUserAndPassword(const QString &user, const QString &password);

    //修改密码
    bool modifyPassword(const QString &user, const QString &password);

    //修改用户类型
    bool modifyUserType(const QString &user, int type);

    //删除用户
    bool deleteUser(const QString &user);

    //增加用户
    bool addUser(const QString &user, const QString &password, int type = OrdinaryUser);

    //获取用户信息
    bool getUserInfo(QVector<struct User> &vecUser);

private:
    bool createConnection(QSqlDatabase &db);
};

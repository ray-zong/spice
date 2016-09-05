#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QMap>

class QLabel;
class QLineEdit;
class QComboBox;

class AddUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddUserDialog(QWidget *parent = 0);
    ~AddUserDialog();

    //重置//
    void reset();
    //获取用户名//
    QString getUserName();
    //获取密码//
    QString getPassword();
    //获取用户类型//
    int getUserType();

private:
    void initUI();
    //检测用户名是否符合要求//
    bool checkUserName();
    //检测输入密码//
    bool checkPassword();

private slots:
    //添加//
    void addUserName(bool);

private:
    QLineEdit *m_pLineEdit_user;
    QLineEdit *m_pLineEdit_password;
    QLineEdit *m_pLineEdit_password1;
    QComboBox *m_pComboBox_type;

};

class ModifyPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyPasswordDialog(QWidget *parent = 0);
    ~ModifyPasswordDialog();

    //重置//
    void reset(const QString &user);
    //获取密码//
    QString getPassword();

private:
    void initUI();
    bool checkPassword();

private slots:
    void modifyPassword(bool);

private:
    QLabel *m_pLabel_user;
    QLineEdit *m_pLineEdit_password;
    QLineEdit *m_pLineEdit_password1;
};

class ModifyUserTypeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyUserTypeDialog(QWidget *parent = 0);
    ~ModifyUserTypeDialog();

    //重置//
    void reset(const QString &user, int type);
    //获取用户类型//
    int getUserType();

private:
    void initUI();

private slots:
    void modifyUserType(bool);

private:
    QComboBox *m_pComboBox_type;
    QLabel *m_pLabel_user;
};

class UserManagementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagementDialog(QWidget *parent = 0);
    ~UserManagementDialog();

private:
    void initUI();
    void readUserAndPwd();
    void writeUserAndPwd();
    void updateTableWidget();

private slots:
    void addUser(bool);
    void modifyPassword(bool);
    void modifyUserType(bool);
    void deleteUser(bool);

private:
    QTableWidget *m_pTableWidget;
    QMap<QString, struct User> m_mapUser;

    AddUserDialog *m_pAddUserDialog;               //添加用户名//
    ModifyPasswordDialog *m_pModifyPasswordDialog; //修改密码//
    ModifyUserTypeDialog *m_pModifyUserTypeDialog; //修改类型//
};

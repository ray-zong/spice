#include "UserManagementDialog.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>

#include "Common.h"
#include "DBUser.h"

UserManagementDialog::UserManagementDialog(QWidget *parent)
    : QDialog(parent)
    , m_pTableWidget(NULL)
    , m_pAddUserDialog(NULL)
    , m_pModifyPasswordDialog(NULL)
    , m_pModifyUserTypeDialog(NULL)
    , m_pDBUser(NULL)
{
    setWindowTitle(tr("User Management"));
    resize(800, 600);
    initUI();
    m_pDBUser = new DBUser;
    updateTableWidget();
}

UserManagementDialog::~UserManagementDialog()
{
    if(m_pDBUser)
    {
        delete m_pDBUser;
        m_pDBUser = NULL;
    }
}

void UserManagementDialog::initUI()
{
    //显示用户//
    m_pTableWidget = new QTableWidget(this);
    QHeaderView *pHeader = m_pTableWidget->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    //不可编辑//
    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置列数//
    m_pTableWidget->setColumnCount(3);
    QStringList list;
    list << tr("User")
         << tr("Password")
         << tr("User Type");
    m_pTableWidget->setHorizontalHeaderLabels(list);

    //添加用户//
    QHBoxLayout *pHLayout = new QHBoxLayout;
    QPushButton *pPushButton_add = new QPushButton(this);
    connect(pPushButton_add, SIGNAL(clicked(bool)), this, SLOT(addUser(bool)));
    pPushButton_add->setText(tr("New User"));

    //修改密码//
    QPushButton *pPushButton_alter = new QPushButton(this);
    connect(pPushButton_alter, SIGNAL(clicked(bool)), this, SLOT(modifyPassword(bool)));
    pPushButton_alter->setText(tr("Modify Password"));

    //修改用户类型//
    QPushButton *pPushButton_alterType = new QPushButton(this);
    connect(pPushButton_alterType, SIGNAL(clicked(bool)), this, SLOT(modifyUserType(bool)));
    pPushButton_alterType->setText(tr("Modify Type"));

    //删除用户//
    QPushButton *pPushButton_delete = new QPushButton(this);
    connect(pPushButton_delete, SIGNAL(clicked(bool)), this, SLOT(deleteUser(bool)));
    pPushButton_delete->setText(tr("Delete User"));

    pHLayout->addStretch();
    pHLayout->addWidget(pPushButton_add);
    pHLayout->addWidget(pPushButton_alter);
    pHLayout->addWidget(pPushButton_alterType);
    pHLayout->addWidget(pPushButton_delete);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTableWidget);
    pVLayout->addLayout(pHLayout);
}

void UserManagementDialog::updateTableWidget()
{
    if(m_pTableWidget == NULL)
        return;

    QTableWidgetItem *pItem = NULL;
    QVector<User> vecUser;
    Q_ASSERT(m_pDBUser != NULL);

    m_pDBUser->getUserInfo(vecUser);

    m_pTableWidget->clearContents();
    m_pTableWidget->setRowCount(vecUser.size());

    for(int i = 0; i < vecUser.size(); ++i)
    {
        pItem = new QTableWidgetItem(vecUser.at(i).user);
        m_pTableWidget->setItem(i, 0, pItem);

        pItem = new QTableWidgetItem(vecUser.at(i).password);
        m_pTableWidget->setItem(i, 1, pItem);

        switch (vecUser.at(i).type)
        {
        case Administrator:
            pItem = new QTableWidgetItem(tr("Administrator"));
            break;
        default:
            pItem = new QTableWidgetItem(tr("OrdinaryUser"));
            break;
        }
        m_pTableWidget->setItem(i, 2, pItem);
    }
}

void UserManagementDialog::addUser(bool)
{
    if(m_pAddUserDialog == NULL)
    {
        m_pAddUserDialog = new AddUserDialog(this);
    }

    m_pAddUserDialog->reset();

    if(m_pAddUserDialog->exec() == QDialog::Accepted)
    {
        //保存用户名和密码//
        User user;
        user.user = m_pAddUserDialog->getUserName().trimmed();
        user.password = m_pAddUserDialog->getPassword();
        user.type = m_pAddUserDialog->getUserType() == 0 ? Administrator : OrdinaryUser;

        Q_ASSERT(m_pDBUser != NULL);
        if(!m_pDBUser->addUser(user.user, user.password, user.type))
        {
            qDebug() << QString("add error:%1").arg(user.user);
            return;
        }

        int row = m_pTableWidget->rowCount();

        m_pTableWidget->insertRow(row);

        QTableWidgetItem *pItem = new QTableWidgetItem(user.user);
        m_pTableWidget->setItem(row, 0, pItem);

        pItem = new QTableWidgetItem(user.password);
        m_pTableWidget->setItem(row, 1, pItem);

        switch (user.type)
        {
        case Administrator:
            pItem = new QTableWidgetItem(tr("Administrator"));
            break;
        default:
            pItem = new QTableWidgetItem(tr("OrdinaryUser"));
            break;
        }
        m_pTableWidget->setItem(row, 2, pItem);
    }
}

void UserManagementDialog::modifyPassword(bool)
{
    if(m_pModifyPasswordDialog == NULL)
    {
        m_pModifyPasswordDialog = new ModifyPasswordDialog(this);
    }
    int row = m_pTableWidget->currentRow();

    m_pModifyPasswordDialog->reset(m_pTableWidget->item(row, 0)->text());

    if(m_pModifyPasswordDialog->exec() == QDialog::Accepted)
    {
        Q_ASSERT(m_pDBUser != NULL);
        if(m_pDBUser->modifyPassword(m_pTableWidget->item(row, 0)->text(), m_pModifyPasswordDialog->getPassword()))
        {
            m_pTableWidget->item(row, 1)->setText(m_pModifyPasswordDialog->getPassword());
        }
    }
}

void UserManagementDialog::modifyUserType(bool)
{
    if(m_pModifyUserTypeDialog == NULL)
    {
        m_pModifyUserTypeDialog = new ModifyUserTypeDialog(this);
    }

    int row = m_pTableWidget->currentRow();
    QString userName = m_pTableWidget->item(row, 0)->text();

    if(m_pTableWidget->item(row, 2)->text() == tr("Administrator"))
    {
        m_pModifyUserTypeDialog->reset(userName, Administrator);
    }
    else
    {
        m_pModifyUserTypeDialog->reset(userName, OrdinaryUser);
    }


    if(m_pModifyUserTypeDialog->exec() == QDialog::Accepted)
    {
        Q_ASSERT(m_pDBUser != NULL);
        int type = m_pModifyUserTypeDialog->getUserType();
        if(m_pDBUser->modifyUserType(userName, type))
        {
            if(type == Administrator)
            {
                m_pTableWidget->item(row, 2)->setText(tr("Administrator"));
            }
            else
            {
                m_pTableWidget->item(row, 2)->setText(tr("OrdinaryUser"));
            }
        }
    }
}

void UserManagementDialog::deleteUser(bool)
{
    if(m_pTableWidget == NULL)
        return;

    int row = m_pTableWidget->currentRow();
    QString strUser = m_pTableWidget->item(row, 0)->text();
    //admin无法删除//
    if(strUser == "admin")
    {
        QMessageBox msgBox_error;
        msgBox_error.setText(tr("You cannot delete the user:%1.").arg(strUser));
        msgBox_error.exec();
        return;
    }

    //进行删除提示//
    QMessageBox msgBox;
    msgBox.setText(tr("You will delete the user:%1.").arg(strUser));
    msgBox.setInformativeText(tr("Do you want to delete the user?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        //更新状态
        Q_ASSERT(m_pDBUser != NULL);
        if(m_pDBUser->deleteUser(m_pTableWidget->item(row, 0)->text()))
        {
            //界面删除//
            m_pTableWidget->removeRow(row);
        }
    }
}



AddUserDialog::AddUserDialog(QWidget *parent)
    : QDialog(parent)
    , m_pLineEdit_user(NULL)
    , m_pLineEdit_password(NULL)
    , m_pLineEdit_password1(NULL)
    , m_pComboBox_type(NULL)
{
    resize(600, 450);
    initUI();
}

AddUserDialog::~AddUserDialog()
{

}

void AddUserDialog::reset()
{
    m_pLineEdit_user->clear();
    m_pLineEdit_password->clear();
    m_pLineEdit_password1->clear();
    m_pComboBox_type->setCurrentIndex(1);
}

QString AddUserDialog::getUserName()
{
    return m_pLineEdit_user->text();
}

QString AddUserDialog::getPassword()
{
    return m_pLineEdit_password->text();
}

int AddUserDialog::getUserType()
{
    return m_pComboBox_type->currentIndex();
}

void AddUserDialog::initUI()
{
    //添加用户//
    QLabel *pLabel_user = new QLabel(tr("User Name:"), this);
    m_pLineEdit_user = new QLineEdit(this);
    QHBoxLayout *pHLayout_user = new QHBoxLayout;
    pHLayout_user->addWidget(pLabel_user);
    pHLayout_user->addWidget(m_pLineEdit_user);

    //第一次输入密码//
    QLabel *pLabel_password = new QLabel(tr("Input Password:"), this);
    m_pLineEdit_password = new QLineEdit(this);
    m_pLineEdit_password->setEchoMode(QLineEdit::Password);
    QHBoxLayout *pHLayout_password = new QHBoxLayout;
    pHLayout_password->addWidget(pLabel_password);
    pHLayout_password->addWidget(m_pLineEdit_password);

    //再次确认密码//
    QLabel *pLabel_password1 = new QLabel(tr("ReInput Password:"), this);
    m_pLineEdit_password1 = new QLineEdit(this);
    m_pLineEdit_password1->setEchoMode(QLineEdit::Password);
    QHBoxLayout *pHLayout_password1 = new QHBoxLayout;
    pHLayout_password1->addWidget(pLabel_password1);
    pHLayout_password1->addWidget(m_pLineEdit_password1);

    //用户类型//
    QLabel *pLabel_type = new QLabel(tr("User Type"), this);
    m_pComboBox_type = new QComboBox;
    m_pComboBox_type->addItem(tr("Administrator"));
    m_pComboBox_type->addItem(tr("OrdinaryUser"));
    m_pComboBox_type->setCurrentIndex(1);
    QHBoxLayout *pHLayout_type = new QHBoxLayout;
    pHLayout_type->addWidget(pLabel_type);
    pHLayout_type->addWidget(m_pComboBox_type);

    //按钮：添加，取消//
    QPushButton *pPushButton_add = new QPushButton(tr("Add"),this);
    connect(pPushButton_add, SIGNAL(clicked(bool)), this, SLOT(addUserName(bool)));

    QPushButton *pPushButton_cancel = new QPushButton(tr("Cancel"), this);
    connect(pPushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *pHLayout_pb = new QHBoxLayout;
    pHLayout_pb->addStretch();
    pHLayout_pb->addWidget(pPushButton_add);
    pHLayout_pb->addWidget(pPushButton_cancel);


    QVBoxLayout *pVLayout_addUser = new QVBoxLayout(this);
    pVLayout_addUser->addLayout(pHLayout_user);
    pVLayout_addUser->addLayout(pHLayout_password);
    pVLayout_addUser->addLayout(pHLayout_password1);
    pVLayout_addUser->addLayout(pHLayout_type);
    pVLayout_addUser->addLayout(pHLayout_pb);
}

bool AddUserDialog::checkUserName()
{
    QString text = m_pLineEdit_user->text();
    if(text.isEmpty())
    {
        m_pLineEdit_user->setPlaceholderText(tr("The user name cannot be Empty!"));
        return false;
    }
    //用户名不能重复//

    return true;
}

bool AddUserDialog::checkPassword()
{
    QString text1 = m_pLineEdit_password->text();
    QString text2 = m_pLineEdit_password1->text();

    if(text1.isEmpty())
    {
        m_pLineEdit_password->setPlaceholderText(tr("The password cannot be Empty!"));
        return false;
    }

    if(text2.isEmpty())
    {
        m_pLineEdit_password1->setPlaceholderText(tr("The password cannot be Empty!"));
        return false;
    }

    if(text1 != text2)
    {
        m_pLineEdit_password1->clear();
        m_pLineEdit_password1->setPlaceholderText(tr("The password is error!"));
        return false;
    }

    return true;
}

void AddUserDialog::addUserName(bool)
{
    if(!checkUserName())
    {
        return;
    }
    if(!checkPassword())
    {
        return;
    }

    accept();
}

ModifyPasswordDialog::ModifyPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , m_pLabel_user(NULL)
    , m_pLineEdit_password(NULL)
    , m_pLineEdit_password1(NULL)
{
    initUI();
}

ModifyPasswordDialog::~ModifyPasswordDialog()
{

}

void ModifyPasswordDialog::reset(const QString &user)
{
    m_pLabel_user->setText(tr("%1").arg(user));
    m_pLineEdit_password->clear();
    m_pLineEdit_password1->clear();
}

QString ModifyPasswordDialog::getPassword()
{
    return m_pLineEdit_password->text();
}

void ModifyPasswordDialog::initUI()
{
    m_pLabel_user = new QLabel(this);
    //新密码//
    QLabel *pLabel_password = new QLabel(tr("Input Password:"), this);
    m_pLineEdit_password = new QLineEdit(this);
    m_pLineEdit_password->setEchoMode(QLineEdit::Password);
    QHBoxLayout *pHLayout_password = new QHBoxLayout;
    pHLayout_password->addWidget(pLabel_password);
    pHLayout_password->addWidget(m_pLineEdit_password);

    //确认密码//
    QLabel *pLabel_password1 = new QLabel(tr("ReInput Password:"), this);
    m_pLineEdit_password1 = new QLineEdit(this);
    m_pLineEdit_password1->setEchoMode(QLineEdit::Password);
    QHBoxLayout *pHLayout_password1 = new QHBoxLayout;
    pHLayout_password1->addWidget(pLabel_password1);
    pHLayout_password1->addWidget(m_pLineEdit_password1);

    QPushButton *pPushButton_modify = new QPushButton(tr("Modify"),this);
    connect(pPushButton_modify, SIGNAL(clicked(bool)), this, SLOT(modifyPassword(bool)));
    QPushButton *pPushButton_cancel = new QPushButton(tr("Cancel"), this);
    connect(pPushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *pHLayout_pb = new QHBoxLayout;
    pHLayout_pb->addStretch();
    pHLayout_pb->addWidget(pPushButton_modify);
    pHLayout_pb->addWidget(pPushButton_cancel);


    QVBoxLayout *pVLayout_addUser = new QVBoxLayout(this);
    pVLayout_addUser->addWidget(m_pLabel_user);
    pVLayout_addUser->addLayout(pHLayout_password);
    pVLayout_addUser->addLayout(pHLayout_password1);
    pVLayout_addUser->addLayout(pHLayout_pb);
}

bool ModifyPasswordDialog::checkPassword()
{
    QString text1 = m_pLineEdit_password->text();
    QString text2 = m_pLineEdit_password1->text();

    if(text1.isEmpty())
    {
        m_pLineEdit_password->setPlaceholderText(tr("The password cannot be Empty!"));
        return false;
    }

    if(text2.isEmpty())
    {
        m_pLineEdit_password1->setPlaceholderText(tr("The password cannot be Empty!"));
        return false;
    }

    if(text1 != text2)
    {
        m_pLineEdit_password1->clear();
        m_pLineEdit_password1->setPlaceholderText(tr("The password is error!"));
        return false;
    }

    return true;
}

void ModifyPasswordDialog::modifyPassword(bool)
{
    if(!checkPassword())
    {
        return;
    }

    accept();
}

ModifyUserTypeDialog::ModifyUserTypeDialog(QWidget *parent)
    : QDialog(parent)
    , m_pComboBox_type(NULL)
    , m_pLabel_user(NULL)
{
    initUI();
}

ModifyUserTypeDialog::~ModifyUserTypeDialog()
{

}

void ModifyUserTypeDialog::reset(const QString &user, int type)
{
    m_pLabel_user->setText(tr("%1").arg(user));
    m_pComboBox_type->setCurrentIndex(type);
}

int ModifyUserTypeDialog::getUserType()
{
    return m_pComboBox_type->currentIndex();
}

void ModifyUserTypeDialog::initUI()
{
    //添加用户//
    m_pLabel_user = new QLabel(this);

    QLabel *pLabel_type = new QLabel(tr("User Type"), this);
    m_pComboBox_type = new QComboBox;
    m_pComboBox_type->addItem(tr("Administrator"));
    m_pComboBox_type->addItem(tr("OrdinaryUser"));
    QHBoxLayout *pHLayout_type = new QHBoxLayout;
    pHLayout_type->addWidget(pLabel_type);
    pHLayout_type->addWidget(m_pComboBox_type);

    QPushButton *pPushButton_modify = new QPushButton(tr("Modify"),this);
    connect(pPushButton_modify, SIGNAL(clicked(bool)), this, SLOT(modifyUserType(bool)));
    QPushButton *pPushButton_cancel = new QPushButton(tr("Cancel"), this);
    connect(pPushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
    QHBoxLayout *pHLayout_pb = new QHBoxLayout;
    pHLayout_pb->addStretch();
    pHLayout_pb->addWidget(pPushButton_modify);
    pHLayout_pb->addWidget(pPushButton_cancel);


    QVBoxLayout *pVLayout_addUser = new QVBoxLayout(this);
    pVLayout_addUser->addWidget(m_pLabel_user);
    pVLayout_addUser->addLayout(pHLayout_type);
    pVLayout_addUser->addLayout(pHLayout_pb);
}

void ModifyUserTypeDialog::modifyUserType(bool)
{
    accept();
}

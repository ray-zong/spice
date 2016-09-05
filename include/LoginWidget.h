#pragma once

#include <QWidget>
#include <QMap>

class MainWindow;
class QLineEdit;
class QLabel;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    LoginWidget();
    ~LoginWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    QWidget* initLoginWidget();
    void setMinAndCloseButton(QWidget *parent);
    bool checkUserAndPwd();

    void readUserAndPwd();

private slots:
    void start();
    void slot_minWindow();
    void slot_closeWindow();
    void timeout();

private:
    MainWindow *m_pMainWindow;

    QLineEdit *m_pLineEdit_user;
    QLineEdit *m_pLineEdit_password;

    //move UI
    QPoint m_movePoint;
    bool m_bMousePress;

    QMap<QString, struct User> m_mapUser;

    QLabel *m_pLabelError;
    QTimer *m_pErrorTipTimer;
};


#pragma once

#include <QWidget>

class MainWindow;
class QLineEdit;

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

private slots:
    void start();
    void slot_minWindow();
    void slot_closeWindow();

private:
    MainWindow *m_pMainWindow;

    QLineEdit *m_pLineEdit_user;
    QLineEdit *m_pLineEdit_password;

    //move UI
    QPoint m_movePoint;
    bool m_bMousePress;
};


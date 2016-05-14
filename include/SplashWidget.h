#pragma once

#include <QWidget>

class MainWindow;
class QMouseEvent;

class SplashWidget : public QWidget
{
    Q_OBJECT
public:
    SplashWidget();
    ~SplashWidget();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private slots:
    void about();
    void start();

private:
    MainWindow *m_pMainWindow;

    //move UI
    QPoint m_movePoint;
    bool m_bMousePress;
};


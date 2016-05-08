#pragma once

#include <QWidget>

class MainWindow;

class SplashWidget : public QWidget
{
    Q_OBJECT
public:
    SplashWidget();
    ~SplashWidget();

private slots:
    void about();
    void start();

private:
    MainWindow *m_pMainWindow;
};


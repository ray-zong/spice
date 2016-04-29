#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class AddSpice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadExcelFile();
    void aboutSoftware();

private:
    Ui::MainWindow *ui;

    AddSpice *m_pAddSpice;
};

#endif // MAINWINDOW_H

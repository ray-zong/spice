#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class AddSpice;
class DeleteSpice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void loadExcelFile();
    void aboutSoftware();
    void showDeleteSpiceWidget();

private:
    Ui::MainWindow *ui;

    AddSpice *m_pAddSpice;
    DeleteSpice *m_pDeleteSpice;
};


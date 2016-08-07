#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class DisplaySpice;
class ResultWidget;
class QueryWidget;
class SingleSpiceDialog;
class OptionDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private:
    void initUI();

private slots:
    //加载Excel文件//
    void loadExcelFile();
    //关于软件//
    void aboutSoftware();
    //显示添加香料界面//
    void showNewSpiceWidget();
    //显示添修改香料界面//
    void showAlterSpiceWidget();
    //显示停靠窗口//
    void showAllSpiceWidget();
    //删除香料//
    void deleteSpice(int id);
    //修改香料//
    void alterSpice(int id);
    //显示香料//
    void showSpice(int id);
    //设置//
    void showOption();

signals:
    void closeWindow();

private:
    Ui::MainWindow *ui;

    QueryWidget *m_pQueryWidget;
    DisplaySpice *m_pDisplaySpice;          //分类显示所有香料
    ResultWidget *m_pResultWidget;          //显示查询结果
    SingleSpiceDialog *m_pSingleSpiceDialog;//显示单个香料界面(添加和修改)
    QDockWidget *m_pDockWidget;             //显示所有香料
    OptionDialog *m_pOptionDialog;          //选项(设置)界面

};


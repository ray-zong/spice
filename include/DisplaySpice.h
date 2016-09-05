#pragma once

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class DisplaySpice : public QWidget
{
    Q_OBJECT
public:
    explicit DisplaySpice(int userType, QWidget *parent = 0);
    ~DisplaySpice();

    //更新香料//
    void updateSpice();

private:
    void initTreeWidgetUI();
    void initStackedWidgetUI();

signals:
    void deleteSpice(int id);
    void modifySpice(int id);
    void showSpice(int id);

public slots:
    void customContextMenuRequested(QPoint);
    void currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

private:
    QTreeWidget *m_pTreeWidget;
    int m_nUserType;
};

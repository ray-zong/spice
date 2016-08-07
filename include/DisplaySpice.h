#pragma once

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class DisplaySpice : public QWidget
{
    Q_OBJECT
public:
    explicit DisplaySpice(QWidget *parent = 0);
    ~DisplaySpice();

private:
    void initTreeWidgetUI();
    void initStackedWidgetUI();

signals:
    void deleteSpice(int id);
    void alterSpice(int id);
    void showSpice(int id);

public slots:
    void customContextMenuRequested(QPoint);
    void currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

private:
    QTreeWidget *m_pTreeWidget;
};

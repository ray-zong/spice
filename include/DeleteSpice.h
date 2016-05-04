#pragma once

#include <QWidget>

class QTableWidget;

class DeleteSpice : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteSpice(QWidget *parent = 0);
    ~DeleteSpice();

    void showAllSpice();

private:
    void initUI();
    void save();

private slots:
    void deleteSpice();

private:
    QTableWidget *m_pTableWidget;
};


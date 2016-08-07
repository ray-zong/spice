#pragma once

#include <QWidget>

#include "Common.h"

//Qt
class QTabWidget;

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = 0);

    //香料查询结果
    void displaySpice(const QVector<SpiceInfoData> &);

    //成分查询结果
    void displayContent(const QString &name, const QVector<SpiceByContent> &);

    void showSpice(int id);

private:
    //初始化界面
    void initUI();

    //通过香料成份查询名称
    bool querySpiceContent(const QString &);

signals:

public slots:


private:
    QTabWidget *m_pTabWidget;
};

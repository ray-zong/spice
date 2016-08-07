#pragma once

#include <QWidget>

#include "Common.h"

class QTabWidget;
class QPaintEvent;

class BaseInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseInfoWidget(QWidget *parent = 0);
    ~BaseInfoWidget();

    void setSpiceBaseInfo(const SpiceInfoData &spiceInfo);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    void initUI();

private:
    QTabWidget *m_pTabWidget;
};


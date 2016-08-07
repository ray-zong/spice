#pragma once

#include <QWidget>
#include <QTableWidgetItem>

#include "Common.h"

class QTabWidget;
class QPaintEvent;

class MyTableWidgetItem : public QTableWidgetItem {
    public:
        bool operator <(const QTableWidgetItem &other) const
        {
            bool ok = false;
            bool otherOk = false;
            double value = text().toDouble(&ok);
            double otherValue = other.text().toDouble(&otherOk);
            if(ok && otherOk)
            {
                return value < otherValue;
            }
            else
            {
                return QTableWidgetItem::operator <(other);
            }
        }
};


class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = 0);
    ~ContentWidget();

    void setSpiceContent(const SpiceInfoData &spiceInfo);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    void initUI();

private:
    QTabWidget *m_pTabWidget;
};


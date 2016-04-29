#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>

//Qt
class QTableWidget;

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = 0);

    bool display(int type, QString query);

private:
    //通过香料名称查询成份
    bool querySpice(const QString &);

    //通过香料成份查询名称
    bool querySpiceContent(const QString &);

signals:

public slots:


private:
    QTableWidget *m_pTableWidget;
};

#endif // RESULTWIDGET_H

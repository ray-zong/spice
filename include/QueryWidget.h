#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QComboBox;
class QStringListModel;

class ResultWidget;

class QueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueryWidget(ResultWidget *pResultWidget, QWidget *parent = 0);
    ~QueryWidget();

    void setQueryType(int type);

private:
    void initUI();

signals:
    void queryTypeChanged(int);

public slots:
    void displayResult();
    void queryTextChanged(const QString &);
    void currentIndexChanged(int);

private:
    QLineEdit *m_pLineEdit_query;
    QPushButton *m_pPushButton_query;
    QComboBox *m_pComboBox_precise;

    QStringListModel *m_pStringListModel;

    ResultWidget *m_pResultWidget;
};

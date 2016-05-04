#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QRadioButton;
class QStringListModel;

class ResultWidget;

class QueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueryWidget(QWidget *parent = 0);
    ~QueryWidget();

private:
    void initUI();
signals:

public slots:
    void displayResult();
    void queryTextChanged(const QString &);
private:
    QLineEdit *m_pLineEdit_query;
    QPushButton *m_pPushButton_query;
    QRadioButton *m_pRadioButton_precise;

    ResultWidget *m_pResultWidget;

    QStringListModel *m_pStringListModel;
};

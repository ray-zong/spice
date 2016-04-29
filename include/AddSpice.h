#ifndef ADDSPICE_H
#define ADDSPICE_H

#include <QWidget>

namespace Ui {
class AddSpice;
}

class QStringListModel;

class AddSpice : public QWidget
{
    Q_OBJECT

public:
    explicit AddSpice(QWidget *parent = 0);
    ~AddSpice();

protected:
    void closeEvent( QCloseEvent * event );

private slots:
    void finish();
    void addContent();
    void deleteContent();
    void insertContent();
    void spiceNameChanged(const QString &);
    void displayResult();

    void save();

private:
    QStringListModel *m_pStringListModel;
private:
    Ui::AddSpice *ui;
};

#endif // ADDSPICE_H

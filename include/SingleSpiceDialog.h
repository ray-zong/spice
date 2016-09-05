#ifndef SINGLESPICEDIALOG_H
#define SINGLESPICEDIALOG_H

#include <QDialog>

#include "Common.h"

namespace Ui {
class SingleSpiceDialog;
}

class SingleSpiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingleSpiceDialog(QWidget *parent = 0);
    ~SingleSpiceDialog();

    void clearSpice();
    void setSpice(const SpiceInfoData &spiceInfo);

private slots:
    void updateSpice(bool);
    void addContent(bool);
    void deleteContent(bool);
    void insertContent(bool);
    void exportContent(bool);
    void importContent(bool);

private:
    Ui::SingleSpiceDialog *ui;
};

#endif // SINGLESPICEDIALOG_H

#pragma once

#include <QDialog>

#include "Common.h"

namespace Ui {
class SingleSpiceDialog;
}

class QProgressDialog;

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
    void selectImagePath(bool);

private:
    Ui::SingleSpiceDialog *ui;
    int m_nSpiceId;

    int m_nDialogStatus;
    QProgressDialog *m_pProgressDialog;
};

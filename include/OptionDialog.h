﻿#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog();

private slots:
    void setImageFilePath(bool);
    void saveOption();

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H

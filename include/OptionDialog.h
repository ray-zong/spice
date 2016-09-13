#pragma once

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

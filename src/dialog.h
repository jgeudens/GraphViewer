#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "datatypes.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void getDataSettings(DataTypes::DataFileSettings * pSettings);

private slots:
    void selectDataFile();
    void presetSelected(int index);
    void fieldSeparatorSelected(int index);

    void done(int r);

private:
    Ui::Dialog * _ui;

    DataTypes::DataFileSettings _settings;

};

#endif // DIALOG_H

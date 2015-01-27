#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "dataparsersettings.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void getDataSettings(DataParserSettings *pSettings);

private slots:
    void selectDataFile();
    void presetSelected(int index);
    void fieldSeparatorSelected(int index);

    void done(int r);

private:
    Ui::Dialog * _ui;

    DataParserSettings _settings;

};

#endif // DIALOG_H

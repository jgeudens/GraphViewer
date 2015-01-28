#ifndef LoadFileDialog_H
#define LoadFileDialog_H

#include <QDialog>

#include "dataparsersettings.h"

namespace Ui {
class LoadFileDialog;
}

class LoadFileDialog : public QDialog
{
    Q_OBJECT

public:

    explicit LoadFileDialog(DataParserSettings *pParseSettings, QWidget *parent = 0);
    ~LoadFileDialog();

private slots:
    void selectDataFile();
    void presetSelected(int index);
    void fieldSeparatorSelected(int index);

    void done(int r);

private:
    Ui::LoadFileDialog * _ui;

    DataParserSettings * _pParseSettings;

};

#endif // LoadFileDialog_H

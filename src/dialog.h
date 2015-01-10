#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:


    typedef struct
    {
      QString path;
      QString fieldSeparator;
      quint32 dataRow;
      quint32 dataColumn;
    } DataFileSettings;

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void getDataSettings(DataFileSettings * pSettings);

private slots:
    void selectDataFile();

private:
    Ui::Dialog * _ui;

    DataFileSettings _settings;

};

#endif // DIALOG_H

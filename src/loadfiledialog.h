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

    explicit LoadFileDialog(QWidget *parent = 0);
    ~LoadFileDialog();

    int exec(DataParserSettings *pParseSettings);

private slots:
    void selectDataFile();
    void presetSelected(int index);
    void fieldSeparatorSelected(int index);

    void done(int r);

private:

    typedef struct _ComboListItem
    {
        _ComboListItem(QString _name, QString _userData)
        {
            name = _name;
            userData = _userData;
        }


        QString name;
        QString userData;
    } ComboListItem;


    static const QList<ComboListItem> _fieldSeparatorList;
    static const QList<ComboListItem> _decimalSeparatorList;
    static const QList<ComboListItem> _groupSeparatorList;

    Ui::LoadFileDialog * _pUi;

    DataParserSettings * _pParseSettings;

    qint32 findIndexInCombo( QList<ComboListItem> comboItemList, QString userDataKey);

};

#endif // LoadFileDialog_H

#ifndef LoadFileDialog_H
#define LoadFileDialog_H

#include <QDialog>
#include <QDateTime>

#include "presetparser.h"
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

    int exec();
    int exec(QString file);
    void getParserSettings(DataParserSettings *pParseSettings);

private slots:
    void selectDataFile();
    void presetSelected(int index);
    void fieldSeparatorSelected(int index);
    void toggledLabelRow(bool bState);

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
    static const QString _presetFilename;


    Ui::LoadFileDialog * _pUi;

    QDateTime _lastModified;
    QList<PresetParser::Preset> _presetList;

    qint32 findIndexInCombo(QList<ComboListItem> comboItemList, QString userDataKey);
    void loadPreset(void);
    void setPreset(QString filename);

};

#endif // LoadFileDialog_H

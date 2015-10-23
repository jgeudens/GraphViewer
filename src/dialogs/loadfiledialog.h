#ifndef LoadFileDialog_H
#define LoadFileDialog_H

#include <QDialog>

#include "parsermodel.h"
#include "presetparser.h"

namespace Ui {
class LoadFileDialog;
}

class LoadFileDialog : public QDialog
{
    Q_OBJECT

public:

    explicit LoadFileDialog(ParserModel * pParserModel, QWidget *parent = 0);
    ~LoadFileDialog();

    int exec();
    int exec(QString file);

private slots:

    void updateDynamicSession();
    void updatePath();
    void updateFieldSeparator();
    void updategroupSeparator();
    void updateDecimalSeparator();
    void updateCommentSequence();
    void updateDataRow();
    void updateColumn();
    void updateLabelRow();

    void dynamicSessionUpdated(bool bDynamic);
    void dataFileSelected();
    void fieldSeparatorSelected(int index);
    void customFieldSeparatorUpdated();
    void groupSeparatorSelected(int  index);
    void decimalSeparatorSelected(int  index);
    void commentUpdated();
    void dataRowUpdated();
    void columnUpdated();
    void toggledLabelRow(bool bLabelRow);
    void labelRowUpdated();

    void presetSelected(int index);

    void done(int r);

private:

    bool validateSettingsData();

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

    ParserModel * _pParserModel;
    PresetParser _presetParser;

    qint32 findIndexInCombo(QList<ComboListItem> comboItemList, QString userDataKey);
    void loadPreset(void);
    void setPreset(QString filename);

};

#endif // LoadFileDialog_H

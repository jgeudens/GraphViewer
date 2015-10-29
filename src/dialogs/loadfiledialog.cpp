
#include <QFileDialog>

#include "util.h"

#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"


const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_fieldSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" ; (semicolon)", ";")
                                                             << ComboListItem(" , (comma)", ",")
                                                             << ComboListItem(" tab", QString('\t'))
                                                             << ComboListItem(" custom", "custom");

const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_decimalSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" , (comma)", ",")
                                                            << ComboListItem(" . (point)", ".");

const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_groupSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" , (comma)", ",")
                                                            << ComboListItem(" . (point)", ".")
                                                            << ComboListItem("   (space)", " ");

LoadFileDialog::LoadFileDialog(ParserModel * pParserModel, QWidget *parent) :
    QDialog(parent),
    _pUi(new Ui::LoadFileDialog)
{
    _pUi->setupUi(this);

    _pParserModel = pParserModel;

    // load presets
    loadPreset();

    /*-- Fill combo boxes --*/
    foreach(ComboListItem listItem, _decimalSeparatorList)
    {
        _pUi->comboDecimalSeparator->addItem(listItem.name, listItem.userData);
    }

    foreach(ComboListItem listItem, _fieldSeparatorList)
    {
        _pUi->comboFieldSeparator->addItem(listItem.name, listItem.userData);
    }

    foreach(ComboListItem listItem, _groupSeparatorList)
    {
        _pUi->comboGroupSeparator->addItem(listItem.name, listItem.userData);
    }

    // Handle signals from model
    connect(_pParserModel, SIGNAL(dynamicSessionChanged()), this, SLOT(updateDynamicSession()));
    connect(_pParserModel, SIGNAL(pathChanged()), this, SLOT(updatePath()));
    connect(_pParserModel, SIGNAL(fieldSeparatorChanged()), this, SLOT(updateFieldSeparator()));
    connect(_pParserModel, SIGNAL(groupSeparatorChanged()), this, SLOT(updategroupSeparator()));
    connect(_pParserModel, SIGNAL(decimalSeparatorChanged()), this, SLOT(updateDecimalSeparator()));
    connect(_pParserModel, SIGNAL(commentSequenceChanged()), this, SLOT(updateCommentSequence()));
    connect(_pParserModel, SIGNAL(dataRowChanged()), this, SLOT(updateDataRow()));
    connect(_pParserModel, SIGNAL(columnChanged()), this, SLOT(updateColumn()));
    connect(_pParserModel, SIGNAL(labelRowChanged()), this, SLOT(updateLabelRow()));


    // Handle signal from controls
    connect(_pUi->checkDynamicSession, SIGNAL(toggled(bool)), this, SLOT(dynamicSessionUpdated(bool)));
    connect(_pUi->btnDataFile, SIGNAL(released()), this, SLOT(dataFileSelected()));
    connect(_pUi->comboFieldSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(fieldSeparatorSelected(int)));
    connect(_pUi->lineCustomFieldSeparator, SIGNAL(editingFinished()), this, SLOT(customFieldSeparatorUpdated()));
    connect(_pUi->comboGroupSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(groupSeparatorSelected(int)));
    connect(_pUi->comboDecimalSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(decimalSeparatorSelected(int)));
    connect(_pUi->lineComment, SIGNAL(editingFinished()), this, SLOT(commentUpdated()));
    connect(_pUi->spinDataRow, SIGNAL(valueChanged(int)), this, SLOT(dataRowUpdated()));
    connect(_pUi->spinColumn, SIGNAL(valueChanged(int)), this, SLOT(columnUpdated()));
    connect(_pUi->checkLabelRow, SIGNAL(toggled(bool)), this, SLOT(toggledLabelRow(bool)));
    connect(_pUi->spinLabelRow, SIGNAL(valueChanged(int)), this, SLOT(labelRowUpdated()));
    connect(_pUi->comboPreset, SIGNAL(currentIndexChanged(int)), this, SLOT(presetSelected(int)));

    // Signal to change preset to manual
    connect(_pUi->comboFieldSeparator, SIGNAL(activated(int)), this, SLOT(setPresetToManual()));
    connect(_pUi->lineCustomFieldSeparator, SIGNAL(textEdited(QString)), this, SLOT(setPresetToManual()));
    connect(_pUi->comboGroupSeparator, SIGNAL(activated(int)), this, SLOT(setPresetToManual()));
    connect(_pUi->comboDecimalSeparator, SIGNAL(activated(int)), this, SLOT(setPresetToManual()));
    connect(_pUi->lineComment, SIGNAL(textEdited(QString)), this, SLOT(setPresetToManual()));
    connect(_pUi->spinDataRow, SIGNAL(editingFinished()), this, SLOT(setPresetToManual()));
    connect(_pUi->spinColumn, SIGNAL(editingFinished()), this, SLOT(setPresetToManual()));
    connect(_pUi->checkLabelRow, SIGNAL(clicked(bool)), this, SLOT(setPresetToManual()));
    connect(_pUi->spinLabelRow, SIGNAL(editingFinished()), this, SLOT(setPresetToManual()));

    // Select first preset
    _pUi->comboPreset->setCurrentIndex(-1);
    _pUi->comboPreset->setCurrentIndex(0);

    _pParserModel->triggerUpdate();

}

LoadFileDialog::~LoadFileDialog()
{
    delete _pUi;
}

void LoadFileDialog::open()
{
    QDialog::open();
}

void LoadFileDialog::open(QString file)
{
    _pParserModel->setPath(file);

    QDialog::open();
}

void LoadFileDialog::updateDynamicSession()
{
    _pUi->checkDynamicSession->setChecked(_pParserModel->dynamicSession());
}

void LoadFileDialog::updatePath()
{
    _pUi->lineDataFile->setText(_pParserModel->path());

    setPresetAccordingKeyword(_pUi->lineDataFile->text());
}

void LoadFileDialog::updateFieldSeparator()
{
    const qint32 comboIndex = findIndexInCombo(_fieldSeparatorList, QString(_pParserModel->fieldSeparator()));
    const qint32 customIndex = findIndexInCombo(_fieldSeparatorList, tr("custom"));

    if (comboIndex == -1)
    {
        // Custom field seperator
        if (_pUi->comboFieldSeparator->currentIndex() != customIndex)
        {
            _pUi->comboFieldSeparator->setCurrentIndex(customIndex);
        }

        _pUi->lineCustomFieldSeparator->setText(QString(_pParserModel->fieldSeparator()));
    }
    else
    {
        _pUi->comboFieldSeparator->setCurrentIndex(comboIndex);
        _pUi->lineCustomFieldSeparator->setText('\0');
    }
}

void LoadFileDialog::updategroupSeparator()
{
    _pUi->comboGroupSeparator->setCurrentIndex(findIndexInCombo(_groupSeparatorList, QString(_pParserModel->groupSeparator())));
}

void LoadFileDialog::updateDecimalSeparator()
{
    _pUi->comboDecimalSeparator->setCurrentIndex(findIndexInCombo(_decimalSeparatorList, QString(_pParserModel->decimalSeparator())));
}

void LoadFileDialog::updateCommentSequence()
{
    _pUi->lineComment->setText(_pParserModel->commentSequence());
}

void LoadFileDialog::updateDataRow()
{
    _pUi->spinDataRow->setValue(_pParserModel->dataRow() + 1);  // + 1 based because 0 based internally
}

void LoadFileDialog::updateColumn()
{
    _pUi->spinColumn->setValue(_pParserModel->column() + 1);  // + 1 based because 0 based internally
}

void LoadFileDialog::updateLabelRow()
{
    if (_pParserModel->labelRow() == -1)
    {
        _pUi->checkLabelRow->setCheckState(Qt::Unchecked);
    }
    else
    {
        _pUi->checkLabelRow->setCheckState(Qt::Checked);
        _pUi->spinLabelRow->setValue(_pParserModel->labelRow() + 1);   // + 1 based because 0 based internally
    }
}


void LoadFileDialog::dynamicSessionUpdated(bool bDynamic)
{
    _pParserModel->setDynamicSession(bDynamic);
}

void LoadFileDialog::dataFileSelected()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setOption(QFileDialog::HideNameFilterDetails, false);
    fileDialog.setWindowTitle(tr("Select data file"));
    fileDialog.setNameFilter(tr("*.* (*.*)"));

    if (fileDialog.exec() == QDialog::Accepted)
    {
        _pParserModel->setPath(fileDialog.selectedFiles().first());
    }
}

void LoadFileDialog::fieldSeparatorSelected(int index)
{
    if (_pUi->comboFieldSeparator->itemData(index).toString().toLower() == tr("custom"))
    {
        // Enable custom field box
        _pUi->lineCustomFieldSeparator->setEnabled(true);
    }
    else
    {
        // Disable custom field box
        _pUi->lineCustomFieldSeparator->setEnabled(false);

        _pParserModel->setFieldSeparator(_pUi->comboFieldSeparator->itemData(index).toString().at(0));
    }
}

void LoadFileDialog::customFieldSeparatorUpdated()
{
    if (!_pUi->lineCustomFieldSeparator->text().isEmpty())
    {
        _pParserModel->setFieldSeparator(_pUi->lineCustomFieldSeparator->text().at(0));
    }
}

void LoadFileDialog::groupSeparatorSelected(int index)
{
    _pParserModel->setGroupSeparator(_pUi->comboGroupSeparator->itemData(index).toString().at(0));
}

void LoadFileDialog::decimalSeparatorSelected(int index)
{
    _pParserModel->setDecimalSeparator(_pUi->comboDecimalSeparator->itemData(index).toString().at(0));
}

void LoadFileDialog::commentUpdated()
{
    _pParserModel->setCommentSequence(_pUi->lineComment->text());
}

void LoadFileDialog::dataRowUpdated()
{
    _pParserModel->setDataRow(_pUi->spinDataRow->value() - 1);  // - 1 based because 0 based internally
}

void LoadFileDialog::columnUpdated()
{
    _pParserModel->setColumn(_pUi->spinColumn->value() - 1);  // - 1 based because 0 based internally
}

void LoadFileDialog::toggledLabelRow(bool bLabelRow)
{
    _pUi->spinLabelRow->setEnabled(bLabelRow);
}

void LoadFileDialog::labelRowUpdated()
{
    _pParserModel->setLabelRow(_pUi->spinLabelRow->value() - 1);   // - 1 based because 0 based internally
}

void LoadFileDialog::presetSelected(int index)
{
    const qint32 presetIndex = index - cPresetListOffset;

    if ((presetIndex >= 0) && (presetIndex < _presetParser.presetList().size()))
    {
        _pParserModel->setColumn(_presetParser.presetList()[presetIndex].column -1);
        _pParserModel->setDataRow(_presetParser.presetList()[presetIndex].dataRow - 1);
        _pParserModel->setLabelRow(_presetParser.presetList()[presetIndex].labelRow - 1);
        _pParserModel->setDecimalSeparator(_presetParser.presetList()[presetIndex].decimalSeparator);
        _pParserModel->setFieldSeparator(_presetParser.presetList()[presetIndex].fieldSeparator);
        _pParserModel->setGroupSeparator(_presetParser.presetList()[presetIndex].thousandSeparator);
        _pParserModel->setCommentSequence(_presetParser.presetList()[presetIndex].commentSequence);
        _pParserModel->setDynamicSession(_presetParser.presetList()[presetIndex].bDynamicSession);
    }
}

void LoadFileDialog::done(int r)
{
    bool bValid = true;

    if(QDialog::Accepted == r)  // ok was pressed
    {
        // Validate the data
        bValid = validateSettingsData();
    }
    else
    {
        // cancel, close or exc was pressed;
        bValid = true;
    }

    if (bValid)
    {
        QDialog::done(r);
    }
}

void LoadFileDialog::setPresetToManual()
{
    _pUi->comboPreset->setCurrentIndex(cPresetManualIndex);
}

bool LoadFileDialog::validateSettingsData()
{
    bool bOk = true;

    if (bOk)
    {
        if (!QFileInfo(_pParserModel->path()).exists())
        {
            bOk = false;
            Util::showError(tr("Data file doesn't exist"));
        }
    }

    if (bOk)
    {
        if (_pParserModel->fieldSeparator().isNull())
        {
            bOk = false;
            Util::showError(tr("Custom field separator isn't defined correctly"));
        }
    }

    return bOk;
}

qint32 LoadFileDialog::findIndexInCombo(QList<ComboListItem> comboItemList, QString userDataKey)
{
    qint32 index = -1;

    for (qint32 i = 0; i < comboItemList.size(); i++)
    {
        if (comboItemList[i].userData.compare(userDataKey, Qt::CaseInsensitive) == 0)
        {
            index = i;
            break;
        }
    }

    return index;
}

void LoadFileDialog::loadPreset(void)
{
    _presetParser.loadPresetsFromFile();

    _pUi->comboPreset->clear();
    _pUi->comboPreset->addItem("Manual");

    foreach(PresetParser::Preset preset, _presetParser.presetList())
    {
        _pUi->comboPreset->addItem(preset.name);
    }
}

void LoadFileDialog::setPresetAccordingKeyword(QString filename)
{
    qint32 presetComboIndex = -1;

    // Loop through presets and set preset if keyword is in filename
    for (qint32 index = 0; index < _presetParser.presetList().size(); index ++)
    {
        if (!_presetParser.presetList()[index].keyword.isEmpty())
        {
            if (QFileInfo(filename).fileName().contains(_presetParser.presetList()[index].keyword, Qt::CaseInsensitive))
            {
                presetComboIndex = index + cPresetListOffset;
                break;
            }
        }
    }

    // No preset found
    if (presetComboIndex == -1)
    {
        // set to manual
        // TODO: set to auto
        presetComboIndex = cPresetManualIndex;
    }

    _pUi->comboPreset->setCurrentIndex(-1);
    _pUi->comboPreset->setCurrentIndex(presetComboIndex);
}

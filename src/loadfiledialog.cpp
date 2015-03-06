
#include <QFileDialog>
#include <QStandardPaths>

#include "util.h"

#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"


const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_fieldSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" ; (semicolon)", ";")
                                                             << ComboListItem(" , (comma)", ",")
                                                             << ComboListItem(" tab", "\t")
                                                             << ComboListItem(" custom", "custom");

const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_decimalSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" , (comma)", ",")
                                                            << ComboListItem(" . (point)", ".");

const QList<LoadFileDialog::ComboListItem> LoadFileDialog::_groupSeparatorList
                                    = QList<ComboListItem>() << ComboListItem(" , (comma)", ",")
                                                            << ComboListItem(" . (point)", ".")
                                                            << ComboListItem("   (space)", " ");

const QString LoadFileDialog::_presetFilename = QString("presets.xml");

LoadFileDialog::LoadFileDialog(QWidget *parent) :
    QDialog(parent),
    _pUi(new Ui::LoadFileDialog)
{
    _pUi->setupUi(this);

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


    /*-- Set default item --*/
    const QChar decimalPoint = QLocale::system().decimalPoint();

    // DecimalSeparator
    if (decimalPoint == '.')
    {
        _pUi->comboDecimalSeparator->setCurrentIndex(findIndexInCombo(_decimalSeparatorList, "."));
    }
    else
    {
        _pUi->comboDecimalSeparator->setCurrentIndex(findIndexInCombo(_decimalSeparatorList, ","));
    }

    // FieldSeparator
    if (decimalPoint == ',')
    {
        _pUi->comboFieldSeparator->setCurrentIndex(findIndexInCombo(_fieldSeparatorList, ";"));
    }
    else
    {
        _pUi->comboFieldSeparator->setCurrentIndex(findIndexInCombo(_fieldSeparatorList, ","));
    }

    // group separator
    _pUi->comboGroupSeparator->setCurrentIndex(findIndexInCombo(_groupSeparatorList, QLocale::system().groupSeparator()));

    connect(_pUi->btnDataFile, SIGNAL(released()), this, SLOT(selectDataFile()));
    connect(_pUi->comboPreset, SIGNAL(currentIndexChanged(int)), this, SLOT(presetSelected(int)));
    connect(_pUi->comboFieldSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(fieldSeparatorSelected(int)));
    connect(_pUi->checkLabelRow, SIGNAL(toggled(bool)), this, SLOT(toggledLabelRow(bool)));
}

LoadFileDialog::~LoadFileDialog()
{
    delete _pUi;
}

int LoadFileDialog::exec()
{
    loadPreset();

    return QDialog::exec();
}

int LoadFileDialog::exec(QString file, bool bSkipDialog)
{
    int result = QDialog::Rejected;
    loadPreset();

    _pUi->lineDataFile->setText(file);
    setPreset(file);

    if (bSkipDialog && validateData())
    {
        result = QDialog::Accepted;
    }
    else
    {
        result = QDialog::exec();
    }

    return result;
}

void LoadFileDialog::getParserSettings(DataParserSettings * pParseSettings)
{
    pParseSettings->setPath(_pUi->lineDataFile->text());
    pParseSettings->setColumn(_pUi->spinColumn->value() - 1); // 1 based to 0 based
    pParseSettings->setDataRow(_pUi->spinDataRow->value() - 1); // 1 based to 0 based

    if (_pUi->checkLabelRow->checkState() == Qt::Checked)
    {
        pParseSettings->setLabelRow(_pUi->spinLabelRow->value() - 1); // 1 based to 0 based
    }
    else
    {
        // No label row
        pParseSettings->setLabelRow(-1);
    }

    if (_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
    {
        pParseSettings->setFieldSeparator(_pUi->lineCustomFieldSeparator->text());
    }
    else
    {
        pParseSettings->setFieldSeparator(_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString());
    }

    pParseSettings->setDecimalSeparator(_pUi->comboDecimalSeparator->itemData(_pUi->comboDecimalSeparator->currentIndex()).toString());
    pParseSettings->setGroupSeparator(_pUi->comboGroupSeparator->itemData(_pUi->comboGroupSeparator->currentIndex()).toString());
    pParseSettings->setDynamicSession(_pUi->checkDynamicSession->checkState() == Qt::Checked ? true : false);
    pParseSettings->setWatchFileData(true);
}

void LoadFileDialog::selectDataFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setOption(QFileDialog::HideNameFilterDetails, false);
    fileDialog.setWindowTitle(tr("Select data file"));
    fileDialog.setNameFilter(tr("*.* (*.*)"));

    if (fileDialog.exec() == QDialog::Accepted)
    {
        _pUi->lineDataFile->setText(fileDialog.selectedFiles().first());

        setPreset(_pUi->lineDataFile->text());
    }
}

void LoadFileDialog::presetSelected(int index)
{
    if ((index >= 0) && (index < _presetList.size()))
    {

        if (_presetList[index].bColumn)
        {
            _pUi->spinColumn->setValue(_presetList[index].column);
        }

        if (_presetList[index].bDataRow)
        {
            _pUi->spinDataRow->setValue(_presetList[index].dataRow);
        }

        if (_presetList[index].labelRow == -1)
        {
            _pUi->spinLabelRow->setValue(0);
            _pUi->checkLabelRow->setChecked(false);
        }
        else
        {
            _pUi->spinLabelRow->setValue(_presetList[index].labelRow);
            _pUi->checkLabelRow->setChecked(true);
        }

        if (_presetList[index].bDecimalSeparator)
        {
            _pUi->comboDecimalSeparator->setCurrentIndex(findIndexInCombo(_groupSeparatorList, _presetList[index].decimalSeparator));
        }

        if (_presetList[index].bFieldSeparator)
        {
            // DIRTY DIRTY workaround for tab input problem
            if (
                (_presetList[index].fieldSeparator.at(0) == '\\')
                && (_presetList[index].fieldSeparator.at(1) == 't')
                )
            {
                _presetList[index].fieldSeparator = QString("\t");
            }

            qint32 comboIndex = findIndexInCombo(_fieldSeparatorList, _presetList[index].fieldSeparator);

            if (comboIndex == -1)
            {
                _pUi->comboFieldSeparator->setCurrentIndex(findIndexInCombo(_fieldSeparatorList, "custom"));
                _pUi->lineCustomFieldSeparator->setText(_presetList[index].fieldSeparator);
            }
            else
            {
                _pUi->comboFieldSeparator->setCurrentIndex(comboIndex);
            }
        }

        if (_presetList[index].bThousandSeparator)
        {
            _pUi->comboGroupSeparator->setCurrentIndex(findIndexInCombo(_groupSeparatorList, _presetList[index].thousandSeparator));
        }

        _pUi->checkDynamicSession->setChecked(_presetList[index].bDynamicSession);
    }

}


void LoadFileDialog::fieldSeparatorSelected(int index)
{
    if (_pUi->comboFieldSeparator->itemData(index).toString().toLower() == "custom")
    {
        _pUi->lineCustomFieldSeparator->setEnabled(true);
    }
    else
    {
        _pUi->lineCustomFieldSeparator->setEnabled(false);
    }
}


void LoadFileDialog::toggledLabelRow(bool bState)
{
    _pUi->spinLabelRow->setEnabled(bState);
}

void LoadFileDialog::done(int r)
{
    bool bValid = true;

    if(QDialog::Accepted == r)  // ok was pressed
    {
        // Validate the data
        bValid = validateData();
    }
    else
    {
        // cancel, close or exc was pressed
        bValid = true;
    }

    if (bValid)
    {
        QDialog::done(r);
    }
}

bool LoadFileDialog::validateData()
{
    bool bOk = true;

    if (bOk)
    {
        if (!QFileInfo(_pUi->lineDataFile->text()).exists())
        {
            bOk = false;
            Util::showError(tr("Data file doesn't exist"));
        }
    }

    if (bOk)
    {
        if (
                (_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
                && (_pUi->lineCustomFieldSeparator->text().isEmpty())
            )
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
    QString presetFile;
    /* Check if preset file exists (2 locations)
    *   <document_folder>\GraphViewer\
    *   directory of executable
    */
    QString documentsfolder = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();

    presetFile = documentsfolder + "/GraphViewer/" + _presetFilename;
    if (!QFileInfo(presetFile).exists())
    {
        // xml in documents folder doesn't exist, check directory of executable
        presetFile = _presetFilename;
        if (!QFileInfo(presetFile).exists())
        {
            presetFile = "";
            _lastModified = QDateTime();
        }
    }

    if (presetFile != "")
    {
        if (_lastModified != QFileInfo(presetFile).lastModified())
        {
            _lastModified = QFileInfo(presetFile).lastModified();

            _pUi->comboPreset->clear();

            QFile file(presetFile);

            /* If we can't open it, let's show an error message. */
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                PresetParser presetParser;
                _presetList.clear();
                if (presetParser.parseFile(&file, &_presetList))
                {
                    foreach(PresetParser::Preset preset, _presetList)
                    {
                        if (preset.bName)
                        {
                            _pUi->comboPreset->addItem(preset.name);
                        }
                        else
                        {
                            _pUi->comboPreset->addItem(QString("(%1)").arg(preset.keyword));
                        }
                    }
                }

                file.close();
            }
            else
            {
                QMessageBox::critical(this,
                                      "GraphViewer",
                                      tr("Couldn't open preset file: %1").arg(presetFile),
                                      QMessageBox::Ok);
            }
        }
    }
    else
    {
        _presetList.clear();
        _pUi->comboPreset->clear();
    }

}

void LoadFileDialog::setPreset(QString filename)
{
    // Loop through presets and set preset if keyword is in filename
    for (qint32 index = 0; index < _presetList.size(); index ++)
    {
        if (_presetList[index].bKeyword)
        {
            if (QFileInfo(filename).fileName().contains(_presetList[index].keyword, Qt::CaseInsensitive))
            {
                _pUi->comboPreset->setCurrentIndex(index);
                break;
            }
        }
    }
}

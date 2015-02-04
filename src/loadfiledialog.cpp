
#include <QFileDialog>

#include "util.h"
#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"

LoadFileDialog::LoadFileDialog(DataParserSettings *pParseSettings, QWidget *parent) :
    QDialog(parent),
    _pUi(new Ui::LoadFileDialog)
{
    _pUi->setupUi(this);

    _pParseSettings = pParseSettings;

    _pUi->comboFieldSeparator->addItem(" ; (semicolon)", ";");
    _pUi->comboFieldSeparator->addItem(" , (comma)", ",");
    _pUi->comboFieldSeparator->addItem(" tab", "\t");
    _pUi->comboFieldSeparator->addItem(" custom", "custom");
    if (QLocale::system().decimalPoint() == ',')
    {
        _pUi->comboFieldSeparator->setCurrentIndex(0);
    }
    else
    {
        _pUi->comboFieldSeparator->setCurrentIndex(1);
    }

    _pUi->comboPreset->addItem("Manual");
    _pUi->comboPreset->addItem("STMStudio");

    _pUi->comboDecimalSeparator->addItem(" , (comma)", ",");
    _pUi->comboDecimalSeparator->addItem(" . (point)", ".");
    if (QLocale::system().decimalPoint() == '.')
    {
        _pUi->comboDecimalSeparator->setCurrentIndex(1);
    }
    else
    {
        _pUi->comboDecimalSeparator->setCurrentIndex(0);
    }

    _pUi->comboGroupSeparator->addItem(" , (comma)", ",");
    _pUi->comboGroupSeparator->addItem(" . (point)", ".");
    _pUi->comboGroupSeparator->addItem("   (space)", " ");
    if (QLocale::system().groupSeparator() == ',')
    {
        _pUi->comboGroupSeparator->setCurrentIndex(0);
    }
    else if (QLocale::system().groupSeparator() == '.')
    {
        _pUi->comboGroupSeparator->setCurrentIndex(1);
    }
    else
    {
        _pUi->comboGroupSeparator->setCurrentIndex(2);
    }

    connect(_pUi->btnDataFile, SIGNAL(released()), this, SLOT(selectDataFile()));
    connect(_pUi->comboPreset, SIGNAL(currentIndexChanged(int)), this, SLOT(presetSelected(int)));
    connect(_pUi->comboFieldSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(fieldSeparatorSelected(int)));
}

LoadFileDialog::~LoadFileDialog()
{
    delete _pUi;
}

void LoadFileDialog::selectDataFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setOption(QFileDialog::HideNameFilterDetails, false);
    fileDialog.setWindowTitle(tr("Select data file"));
    fileDialog.setNameFilter(tr("*.* (*.*)"));

    if (fileDialog.exec())
    {
        _pParseSettings->setPath(fileDialog.selectedFiles().first());
        _pUi->lineDataFile->setText(QFileInfo(_pParseSettings->getPath()).fileName());
    }
}

void LoadFileDialog::presetSelected(int index)
{
    switch (index) {
    case 0:
        // Manual
        break;
    case 1:
        //Set STMStudio preset
        _pUi->spinColumn->setValue(2);
        _pUi->spinDataRow->setValue(9);
        _pUi->comboFieldSeparator->setCurrentIndex(2); // TODO: hardcoded
        _pUi->comboGroupSeparator->setCurrentIndex(0); // TODO: hardcoded
        _pUi->comboDecimalSeparator->setCurrentIndex(1); // TODO: hardcoded
        _pUi->spinLabelRow->setValue(7);
        break;
    default:
        break;
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

void LoadFileDialog::done(int r)
{
    bool bStop = true;

    if(QDialog::Accepted == r)  // ok was pressed
    {
        // Validate the data

        if (bStop)
        {
            if (!QFileInfo(_pParseSettings->getPath()).exists())
            {
                bStop = false;
                Util::showError(tr("Data file doesn't exist"));
            }
        }

        if (bStop)
        {
            if (
                    (_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
                    && (_pUi->lineCustomFieldSeparator->text().isEmpty())
                )
            {
                bStop = false;
                Util::showError(tr("Custom field separator isn't defined correctly"));
            }
        }

        if (bStop)
        {
            // path is already set
            _pParseSettings->setColumn(_pUi->spinColumn->value() - 1); // 1 based to 0 based
            _pParseSettings->setDataRow(_pUi->spinDataRow->value() - 1); // 1 based to 0 based
            _pParseSettings->setLabelRow(_pUi->spinLabelRow->value() - 1); // 1 based to 0 based

            if (_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
            {
                _pParseSettings->setFieldSeparator(_pUi->lineCustomFieldSeparator->text());
            }
            else
            {
                _pParseSettings->setFieldSeparator(_pUi->comboFieldSeparator->itemData(_pUi->comboFieldSeparator->currentIndex()).toString());
            }

            _pParseSettings->setDecimalSeparator(_pUi->comboDecimalSeparator->itemData(_pUi->comboDecimalSeparator->currentIndex()).toString());
            _pParseSettings->setGroupSeparator(_pUi->comboGroupSeparator->itemData(_pUi->comboGroupSeparator->currentIndex()).toString());
            _pParseSettings->setDynamicSession(_pUi->checkDynamicSession->checkState() == Qt::Checked ? true : false);
            _pParseSettings->setWatchFileData(true);
        }

    }
    else
    {
        // cancel, close or exc was pressed
        bStop = true;
    }

    if (bStop)
    {
        QDialog::done(r);
    }

}

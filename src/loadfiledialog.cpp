
#include <QFileDialog>

#include "util.h"
#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"

LoadFileDialog::LoadFileDialog(DataParserSettings *pParseSettings, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::LoadFileDialog)
{
    _ui->setupUi(this);

    _pParseSettings = pParseSettings;

    _ui->comboFieldSeparator->addItem(" ; (semicolon)", ";");
    _ui->comboFieldSeparator->addItem(" , (comma)", ",");
    _ui->comboFieldSeparator->addItem(" tab", "\t");
    _ui->comboFieldSeparator->addItem(" custom", "custom");
    if (QLocale::system().decimalPoint() == ',')
    {
        _ui->comboFieldSeparator->setCurrentIndex(0);
    }
    else
    {
        _ui->comboFieldSeparator->setCurrentIndex(1);
    }

    _ui->comboPreset->addItem("Manual");
    _ui->comboPreset->addItem("STMStudio");

    _ui->comboDecimalSeparator->addItem(" , (comma)", ",");
    _ui->comboDecimalSeparator->addItem(" . (point)", ".");
    if (QLocale::system().decimalPoint() == '.')
    {
        _ui->comboDecimalSeparator->setCurrentIndex(1);
    }
    else
    {
        _ui->comboDecimalSeparator->setCurrentIndex(0);
    }

    _ui->comboGroupSeparator->addItem(" , (comma)", ",");
    _ui->comboGroupSeparator->addItem(" . (point)", ".");
    _ui->comboGroupSeparator->addItem("   (space)", " ");
    if (QLocale::system().groupSeparator() == ',')
    {
        _ui->comboGroupSeparator->setCurrentIndex(0);
    }
    else if (QLocale::system().groupSeparator() == '.')
    {
        _ui->comboGroupSeparator->setCurrentIndex(1);
    }
    else
    {
        _ui->comboGroupSeparator->setCurrentIndex(2);
    }

    connect(_ui->btnDataFile, SIGNAL(released()), this, SLOT(selectDataFile()));
    connect(_ui->comboPreset, SIGNAL(currentIndexChanged(int)), this, SLOT(presetSelected(int)));
    connect(_ui->comboFieldSeparator, SIGNAL(currentIndexChanged(int)), this, SLOT(fieldSeparatorSelected(int)));
}

LoadFileDialog::~LoadFileDialog()
{
    delete _ui;
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
        _ui->lineDataFile->setText(QFileInfo(_pParseSettings->getPath()).fileName());
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
        _ui->spinColumn->setValue(2);
        _ui->spinDataRow->setValue(9);
        _ui->comboFieldSeparator->setCurrentIndex(2); // TODO: hardcoded
        _ui->comboGroupSeparator->setCurrentIndex(0); // TODO: hardcoded
        _ui->comboDecimalSeparator->setCurrentIndex(1); // TODO: hardcoded
        _ui->spinLabelRow->setValue(7);
        break;
    default:
        break;
    }
}


void LoadFileDialog::fieldSeparatorSelected(int index)
{
    if (_ui->comboFieldSeparator->itemData(index).toString().toLower() == "custom")
    {
        _ui->lineCustomFieldSeparator->setEnabled(true);
    }
    else
    {
        _ui->lineCustomFieldSeparator->setEnabled(false);
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
                    (_ui->comboFieldSeparator->itemData(_ui->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
                    && (_ui->lineCustomFieldSeparator->text().isEmpty())
                )
            {
                bStop = false;
                Util::showError(tr("Custom field separator isn't defined correctly"));
            }
        }

        if (bStop)
        {
            // path is already set
            _pParseSettings->setColumn(_ui->spinColumn->value() - 1); // 1 based to 0 based
            _pParseSettings->setDataRow(_ui->spinDataRow->value() - 1); // 1 based to 0 based
            _pParseSettings->setLabelRow(_ui->spinLabelRow->value() - 1); // 1 based to 0 based

            if (_ui->comboFieldSeparator->itemData(_ui->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
            {
                _pParseSettings->setFieldSeparator(_ui->lineCustomFieldSeparator->text());
            }
            else
            {
                _pParseSettings->setFieldSeparator(_ui->comboFieldSeparator->itemData(_ui->comboFieldSeparator->currentIndex()).toString());
            }

            _pParseSettings->setDecimalSeparator(_ui->comboDecimalSeparator->itemData(_ui->comboDecimalSeparator->currentIndex()).toString());
            _pParseSettings->setGroupSeparator(_ui->comboGroupSeparator->itemData(_ui->comboGroupSeparator->currentIndex()).toString());
            _pParseSettings->setDynamicSession(_ui->checkDynamicSession->checkState() == Qt::Checked ? true : false);
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

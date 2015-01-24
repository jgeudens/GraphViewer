
#include <QFileDialog>

#include "util.h"
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
    _ui->setupUi(this);

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

Dialog::~Dialog()
{
    delete _ui;
}

void Dialog::getDataSettings(DataTypes::DataFileSettings * pSettings)
{
    pSettings->path = _settings.path;
    pSettings->column = _ui->spinColumn->value() - 1; // 1 based to 0 based
    pSettings->dataRow = _ui->spinDataRow->value() - 1; // 1 based to 0 based
    pSettings->labelRow = _ui->spinLabelRow->value() - 1; // 1 based to 0 based

    if (_ui->comboFieldSeparator->itemData(_ui->comboFieldSeparator->currentIndex()).toString().toLower() == "custom")
    {
        pSettings->fieldSeparator = _ui->lineCustomFieldSeparator->text();
    }
    else
    {
        pSettings->fieldSeparator = _ui->comboFieldSeparator->itemData(_ui->comboFieldSeparator->currentIndex()).toString();
    }

    pSettings->decimalSeparator = _ui->comboDecimalSeparator->itemData(_ui->comboDecimalSeparator->currentIndex()).toString();
    pSettings->groupSeparator = _ui->comboGroupSeparator->itemData(_ui->comboGroupSeparator->currentIndex()).toString();
    pSettings->dynamicSession = (_ui->dynamicSession->checkState() == Qt::Checked ? true : false);
}

void Dialog::selectDataFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setOption(QFileDialog::HideNameFilterDetails, false);
    dialog.setWindowTitle(tr("Select data file"));
    dialog.setNameFilter(tr("*.* (*.*)"));

    if (dialog.exec())
    {
        _settings.path = dialog.selectedFiles().first();
        _ui->lineDataFile->setText(QFileInfo(_settings.path).fileName());
    }
}

void Dialog::presetSelected(int index)
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


void Dialog::fieldSeparatorSelected(int index)
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

void Dialog::done(int r)
{
    bool bStop = true;

    if(QDialog::Accepted == r)  // ok was pressed
    {
        // Validate the data

        if (bStop)
        {
            if (!QFileInfo(_settings.path).exists())
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

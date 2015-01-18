
#include <QFileDialog>

#include "util.h"
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
    _ui->setupUi(this);

    _ui->comboFieldSeparator->addItem(" ; (point comma)", ";");
    _ui->comboFieldSeparator->addItem(" , (comma)", ",");
    _ui->comboFieldSeparator->addItem(" tab", "\t");
    _ui->comboFieldSeparator->addItem(" custom", "custom");

    _ui->comboPreset->addItem("Manual");
    _ui->comboPreset->addItem("STMStudio");

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
    pSettings->dataColumn = _ui->spinDataColumn->value() - 1; // 1 based to 0 based
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
        _ui->spinDataColumn->setValue(2);
        _ui->spinDataRow->setValue(9);
        _ui->comboFieldSeparator->setCurrentIndex(2); // TODO: hardcoded
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

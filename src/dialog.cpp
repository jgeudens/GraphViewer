
#include <QFileDialog>

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Dialog)
{
    _ui->setupUi(this);

    connect(_ui->btnDataFile, SIGNAL(released()), this, SLOT(selectDataFile()));
}

Dialog::~Dialog()
{
    delete _ui;
}

void Dialog::getDataSettings(DataFileSettings * pSettings)
{
    pSettings->path = _settings.path;
    pSettings->dataColumn = _ui->spinDataColumn->value();
    pSettings->dataRow = _ui->spinDataRow->value();
    pSettings->fieldSeparator = _ui->lineFieldSeperator->text();
}

void Dialog::selectDataFile()
{
    QString filePath;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setOption(QFileDialog::HideNameFilterDetails, false);
    dialog.setWindowTitle(tr("Select data file"));
    dialog.setNameFilter(tr("*.* (*.*)"));

    if (dialog.exec())
    {
        _settings.path = dialog.selectedFiles().first();
    }
}

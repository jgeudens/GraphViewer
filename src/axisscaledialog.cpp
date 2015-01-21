#include "axisscaledialog.h"
#include "ui_axisscaledialog.h"

AxisScaleDialog::AxisScaleDialog(Axis axisType, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AxisScaleDialog)
{
    _ui->setupUi(this);

    _axisType = axisType;

    if (_axisType == AXIS_X)
    {
        setWindowTitle("Set min and max for x-axis");
        _ui->labelUnit->setVisible(true);
        _ui->comboUnit->setVisible(true);

        _ui->comboUnit->addItem("Milliseconds", 1);
        _ui->comboUnit->addItem("Seconds", 1000);
        _ui->comboUnit->addItem("Minutes", 60 * 1000);
        _ui->comboUnit->addItem("Hours", 60 * 60 * 1000);
    }
    else if (_axisType == AXIS_Y)
    {
        setWindowTitle("Set min and max for y-axis");
        _ui->labelUnit->setVisible(false);
        _ui->comboUnit->setVisible(false);
    }
    else
    {
        setWindowTitle("???");
    }
}

AxisScaleDialog::~AxisScaleDialog()
{
    delete _ui;
}

qint64 AxisScaleDialog::getMinimum()
{
    qint64 min = _ui->spinMinimum->text().toLongLong();
    if (_axisType == AXIS_X)
    {
        min = min * _ui->comboUnit->itemData(_ui->comboUnit->currentIndex()).toLongLong();
    }
    return min;
}


qint64 AxisScaleDialog::getMaximum()
{
    qint64 max = _ui->spinMaximum->text().toLongLong();
    if (_axisType == AXIS_X)
    {
        max = max * _ui->comboUnit->itemData(_ui->comboUnit->currentIndex()).toLongLong();
    }
    return max;
}

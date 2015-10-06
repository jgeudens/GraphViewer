#include "axisscaledialog.h"
#include "ui_axisscaledialog.h"

AxisScaleDialog::AxisScaleDialog(Axis axisType, QWidget *parent) :
    QDialog(parent),
    _pUi(new Ui::AxisScaleDialog)
{
    _pUi->setupUi(this);

    _axisType = axisType;

    if (_axisType == AXIS_X)
    {
        setWindowTitle("Set min and max for x-axis");
        _pUi->labelUnit->setVisible(true);
        _pUi->comboUnit->setVisible(true);

        _pUi->comboUnit->addItem("Milliseconds", 1);
        _pUi->comboUnit->addItem("Seconds", 1000);
        _pUi->comboUnit->addItem("Minutes", 60 * 1000);
        _pUi->comboUnit->addItem("Hours", 60 * 60 * 1000);
    }
    else if (_axisType == AXIS_Y)
    {
        setWindowTitle("Set min and max for y-axis");
        _pUi->labelUnit->setVisible(false);
        _pUi->comboUnit->setVisible(false);
    }
    else
    {
        setWindowTitle("???");
    }
}

AxisScaleDialog::~AxisScaleDialog()
{
    delete _pUi;
}

qint64 AxisScaleDialog::getMinimum()
{
    qint64 min = _pUi->spinMinimum->text().toLongLong();
    if (_axisType == AXIS_X)
    {
        min = min * _pUi->comboUnit->itemData(_pUi->comboUnit->currentIndex()).toLongLong();
    }
    return min;
}


qint64 AxisScaleDialog::getMaximum()
{
    qint64 max = _pUi->spinMaximum->text().toLongLong();
    if (_axisType == AXIS_X)
    {
        max = max * _pUi->comboUnit->itemData(_pUi->comboUnit->currentIndex()).toLongLong();
    }
    return max;
}

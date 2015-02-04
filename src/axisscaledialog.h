#ifndef AXISSCALEDIALOG_H
#define AXISSCALEDIALOG_H

#include <QDialog>

namespace Ui {
class AxisScaleDialog;
}

class AxisScaleDialog : public QDialog
{
    Q_OBJECT

public:

    typedef enum
    {
        AXIS_X = 0,
        AXIS_Y,
    } Axis;

    explicit AxisScaleDialog(Axis axisType, QWidget *parent = 0);
    ~AxisScaleDialog();

    qint64 getMinimum();
    qint64 getMaximum();

private:
    Ui::AxisScaleDialog * _pUi;

    Axis _axisType;
};

#endif // AXISSCALEDIALOG_H

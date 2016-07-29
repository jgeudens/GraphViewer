#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QColor>
#include "qcustomplot.h"

class GraphData
{

public:
    explicit GraphData();

    bool isVisible() const;
    void setVisible(bool bVisible);

    QString label() const;
    void setLabel(const QString &label);

    QColor color() const;
    void setColor(const QColor &color);

    bool isActive() const;
    void setActive(bool bActive);

    QCPDataMap * dataMap();

private:

    bool _bVisible;
    QString _label;
    QColor _color;
    bool _bActive;

    QCPDataMap _dataMap;

};

#endif // GRAPHDATA_H

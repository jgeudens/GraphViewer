#include "graphdata.h"

GraphData::GraphData()
{
    _bVisible = true;
    _label = QString("Unknown register");
    _color = "-1"; // Invalid color

    _dataMap.clear();
}

bool GraphData::isVisible() const
{
    return _bVisible;
}

void GraphData::setVisible(bool bVisible)
{
    _bVisible = bVisible;
}

QString GraphData::label() const
{
    return _label;
}

void GraphData::setLabel(const QString &label)
{
    _label = label;
}

QColor GraphData::color() const
{
    return _color;
}

void GraphData::setColor(const QColor &color)
{
    _color = color;
}

QCPDataMap * GraphData::dataMap()
{
    return &_dataMap;
}

bool GraphData::isActive() const
{
    return _bActive;
}

void GraphData::setActive(bool bActive)
{
    _bActive = bActive;
}

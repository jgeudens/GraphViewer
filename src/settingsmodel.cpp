#include <QColor>
#include "settingsmodel.h"


const QString SettingsModel::_cWindowTitle = QString("GraphViewer");
const QList<QColor> SettingsModel::_colorlist = QList<QColor>() << QColor(0, 0, 0)
                                                           << QColor(0, 0, 255)
                                                           << QColor(0, 255, 255)
                                                           << QColor(0, 255, 0)
                                                           << QColor(220, 220, 0)
                                                           << QColor(220, 153, 14)
                                                           << QColor(255, 165, 0)
                                                           << QColor(255, 0, 0)
                                                           << QColor(255, 160, 122)
                                                           << QColor(230, 104, 86)
                                                           << QColor(205, 205, 180)
                                                           << QColor(157, 153, 120)
                                                           << QColor(139, 69, 19)
                                                           << QColor(255, 20, 147)
                                                           << QColor(74, 112, 139)
                                                           << QColor(46, 139, 87)
                                                           << QColor(128, 0, 128)
                                                           << QColor(189, 183, 107)
                                                           ;

SettingsModel::SettingsModel(QObject *parent) : QObject(parent)
{
    _graphData.clear();
    _frontGraph = 0;
    _loadedFile = "";
    _bWatchFile = false;
    _bDynamicSession = false;
    _bHighlightSamples = true;
    _bValueTooltip = false;
    _bLegendVisibility = true;
}

SettingsModel::~SettingsModel()
{

}

void SettingsModel::triggerUpdate(void)
{
    if (_graphData.size() > 0)
    {
        for (quint8 idx = 0; idx < _graphData.size(); idx++)
        {
            emit graphVisibilityChanged(idx);
        }
    }

    emit frontGraphChanged();
    emit highlightSamplesChanged();
    emit valueTooltipChanged();
    emit windowTitleChanged();
    emit watchFileChanged();
    emit dynamicSessionChanged();
}

void SettingsModel::addGraphs(QStringList labels, QList<QList<double> > data)
{

    for (qint32 idx = 1; idx < labels.size(); idx++)
    {
        GraphData * pGraphData = new GraphData();

        pGraphData->bVisibility = true;
        pGraphData->label = labels[idx];

        pGraphData->color = getColor(idx);


        _graphData.append(pGraphData);
    }

    emit graphsAdded(data);
}

void SettingsModel::clearGraph()
{
    _graphData.clear();
    setFrontGraph(-1);
    emit graphCleared();
}

quint32 SettingsModel::graphCount()
{
    if (_graphData.size() > 0)
    {
        return _graphData.size();
    }
    else
    {
        return 0;
    }
}

bool SettingsModel::graphVisibility(quint32 index) const
{
    return _graphData[index]->bVisibility;
}

QColor SettingsModel::graphColor(quint32 index) const
{
    return _graphData[index]->color;
}

QString SettingsModel::graphLabel(quint32 index) const
{
    return _graphData[index]->label;
}

void SettingsModel::setGraphVisibility(quint32 index, const bool &value)
{
    if (_graphData[index]->bVisibility != value)
    {
         _graphData[index]->bVisibility = value;
         emit graphVisibilityChanged(index);
    }
}

qint32 SettingsModel::frontGraph() const
{
    return _frontGraph;
}

void SettingsModel::setFrontGraph(const qint32 &frontGraph)
{
    if (_frontGraph != frontGraph)
    {
        _frontGraph = frontGraph;

        if (frontGraph != -1)
        {
            emit frontGraphChanged();
        }
    }
}

QString SettingsModel::loadedFile() const
{
    return _loadedFile;
}

void SettingsModel::setLoadedFile(const QString &loadedFile)
{
    if (_loadedFile != loadedFile)
    {
        _loadedFile = loadedFile;
         emit loadedFileChanged();
    }
}

bool SettingsModel::watchFile() const
{
    return _bWatchFile;
}

void SettingsModel::setWatchFile(bool bWatchFile)
{
    if (_bWatchFile != bWatchFile)
    {
        _bWatchFile = bWatchFile;
        emit watchFileChanged();
    }
}

bool SettingsModel::dynamicSession() const
{
    return _bDynamicSession;
}

void SettingsModel::setDynamicSession(bool bDynamicSession)
{
    if (_bDynamicSession != bDynamicSession)
    {
        _bDynamicSession = bDynamicSession;
        emit dynamicSessionChanged();
    }
}

bool SettingsModel::highlightSamples() const
{
    return _bHighlightSamples;
}

void SettingsModel::setHighlightSamples(bool bHighlightSamples)
{
    if (_bHighlightSamples != bHighlightSamples)
    {
        _bHighlightSamples = bHighlightSamples;
         emit highlightSamplesChanged();
    }
}

bool SettingsModel::valueTooltip() const
{
    return _bValueTooltip;
}

void SettingsModel::setValueTooltip(bool bValueTooltip)
{
    if (_bValueTooltip != bValueTooltip)
    {
        _bValueTooltip = bValueTooltip;
         emit valueTooltipChanged();
    }
}

QColor SettingsModel::getColor(const quint32 index)
{
    const quint32 colorIndex = index % _colorlist.size();
    return _colorlist[colorIndex];
}

QString SettingsModel::windowTitle()
{
    return _windowTitle;
}

void SettingsModel::setWindowTitleDetail(QString detail)
{
    QString tmpTitle;
    if (detail == "")
    {
        tmpTitle = _cWindowTitle;
    }
    else
    {
        tmpTitle = QString(tr("%1 - %2")).arg(_cWindowTitle).arg(detail);
    }

    if (tmpTitle != _windowTitle)
    {
        _windowTitle = tmpTitle;
        emit windowTitleChanged();
    }
}

bool SettingsModel::legendVisibility()
{
    return _bLegendVisibility;
}

void SettingsModel::setLegendVisibility(bool bLegendVisibility)
{
    if (_bLegendVisibility != bLegendVisibility)
    {
        _bLegendVisibility = bLegendVisibility;
         emit legendVisibilityChanged();
    }
}

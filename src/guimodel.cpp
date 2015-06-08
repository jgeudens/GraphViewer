#include <QColor>
#include "guimodel.h"


const QString GuiModel::_cWindowTitle = QString("GraphViewer");
const QList<QColor> GuiModel::_colorlist = QList<QColor>() << QColor(0, 0, 0)
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

GuiModel::GuiModel(QObject *parent) : QObject(parent)
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

GuiModel::~GuiModel()
{

}

void GuiModel::triggerUpdate(void)
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

void GuiModel::addGraphs(QStringList labels, QList<QList<double> > data)
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

void GuiModel::clearGraph()
{
    _graphData.clear();
    setFrontGraph(-1);
    emit graphCleared();
}

quint32 GuiModel::graphCount()
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

bool GuiModel::graphVisibility(quint32 index) const
{
    return _graphData[index]->bVisibility;
}

QColor GuiModel::graphColor(quint32 index) const
{
    return _graphData[index]->color;
}

QString GuiModel::graphLabel(quint32 index) const
{
    return _graphData[index]->label;
}

void GuiModel::setGraphVisibility(quint32 index, const bool &value)
{
    if (_graphData[index]->bVisibility != value)
    {
         _graphData[index]->bVisibility = value;
         emit graphVisibilityChanged(index);
    }
}

qint32 GuiModel::frontGraph() const
{
    return _frontGraph;
}

void GuiModel::setFrontGraph(const qint32 &frontGraph)
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

QString GuiModel::loadedFile() const
{
    return _loadedFile;
}

void GuiModel::setLoadedFile(const QString &loadedFile)
{
    if (_loadedFile != loadedFile)
    {
        _loadedFile = loadedFile;
         emit loadedFileChanged();
    }
}

bool GuiModel::watchFile() const
{
    return _bWatchFile;
}

void GuiModel::setWatchFile(bool bWatchFile)
{
    if (_bWatchFile != bWatchFile)
    {
        _bWatchFile = bWatchFile;
        emit watchFileChanged();
    }
}

bool GuiModel::dynamicSession() const
{
    return _bDynamicSession;
}

void GuiModel::setDynamicSession(bool bDynamicSession)
{
    if (_bDynamicSession != bDynamicSession)
    {
        _bDynamicSession = bDynamicSession;
        emit dynamicSessionChanged();
    }
}

bool GuiModel::highlightSamples() const
{
    return _bHighlightSamples;
}

void GuiModel::setHighlightSamples(bool bHighlightSamples)
{
    if (_bHighlightSamples != bHighlightSamples)
    {
        _bHighlightSamples = bHighlightSamples;
         emit highlightSamplesChanged();
    }
}

bool GuiModel::valueTooltip() const
{
    return _bValueTooltip;
}

void GuiModel::setValueTooltip(bool bValueTooltip)
{
    if (_bValueTooltip != bValueTooltip)
    {
        _bValueTooltip = bValueTooltip;
         emit valueTooltipChanged();
    }
}

QColor GuiModel::getColor(const quint32 index)
{
    const quint32 colorIndex = index % _colorlist.size();
    return _colorlist[colorIndex];
}

QString GuiModel::windowTitle()
{
    return _windowTitle;
}

void GuiModel::setWindowTitleDetail(QString detail)
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

bool GuiModel::legendVisibility()
{
    return _bLegendVisibility;
}

void GuiModel::setLegendVisibility(bool bLegendVisibility)
{
    if (_bLegendVisibility != bLegendVisibility)
    {
        _bLegendVisibility = bLegendVisibility;
         emit legendVisibilityChanged();
    }
}

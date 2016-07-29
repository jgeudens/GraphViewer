#include <QColor>
#include "util.h"
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
    _frontGraph = 0;
    _dataFilePath = "";

    QStringList docPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if (docPath.size() > 0)
    {
        _lastDir = docPath[0];
    }

    _bWatchFile = false;
    _bHighlightSamples = true;
    _bValueTooltip = false;
    _bStartMarkerState = false;
    _startMarkerPos = 0;

    _bEndMarkerState = false;
    _endMarkerPos = 0;

    _bMarkerState = false;
}

GuiModel::~GuiModel()
{

}

void GuiModel::triggerUpdate(void)
{
    emit frontGraphChanged();
    emit highlightSamplesChanged();
    emit valueTooltipChanged();
    emit windowTitleChanged();
    emit watchFileChanged();
    emit xAxisScalingChanged();
    emit yAxisScalingChanged();
    emit guiStateChanged();
    emit dataFilePathChanged();

    emit markerStateChanged();
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

QString GuiModel::dataFilePath()
{
    return _dataFilePath;
}

void GuiModel::setDataFilePath(QString path)
{
    if (_dataFilePath != path)
    {   
        _dataFilePath = path;
        emit dataFilePathChanged();
    }
}

void GuiModel::setLastDir(QString dir)
{
    _lastDir = dir;
}

QString GuiModel::lastDir()
{
    return _lastDir;
}

void GuiModel::setxAxisScale(BasicGraphView::AxisScaleOptions scaleMode)
{
    if (_guiSettings.xScaleMode != scaleMode)
    {
        _guiSettings.xScaleMode = scaleMode;
        emit xAxisScalingChanged();
    }
}

BasicGraphView::AxisScaleOptions GuiModel::xAxisScalingMode()
{
    return _guiSettings.xScaleMode;
}


BasicGraphView::AxisScaleOptions GuiModel::yAxisScalingMode()
{
    return _guiSettings.yScaleMode;
}

void GuiModel::setyAxisScale(BasicGraphView::AxisScaleOptions scaleMode)
{
    // We only allow manual or auto
    if (
            (scaleMode != BasicGraphView::SCALE_MANUAL)
            && (scaleMode != BasicGraphView::SCALE_AUTO)
        )
    {
        scaleMode = BasicGraphView::SCALE_AUTO;
        qDebug() << "Unsupported y axis scaling selected";
    }

    if (_guiSettings.yScaleMode != scaleMode)
    {
        _guiSettings.yScaleMode = scaleMode;
        emit yAxisScalingChanged();
    }
}

void GuiModel::setGuiState(quint32 state)
{
    if (_guiState != state)
    {
        _guiState = state;
        emit guiStateChanged();
    }
}

quint32 GuiModel::guiState()
{
    return _guiState;
}


double GuiModel::startMarkerPos()
{
    return _startMarkerPos;
}

double GuiModel::endMarkerPos()
{
    return _endMarkerPos;
}

bool GuiModel::markerState()
{
    return _bMarkerState;
}


void GuiModel::clearMarkersState(void)
{
    setStartMarkerState(false);
    setEndMarkerState(false);
}

void GuiModel::setStartMarkerPos(double pos)
{
    if (
            (_startMarkerPos != pos)
            || (!_bStartMarkerState)
        )
    {
        if (
            (!_bEndMarkerState)
            || (pos != _endMarkerPos)
        )
        {
            setStartMarkerState(true);
            _startMarkerPos = pos;

            emit startMarkerPosChanged();
        }
    }
}

void GuiModel::setEndMarkerPos(double pos)
{
    if (
            (_endMarkerPos != pos)
            || (!_bEndMarkerState)
        )
    {
        if (
            (!_bStartMarkerState)
            || (pos != _startMarkerPos)
        )
        {
            setEndMarkerState(true);
            _endMarkerPos = pos;

            emit endMarkerPosChanged();
        }
    }
}


void GuiModel::setStartMarkerState(bool bState)
{
    if (_bStartMarkerState != bState)
    {
        _bStartMarkerState = bState;

        if (_bStartMarkerState && _bEndMarkerState)
        {
            setMarkerState(true);
        }

        if (!_bStartMarkerState && !_bEndMarkerState)
        {
             setMarkerState(false);
        }
    }
}

void GuiModel::setEndMarkerState(bool bState)
{
    if (_bEndMarkerState != bState)
    {
        _bEndMarkerState = bState;

        if (_bStartMarkerState && _bEndMarkerState)
        {
            setMarkerState(true);
        }

        if (!_bStartMarkerState && !_bEndMarkerState)
        {
            setMarkerState(false);
        }
    }
}

void GuiModel::setMarkerState(bool bState)
{
    if (_bMarkerState != bState)
    {
        _bMarkerState = bState;

        emit markerStateChanged();
    }
}


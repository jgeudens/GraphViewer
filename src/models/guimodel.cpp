#include <QColor>
#include <QStringList>
#include <QList>
#include "util.h"
#include "guimodel.h"


const QString GuiModel::_cWindowTitle = QString("GraphViewer");

const quint32 GuiModel::cDifferenceMask    = 1 << 0;
const quint32 GuiModel::cSlopeMask         = 1 << 1;
const quint32 GuiModel::cAverageMask       = 1 << 2;
const quint32 GuiModel::cMinimumMask       = 1 << 3;
const quint32 GuiModel::cMaximumMask       = 1 << 4;
const quint32 GuiModel::cCustomMask        = 1 << 5;

const QStringList GuiModel::cMarkerExpressionStrings = QStringList()
                                                        <<  "Diff: %0\n"
                                                        <<  "Slope: %0\n"
                                                        <<  "Avg: %0\n"
                                                        <<  "Min: %0\n"
                                                        <<  "Max: %0\n"
                                                        <<  "Custom: %0\n";

const QList<quint32> GuiModel::cMarkerExpressionBits = QList<quint32>()
                        << GuiModel::cDifferenceMask
                        << GuiModel::cSlopeMask
                        << GuiModel::cAverageMask
                        << GuiModel::cMinimumMask
                        << GuiModel::cMaximumMask
                        << GuiModel::cCustomMask
                        ;
const QString GuiModel::cMarkerExpressionStart = QString("y1: %0\n");
const QString GuiModel::cMarkerExpressionEnd = QString("y2: %0\n");

GuiModel::GuiModel(QObject *parent) : QObject(parent)
{
    _frontGraph = 0;
    _dataFilePath = "";
    _bHighlightSamples = true;
    _bCursorValues = false;
    _guiState = INIT;
    _windowTitle = _cWindowTitle;

    QStringList docPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if (docPath.size() > 0)
    {
        _lastDir = docPath[0];
    }

    _guiSettings.xScaleMode = BasicGraphView::SCALE_AUTO;
    _guiSettings.yScaleMode = BasicGraphView::SCALE_AUTO;

    _bWatchFile = false;
    _bStartMarkerState = false;
    _startMarkerPos = 0;

    _bEndMarkerState = false;
    _endMarkerPos = 0;

    _bMarkerState = false;

    _markerExpressionMask = cDifferenceMask;
    _markerExpressionCustomScript = "";
}

GuiModel::~GuiModel()
{

}

void GuiModel::triggerUpdate(void)
{
    emit frontGraphChanged();
    emit highlightSamplesChanged();
    emit cursorValuesChanged();
    emit windowTitleChanged();
    emit watchFileChanged();
    emit xAxisScalingChanged();
    emit yAxisScalingChanged();
    emit guiStateChanged();
    emit dataFilePathChanged();

    emit markerStateChanged();
    emit markerExpressionMaskChanged();
    emit markerExpressionCustomScriptChanged();
}

/*
 Return index of activeGraphList */
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

bool GuiModel::cursorValues() const
{
    return _bCursorValues;
}

void GuiModel::setCursorValues(bool bCursorValues)
{
    if (_bCursorValues != bCursorValues)
    {
        _bCursorValues = bCursorValues;
         emit cursorValuesChanged();
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

quint32 GuiModel::markerExpressionMask()
{
    return _markerExpressionMask;
}

void GuiModel::setMarkerExpressionMask(quint32 mask)
{
    if (_markerExpressionMask != mask)
    {
        _markerExpressionMask = mask;

        emit markerExpressionMaskChanged();
    }
}

QString GuiModel::markerExpressionCustomScript()
{
    return _markerExpressionCustomScript;
}

void GuiModel::setMarkerExpressionCustomScript(QString path)
{
    if (_markerExpressionCustomScript != path)
    {
        _markerExpressionCustomScript = path;

        emit markerExpressionCustomScriptChanged();
    }
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
    // Always send signal, because we also need to clear markers when only one is visible */
    _bMarkerState = bState;

    emit markerStateChanged();
}


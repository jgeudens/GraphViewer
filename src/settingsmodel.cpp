#include "settingsmodel.h"

SettingsModel::SettingsModel(QObject *parent) : QObject(parent)
{
    _graphVisibility.clear();
    _frontGraph = 0;
    _loadedFile = "";
    _bWatchFile = false;
    _bDynamicSession = false;
    _bHighlightSamples = true;
    _bValueTooltip = false;
}

SettingsModel::~SettingsModel()
{

}

void SettingsModel::triggerUpdate(void)
{
    emit settingsChanged();

    for (quint8 idx = 0; idx < _graphVisibility.size(); idx++)
    {
        emit graphVisibilityChanged(idx);
    }

    emit frontGraphChanged();
    emit loadedFileChanged();
    emit highlightSamplesChanged();
    emit valueTooltipChanged();
}

bool SettingsModel::GraphVisibility(quint32 index) const
{
    return _graphVisibility[index];
}

void SettingsModel::setGraphVisibility(quint32 index, const bool &value)
{
    if (_graphVisibility[index] != value)
    {
         _graphVisibility[index] = value;
         emit graphVisibilityChanged(index);
    }
}

quint32 SettingsModel::frontGraph() const
{
    return _frontGraph;
}

void SettingsModel::setFrontGraph(const quint32 &frontGraph)
{
    if ( _frontGraph != frontGraph)
    {
        _frontGraph = frontGraph;
         emit frontGraphChanged();
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

bool SettingsModel::WatchFile() const
{
    return _bWatchFile;
}

void SettingsModel::setWatchFile(bool bWatchFile)
{
    if (_bWatchFile != bWatchFile)
    {
        _bWatchFile = bWatchFile;
         emit settingsChanged();
    }
}

bool SettingsModel::DynamicSession() const
{
    return _bDynamicSession;
}

void SettingsModel::setDynamicSession(bool bDynamicSession)
{
    if (_bDynamicSession != bDynamicSession)
    {
        _bDynamicSession = bDynamicSession;
         emit settingsChanged();
    }
}

bool SettingsModel::HighlightSamples() const
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

bool SettingsModel::ValueTooltip() const
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









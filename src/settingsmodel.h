#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>
#include "graphdata.h"

class SettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit SettingsModel(QObject *parent = 0);
    ~SettingsModel();

    void triggerUpdate(void);

    bool graphVisibility(quint32 index) const;
    QColor graphColor(quint32 index) const;
    QString graphLabel(quint32 index) const;

    quint32 frontGraph() const;
    QString loadedFile() const;
    void setLoadedFile(const QString &loadedFile);
    bool watchFile() const;
    bool dynamicSession() const;
    bool highlightSamples() const;
    bool valueTooltip() const;
    void addGraphs(QStringList labels, QList<QList<double> > data);
    void clearGraph();
    quint32 graphCount();

public slots:
    void setValueTooltip(bool bValueTooltip);
    void setHighlightSamples(bool bHighlightSamples);
    void setDynamicSession(bool bDynamicSession);
    void setWatchFile(bool bWatchFile);
    void setFrontGraph(const quint32 &frontGraph);
    void setGraphVisibility(quint32 index, const bool &value);

signals:
    void settingsChanged();

    void graphVisibilityChanged(const quint32 index);
    void graphCleared();
    void graphsAdded(QList<QList<double> > data);

    void frontGraphChanged();
    void loadedFileChanged();
    void highlightSamplesChanged();
    void valueTooltipChanged();


public slots:


private:

    QColor getColor(const quint32 index);

    QList<GraphData * > _graphData;

    quint32 _frontGraph;

    QString _loadedFile;

    bool _bWatchFile;
    bool _bDynamicSession;

    bool _bHighlightSamples;
    bool _bValueTooltip;

    static const QList<QColor> _colorlist;

};

#endif // SETTINGSMODEL_H

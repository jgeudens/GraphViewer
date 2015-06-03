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

    qint32 frontGraph() const;
    QString loadedFile() const;
    void setLoadedFile(const QString &loadedFile);
    bool watchFile() const;
    bool dynamicSession() const;
    bool highlightSamples() const;
    bool valueTooltip() const;
    void addGraphs(QStringList labels, QList<QList<double> > data);
    void clearGraph();
    quint32 graphCount();
    QString windowTitle();
    bool legendVisibility();

public slots:
    void setValueTooltip(bool bValueTooltip);
    void setHighlightSamples(bool bHighlightSamples);
    void setDynamicSession(bool bDynamicSession);
    void setWatchFile(bool bWatchFile);
    void setFrontGraph(const qint32 &frontGraph);
    void setGraphVisibility(quint32 index, const bool &value);
    void setWindowTitleDetail(QString detail);
    void setLegendVisibility(bool bLegendVisibility);

signals:

    void graphVisibilityChanged(const quint32 index);
    void graphCleared();
    void graphsAdded(QList<QList<double> > data);
    void frontGraphChanged();
    void loadedFileChanged();
    void highlightSamplesChanged();
    void valueTooltipChanged();
    void windowTitleChanged();
    void watchFileChanged();
    void dynamicSessionChanged();
    void legendVisibilityChanged();

public slots:


private:

    QColor getColor(const quint32 index);

    QList<GraphData * > _graphData;

    qint32 _frontGraph;

    QString _loadedFile;
    QString _windowTitle;

    bool _bWatchFile;
    bool _bDynamicSession;

    bool _bHighlightSamples;
    bool _bValueTooltip;
    bool _bLegendVisibility;

    static const QString _cWindowTitle;
    static const QList<QColor> _colorlist;

};

#endif // SETTINGSMODEL_H

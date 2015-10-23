#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include "graphdata.h"
#include "basicgraphview.h"

class GuiModel : public QObject
{
    Q_OBJECT
public:
    explicit GuiModel(QObject *parent = 0);
    ~GuiModel();

    void triggerUpdate(void);

    bool graphVisibility(quint32 index) const;
    QColor graphColor(quint32 index) const;
    QString graphLabel(quint32 index) const;

    qint32 frontGraph() const;
    QString loadedFile() const;
    void setLoadedFile(const QString &loadedFile);
    bool watchFile() const;
    bool highlightSamples() const;
    bool valueTooltip() const;
    void addGraphs(QStringList labels, QList<QList<double> > data);
    void clearGraph();
    quint32 graphCount();
    QString windowTitle();
    bool legendVisibility();
    BasicGraphView::LegendsPositionOptions legendPosition();
    BasicGraphView::AxisScaleOptions xAxisScalingMode();
    BasicGraphView::AxisScaleOptions  yAxisScalingMode();
    void setxAxisScale(BasicGraphView::AxisScaleOptions scaleMode);
    void setyAxisScale(BasicGraphView::AxisScaleOptions scaleMode);

public slots:
    void setValueTooltip(bool bValueTooltip);
    void setHighlightSamples(bool bHighlightSamples);
    void setWatchFile(bool bWatchFile);
    void setFrontGraph(const qint32 &frontGraph);
    void setGraphVisibility(quint32 index, const bool &value);
    void setWindowTitleDetail(QString detail);
    void setLegendVisibility(bool bLegendVisibility);
    void setLegendPosition(BasicGraphView::LegendsPositionOptions pos);

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
    void legendVisibilityChanged();
    void legendPositionChanged();
    void xAxisScalingChanged();
    void yAxisScalingChanged();

public slots:


private:

    typedef struct
    {
        BasicGraphView::AxisScaleOptions xScaleMode;
        BasicGraphView::AxisScaleOptions yScaleMode;

    } GuiSettings;

    QColor getColor(const quint32 index);

    QList<GraphData * > _graphData;

    qint32 _frontGraph;
    GuiSettings _guiSettings;

    QString _loadedFile;
    QString _windowTitle;

    bool _bWatchFile;

    bool _bHighlightSamples;
    bool _bValueTooltip;
    bool _bLegendVisibility;
    BasicGraphView::LegendsPositionOptions _legendPosition;

    static const QString _cWindowTitle;
    static const QList<QColor> _colorlist;

};

#endif // GUIMODEL_H

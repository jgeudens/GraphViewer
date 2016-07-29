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

    enum
    {
        INIT,
        DATA_LOADED,
    };

    void triggerUpdate(void);

    qint32 frontGraph() const;
    bool watchFile() const;
    bool highlightSamples() const;
    bool valueTooltip() const;
    QString windowTitle();
    QString dataFilePath();
    QString lastDir();
    BasicGraphView::AxisScaleOptions xAxisScalingMode();
    BasicGraphView::AxisScaleOptions  yAxisScalingMode();
    void setxAxisScale(BasicGraphView::AxisScaleOptions scaleMode);
    void setyAxisScale(BasicGraphView::AxisScaleOptions scaleMode);
    quint32 guiState();
    double startMarkerPos();
    double endMarkerPos();
    bool markerState();

    void setDataFilePath(QString path);
    void setLastDir(QString dir);

public slots:
    void setValueTooltip(bool bValueTooltip);
    void setHighlightSamples(bool bHighlightSamples);
    void setWatchFile(bool bWatchFile);
    void setFrontGraph(const qint32 &frontGraph);

    void setWindowTitleDetail(QString detail);
    void setGuiState(quint32 state);
    void clearMarkersState(void);
    void setStartMarkerPos(double pos);
    void setEndMarkerPos(double pos);

signals:

    void graphVisibilityChanged(const quint32 index);
    void graphCleared();
    void graphsAdded(QList<QList<double> > data);
    void frontGraphChanged();
    void highlightSamplesChanged();
    void valueTooltipChanged();
    void windowTitleChanged();
    void watchFileChanged();
    void xAxisScalingChanged();
    void yAxisScalingChanged();
    void guiStateChanged();
    void dataFilePathChanged();
    void markerStateChanged();
    void startMarkerPosChanged();
    void endMarkerPosChanged();

private slots:

private:

    void setStartMarkerState(bool bState);
    void setEndMarkerState(bool bState);
    void setMarkerState(bool bState);

    typedef struct
    {
        BasicGraphView::AxisScaleOptions xScaleMode;
        BasicGraphView::AxisScaleOptions yScaleMode;

    } GuiSettings;

    qint32 _frontGraph;
    GuiSettings _guiSettings;

    QString _windowTitle;

    bool _bWatchFile;
    QString _dataFilePath;
    QString _lastDir; // Last directory opened for import/export/load project

    bool _bHighlightSamples;
    bool _bValueTooltip;
    quint32 _guiState;

    bool _bMarkerState;
    bool _bStartMarkerState;
    double _startMarkerPos;

    bool _bEndMarkerState;
    double _endMarkerPos;

    static const QString _cWindowTitle;
    static const QList<QColor> _colorlist;

};

#endif // GUIMODEL_H

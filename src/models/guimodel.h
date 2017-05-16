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

    static const quint32 cDifferenceMask;
    static const quint32 cSlopeMask;
    static const quint32 cAverageMask;
    static const quint32 cMinimumMask;
    static const quint32 cMaximumMask;
    static const quint32 cCustomMask;

    static const QStringList cMarkerExpressionStrings;
    static const QList<quint32> cMarkerExpressionBits;

    static const QString cMarkerExpressionStart;
    static const QString cMarkerExpressionEnd;

    enum
    {
        INIT,
        DATA_LOADED,
    };

    void triggerUpdate(void);

    qint32 frontGraph() const;
    bool watchFile() const;
    bool highlightSamples() const;
    bool cursorValues() const;
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
    quint32 markerExpressionMask();
    QString markerExpressionCustomScript();

    void setDataFilePath(QString path);
    void setLastDir(QString dir);
    void setMarkerExpressionMask(quint32 mask);
    void setMarkerExpressionCustomScript(QString path);

public slots:
    void setCursorValues(bool bCursorValues);
    void setHighlightSamples(bool bHighlightSamples);
    void setWatchFile(bool bWatchFile);
    void setFrontGraph(const qint32 &frontGraph);

    void setWindowTitleDetail(QString detail);
    void setGuiState(quint32 state);
    void clearMarkersState(void);
    void setStartMarkerPos(double pos);
    void setEndMarkerPos(double pos);

signals:

    void frontGraphChanged();
    void highlightSamplesChanged();
    void cursorValuesChanged();
    void windowTitleChanged();
    void watchFileChanged();
    void xAxisScalingChanged();
    void yAxisScalingChanged();
    void guiStateChanged();
    void dataFilePathChanged();
    void markerStateChanged();
    void startMarkerPosChanged();
    void endMarkerPosChanged();
    void markerExpressionMaskChanged();
    void markerExpressionCustomScriptChanged();

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
    bool _bCursorValues;
    quint32 _guiState;

    bool _bMarkerState;
    bool _bStartMarkerState;
    double _startMarkerPos;

    bool _bEndMarkerState;
    double _endMarkerPos;

    quint32 _markerExpressionMask;
    QString _markerExpressionCustomScript;

    static const QString _cWindowTitle;

};

#endif // GUIMODEL_H

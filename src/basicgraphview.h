#ifndef BASICGRAPHVIEW_H
#define BASICGRAPHVIEW_H

#include <QObject>
#include "guimodel.h"
#include "qcustomplot.h"


class BasicGraphView : public QObject
{
    Q_OBJECT
public:
    explicit BasicGraphView(GuiModel *pGuiModel, QCustomPlot *pPlot, QObject *parent = 0);

    void exportGraphImage(QString imageFile);
    void manualScaleXAxis(qint64 min, qint64 max);
    void manualScaleYAxis(qint64 min, qint64 max);

public slots:
    void autoScaleXAxis();
    void autoScaleYAxis();

    void enableValueTooltip();
    void enableSamplePoints();
    void clearGraphs();
    void addGraphs();
    void showHideLegend();
    void showGraph(quint32 index);
    void bringToFront();

signals:

private slots:
    void generateTickLabels();
    void selectionChanged();

    void mousePress();
    void mouseWheel();
    void legendClick(QCPLegend * legend, QCPAbstractLegendItem * abstractLegendItem, QMouseEvent * event);
    void legendDoubleClick(QCPLegend * legend,QCPAbstractLegendItem * abstractLegendItem, QMouseEvent * event);

    void paintValueToolTip(QMouseEvent *event);
    void handleSamplePoints();
    void axisDoubleClicked(QCPAxis * axis);

protected:
    GuiModel * _pGuiModel;
    QCustomPlot * _pPlot;

private:
    QString createTickLabelString(qint32 tickKey);
    void highlightSamples(bool bState);
    qint32 getGraphIndex(QCPGraph * pGraph);

    bool _bEnableTooltip;
    bool _bEnableSampleHighlight;

    QVector<QString> tickLabels;

    static const qint32 _cPixelNearThreshold = 20; /* in pixels */
    static const qint32 _cPixelPerPointThreshold = 5; /* in pixels */


};

#endif // BASICGRAPHVIEW_H

#ifndef GraphViewer_H
#define GraphViewer_H

#include <QObject>
#include <QVector>
#include <QMouseEvent>

#include "qcustomplot.h"

class GraphViewer : public QObject
{
    Q_OBJECT
public:
    explicit GraphViewer(QCustomPlot * pPlot, QObject *parent);

    void clear();
    void setupGraph(QList<QList<double> > *pDataLists, QStringList *pLabels);
    void exportGraphImage(QString imageFile);
    void manualScaleXAxis(qint64 min, qint64 max);
    void manualScaleYAxis(qint64 min, qint64 max);
    void showGraph(quint32 index, bool bShow);
    void bringToFront(quint32 index, bool bFront);
    QColor getGraphColor(quint32 index);

public slots:
    void autoScaleXAxis();
    void autoScaleYAxis();
    void enableValueTooltip(bool bState);
    void enableSamplePoints(bool bState);

private slots:
    void generateTickLabels();
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void axisDoubleClicked(QCPAxis * axis);
    void showValueToolTip(QMouseEvent *event);

private:

    QString createTickLabelString(qint32 tickKey);

    QCustomPlot * _pPlot;

    bool _bEnableTooltip;

    static const QList<QColor> _colorlist;

    QVector<QString> tickLabels;

    static const qint32 _cPixelNearThreshold = 20; /* in pixels */

};

#endif // GraphViewer_H

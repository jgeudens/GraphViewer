#ifndef GraphViewer_H
#define GraphViewer_H

#include <QObject>
#include <QVector>
#include <QMouseEvent>

#include <settingsmodel.h>

#include "qcustomplot.h"

class GraphViewer : public QObject
{
    Q_OBJECT
public:
    explicit GraphViewer(SettingsModel * pSettingsModel, QCustomPlot * pPlot, QObject *parent);

    void clear();
    void setupData(QList<QList<double> > * pDataLists, QStringList * pLabels);
    void updateData(QList<QList<double> > * pDataLists);
    void exportGraphImage(QString imageFile);
    void manualScaleXAxis(qint64 min, qint64 max);
    void manualScaleYAxis(qint64 min, qint64 max);

    QColor getGraphColor(quint32 index);

public slots:
    void autoScaleXAxis();
    void autoScaleYAxis();
    void enableValueTooltip();
    void enableSamplePoints();

    void showGraph(quint32 index);
    void bringToFront();

private slots:
    void generateTickLabels();
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void axisDoubleClicked(QCPAxis * axis);
    void paintValueToolTip(QMouseEvent *event);
    void handleSamplePoints();

private:

    QString createTickLabelString(qint32 tickKey);
    void highlightSamples(bool bState);

    SettingsModel * _pSettingsModel;

    QCustomPlot * _pPlot;

    bool _bEnableTooltip;
    bool _bEnableSampleHighlight;

    static const QList<QColor> _colorlist;

    QVector<QString> tickLabels;

    static const qint32 _cPixelNearThreshold = 20; /* in pixels */
    static const qint32 _cPixelPerPointThreshold = 5; /* in pixels */

};

#endif // GraphViewer_H

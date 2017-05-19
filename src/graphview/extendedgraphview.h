#ifndef EXTENDEDGRAPHVIEW_H
#define EXTENDEDGRAPHVIEW_H

#include <QObject>
#include "basicgraphview.h"

class ExtendedGraphView : public BasicGraphView
{
    Q_OBJECT

public:
    ExtendedGraphView(GuiModel *pGuiModel, GraphDataModel * pRegisterDataModel, MyQCustomPlot *pPlot, QObject *parent);
    virtual ~ExtendedGraphView();

public slots:
    void addData(QList<double> timeData, QList<QList<double> > data);
    void showGraph(quint32 graphIdx);
    void rescalePlot();
    void clearResults();

signals:
    void dataAddedToPlot(double timeData, QList<double> dataList);

private slots:
    void updateData(QList<double> *pTimeData, QList<QList<double> > * pDataLists);

private:
    static const quint64 _cOptimizeThreshold = 1000000uL;

    qint32 _diffWithUtc;

};

#endif // EXTENDEDGRAPHVIEW_H

#ifndef EXTENDEDGRAPHVIEW_H
#define EXTENDEDGRAPHVIEW_H

#include <QObject>
#include "basicgraphview.h"

class ExtendedGraphView : public BasicGraphView
{
    Q_OBJECT

public:
    ExtendedGraphView(GuiModel *pGuiModel, QCustomPlot *pPlot, QObject *parent);
    virtual ~ExtendedGraphView();

    void updateData(QList<QList<double> > * pDataLists);

public slots:
    void addGraphs(QList<QList<double> > data);

};

#endif // EXTENDEDGRAPHVIEW_H

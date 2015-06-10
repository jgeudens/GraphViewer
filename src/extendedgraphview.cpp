#include "extendedgraphview.h"


ExtendedGraphView::ExtendedGraphView(GuiModel * pGuiModel, QCustomPlot * pPlot, QObject *parent):
    BasicGraphView(pGuiModel, pPlot)
{
    Q_UNUSED(parent);
}

ExtendedGraphView::~ExtendedGraphView()
{

}

void ExtendedGraphView::addGraphs(QList<QList<double> > data)
{
    BasicGraphView::addGraphs();

    updateData(&data);
}

void ExtendedGraphView::updateData(QList<QList<double> > * pDataLists)
{
    bool bFullScale = false;
    const QVector<double> timeData = pDataLists->at(0).toVector();

    if (_pPlot->graph(0)->data()->keys().size() > 0)
    {
        if (
        (_pPlot->xAxis->range().lower <= _pPlot->graph(0)->data()->keys().first())
        && (_pPlot->xAxis->range().upper >= _pPlot->graph(0)->data()->keys().last())
        )
        {
            bFullScale = true;
        }
    }
    else
    {
        /* First load of file: always rescale */
        bFullScale = true;
    }

    for (qint32 i = 1; i < pDataLists->size(); i++)
    {
        //Add data to graphs
        QVector<double> graphData = pDataLists->at(i).toVector();
        _pPlot->graph(i - 1)->setData(timeData, graphData);
    }

    if (bFullScale)
    {
        _pPlot->rescaleAxes(true);
    }
    _pPlot->replot();
}

#include "extendedgraphview.h"
#include "guimodel.h"


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

void ExtendedGraphView::rescalePlot()
{

    // scale x-axis
    if (_pGuiModel->xAxisScalingMode() == SCALE_AUTO)
    {
        if ((_pPlot->graphCount() != 0) && (_pPlot->graph(0)->data()->keys().size()))
        {
            _pPlot->xAxis->rescale();
        }
        else
        {
            _pPlot->xAxis->setRange(0, 10000);
        }
    }
    else // Manual
    {

    }

    // scale y-axis
    if (_pGuiModel->yAxisScalingMode() == SCALE_AUTO)
    {
        if ((_pPlot->graphCount() != 0) && (_pPlot->graph(0)->data()->keys().size()))
        {
            _pPlot->yAxis->rescale(true);
        }
        else
        {
            _pPlot->yAxis->setRange(0, 10);
        }
    }
    else // Manual
    {

    }

    _pPlot->replot();
}

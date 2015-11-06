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
    const QVector<double> timeData = pDataLists->at(0).toVector();

    quint64 totalPoints = 0;
    for (qint32 i = 1; i < pDataLists->size(); i++)
    {
        //Add data to graphs
        QVector<double> graphData = pDataLists->at(i).toVector();
        _pPlot->graph(i - 1)->setData(timeData, graphData);

        totalPoints += graphData.size();
    }

    // Check if optimizations are needed
    if (totalPoints > _cOptimizeThreshold)
    {
        _pGuiModel->setHighlightSamples(false);

        // Set width to 1
        for (qint32 i = 0; i <  _pPlot->graphCount(); i++)
        {
             _pPlot->graph(i)->pen().setWidth(1);
        }

        // Disable anti aliasing
        _pPlot->setNotAntialiasedElements(QCP::aeAll);
    }

    // Rescale only axis that are in auto scale mode
    if (
        (_pGuiModel->xAxisScalingMode() == SCALE_AUTO)
        && (_pGuiModel->xAxisScalingMode() == SCALE_AUTO)
    )
    {
        _pPlot->rescaleAxes(true);
    }
    else if (_pGuiModel->xAxisScalingMode() == SCALE_AUTO)
    {
        _pPlot->xAxis->rescale(true);
    }
    else if (_pGuiModel->yAxisScalingMode() == SCALE_AUTO)
    {
       _pPlot->yAxis->rescale(true);
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


#include "util.h"
#include "guimodel.h"
#include "graphdatamodel.h"
#include "extendedgraphview.h"

ExtendedGraphView::ExtendedGraphView(GuiModel * pGuiModel, GraphDataModel * pRegisterDataModel, MyQCustomPlot *pPlot, QObject *parent):
    BasicGraphView(pGuiModel, pRegisterDataModel, pPlot)
{
    Q_UNUSED(parent);
}

ExtendedGraphView::~ExtendedGraphView()
{

}

void ExtendedGraphView::addData(QList<double> timeData, QList<QList<double> > data)
{
    updateData(&timeData, &data);
}


void ExtendedGraphView::clearResults()
{
    for (qint32 i = 0; i < _pPlot->graphCount(); i++)
    {
        _pPlot->graph(i)->data()->clear();
        _pPlot->graph(i)->setName(QString("(-) %1").arg(_pGraphDataModel->label(i)));
    }

   rescalePlot();
}

void ExtendedGraphView::rescalePlot()
{

    // scale x-axis
    if (_pGuiModel->xAxisScalingMode() == SCALE_AUTO)
    {
        if ((_pPlot->graphCount() != 0) && (graphDataSize() != 0))
        {
            _pPlot->xAxis->rescale(true);
        }
        else
        {
            _pPlot->xAxis->setRange(0, 10000);
        }
    }
    else if (_pGuiModel->xAxisScalingMode() == SCALE_SLIDING)
    {
        _pPlot->xAxis->rescale(true);
    }
    else // Manual
    {

    }

    // scale y-axis
    if (_pGuiModel->yAxisScalingMode() == SCALE_AUTO)
    {
        if ((_pPlot->graphCount() != 0) && (graphDataSize()))
        {
            _pPlot->yAxis->rescale(true);
        }
        else
        {
            _pPlot->yAxis->setRange(0, 10);
        }
    }
    else if (_pGuiModel->yAxisScalingMode() == SCALE_MINMAX)
    {
        _pPlot->yAxis->rescale(true);
    }
    else if (_pGuiModel->yAxisScalingMode() == SCALE_WINDOW_AUTO)
    {
        _pPlot->yAxis->rescale(true);
    }
    else // Manual
    {

    }

    _pPlot->replot();
}

void ExtendedGraphView::showGraph(quint32 graphIdx)
{
    if (_pGraphDataModel->isActive(graphIdx))
    {
        const bool bShow = _pGraphDataModel->isVisible(graphIdx);

        const quint32 activeIdx = _pGraphDataModel->convertToActiveGraphIndex(graphIdx);

        _pPlot->graph(activeIdx)->setVisible(bShow);

        rescalePlot();
    }
}

void ExtendedGraphView::updateData(QList<double> *pTimeData, QList<QList<double> > * pDataLists)
{
    quint64 totalPoints = 0;
    const QVector<double> timeData = pTimeData->toVector();

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

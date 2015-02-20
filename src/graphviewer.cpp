
#include <QMessageBox>
#include <QVector>
#include <QLocale>

#include "graphviewer.h"

const QList<QColor> GraphViewer::_colorlist = QList<QColor>() << QColor("blue")
                                                           << QColor("green")
                                                           << QColor("red")
                                                           << QColor("orange")
                                                           << QColor("purple")
                                                           << QColor("black")
                                                           << QColor("yellow")
                                                           ;

GraphViewer::GraphViewer(QCustomPlot * pPlot, QObject *parent) :
   QObject(parent)
{

   _pPlot = pPlot;

   _pPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);

   // disable anti aliasing while dragging
   _pPlot->setNoAntialiasingOnDrag(true);

   /*
    * Greatly improves performance
    *
    * phFastPolylines	Graph/Curve lines are drawn with a faster method. This reduces the quality especially
    *                   of the line segment joins. (Only relevant for solid line pens.)
    * phForceRepaint	causes an immediate repaint() instead of a soft update() when QCustomPlot::replot()
    *                   is called with parameter QCustomPlot::rpHint. This is set by default to prevent the
    *                   plot from freezing on fast consecutive replots (e.g. user drags ranges with mouse).
    * phCacheLabels		axis (tick) labels will be cached as pixmaps, increasing replot performance.
    * */
   _pPlot->setPlottingHints(QCP::phCacheLabels | QCP::phFastPolylines | QCP::phForceRepaint);

   _pPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
   _pPlot->xAxis->setNumberFormat("gb");
   _pPlot->xAxis->setRange(0, 10000);
   _pPlot->xAxis->setAutoTicks(true);
   _pPlot->xAxis->setAutoTickLabels(false);
   _pPlot->xAxis->setLabel("Time (s)");

   connect(_pPlot->xAxis, SIGNAL(ticksRequest()), this, SLOT(generateTickLabels()));

   _pPlot->yAxis->setRange(0, 65535);

   _pPlot->legend->setVisible(false);
   QFont legendFont = QApplication::font();
   legendFont.setPointSize(10);
   _pPlot->legend->setFont(legendFont);

   // Add layer to move graph on front
   _pPlot->addLayer("topMain", _pPlot->layer("main"), QCustomPlot::limAbove);

   // connect slot that ties some axis selections together (especially opposite axes):
   connect(_pPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

   // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
   connect(_pPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
   connect(_pPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
   connect(_pPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisDoubleClicked(QCPAxis*)));
}

void GraphViewer::clear()
{
    _pPlot->clearGraphs();
}

void GraphViewer::setupGraph(QList<QList<double> > * pDataLists, QStringList * pLabels)
{
   const QVector<double> timeData = pDataLists->at(0).toVector();

   _pPlot->clearGraphs();

   for (qint32 i = 1; i < pDataLists->size(); i++)
   {
        const quint32 colorIndex = _pPlot->graphCount() % _colorlist.size();

        QCPGraph * pGraph = _pPlot->addGraph();

        //Add data to graphs
        QVector<double> graphData = pDataLists->at(i).toVector();
        _pPlot->graph(i - 1)->addData(timeData, graphData);

        pGraph->setName(pLabels->at(i));

        QPen pen;
        pen.setColor(_colorlist[colorIndex]);
        pen.setWidth(2);
        pen.setCosmetic(true);

        pGraph->setPen(pen);
   }

   _pPlot->legend->setVisible(true);
   _pPlot->rescaleAxes();
   _pPlot->replot();

}

void GraphViewer::exportGraphImage(QString imageFile)
{
    if (!_pPlot->savePng(imageFile))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("GraphViewer"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Save to png file (%1) failed").arg(imageFile));
        msgBox.exec();
    }
}

void GraphViewer::manualScaleXAxis(qint64 min, qint64 max)
{
    _pPlot->xAxis->setRange(min, max);
    _pPlot->replot();
}

void GraphViewer::manualScaleYAxis(qint64 min, qint64 max)
{
    _pPlot->yAxis->setRange(min, max);
    _pPlot->replot();
}

void GraphViewer::showGraph(quint32 index, bool bShow)
{
    _pPlot->graph(index)->setVisible(bShow);

    QFont itemFont = _pPlot->legend->item(index)->font();
    itemFont.setStrikeOut(!bShow);

    _pPlot->legend->item(index)->setFont(itemFont);

    _pPlot->replot();
}

void GraphViewer::bringToFront(quint32 index, bool bFront)
{

    if (bFront)
    {
        _pPlot->graph(index)->setLayer("topMain");
    }
    else
    {
        _pPlot->graph(index)->setLayer("grid");
    }

    _pPlot->replot();
}

void GraphViewer::autoScaleXAxis()
{
    _pPlot->xAxis->rescale();
    _pPlot->replot();
}

void GraphViewer::autoScaleYAxis()
{
    _pPlot->yAxis->rescale();
    _pPlot->replot();
}

void GraphViewer::generateTickLabels()
{
    QVector<double> ticks = _pPlot->xAxis->tickVector();
    quint32 scaleFactor;

    /* Clear ticks vector */
    tickLabels.clear();

    /* Check if we need seconds, minute or hours on x-axis */
    if (ticks[ticks.size()-1] > _cHourTripPoint)
    {
        _pPlot->xAxis->setLabel("Time (hour)");
        scaleFactor = 60*60*1000;
    }
    else if (ticks[ticks.size()-1] > _cMinuteTripPoint)
    {
        _pPlot->xAxis->setLabel("Time (min)");
        scaleFactor = 60*1000;
    }
    else
    {
        _pPlot->xAxis->setLabel("Time (s)");
        scaleFactor = 1000;
    }

    /* Generate correct labels */
    for (qint32 index = 0; index < ticks.size(); index++)
    {
        tickLabels.append(QString::number(ticks[index] / scaleFactor));
    }

    /* Set labels */
    _pPlot->xAxis->setTickVectorLabels(tickLabels);
}

void GraphViewer::selectionChanged()
{
   /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   */

   // handle axis and tick labels as one selectable object:
   if (_pPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || _pPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) || _pPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
   {
       _pPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spAxisLabel|QCPAxis::spTickLabels);
   }
   // handle axis and tick labels as one selectable object:
   if (_pPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || _pPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) || _pPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxisLabel))
   {
       _pPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spAxisLabel|QCPAxis::spTickLabels);
   }

}

void GraphViewer::mousePress()
{
   // if an axis is selected, only allow the direction of that axis to be dragged
   // if no axis is selected, both directions may be dragged

   if (_pPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
   {
       _pPlot->axisRect()->setRangeDrag(_pPlot->xAxis->orientation());
   }
   else if (_pPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
   {
       _pPlot->axisRect()->setRangeDrag(_pPlot->yAxis->orientation());
   }
   else
   {
       _pPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
   }
}

void GraphViewer::mouseWheel()
{
   // if an axis is selected, only allow the direction of that axis to be zoomed
   // if no axis is selected, both directions may be zoomed

   if (_pPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
   {
       _pPlot->axisRect()->setRangeZoom(_pPlot->xAxis->orientation());
   }
   else if (_pPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
   {
       _pPlot->axisRect()->setRangeZoom(_pPlot->yAxis->orientation());
   }
   else
   {
       _pPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
   }
}

void GraphViewer::axisDoubleClicked(QCPAxis * axis)
{
    axis->rescale();
    _pPlot->replot();
}

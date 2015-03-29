
#include <QMessageBox>
#include <QVector>
#include <QLocale>

#include "graphviewer.h"

const QList<QColor> GraphViewer::_colorlist = QList<QColor>() << QColor(0, 0, 0)
                                                           << QColor(0, 0, 255)
                                                           << QColor(0, 255, 255)
                                                           << QColor(0, 255, 0)
                                                           << QColor(220, 220, 0)
                                                           << QColor(220, 153, 14)
                                                           << QColor(255, 165, 0)
                                                           << QColor(255, 0, 0)
                                                           << QColor(255, 160, 122)
                                                           << QColor(230, 104, 86)
                                                           << QColor(205, 205, 180)
                                                           << QColor(157, 153, 120)
                                                           << QColor(139, 69, 19)
                                                           << QColor(255, 20, 147)
                                                           << QColor(74, 112, 139)
                                                           << QColor(46, 139, 87)
                                                           << QColor(128, 0, 128)
                                                           << QColor(189, 183, 107)
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

   // Tooltip is enabled
   _bEnableTooltip = true;

   // Add layer to move graph on front
   _pPlot->addLayer("topMain", _pPlot->layer("main"), QCustomPlot::limAbove);

   // connect slot that ties some axis selections together (especially opposite axes):
   connect(_pPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

   // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
   connect(_pPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
   connect(_pPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
   connect(_pPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisDoubleClicked(QCPAxis*)));
   connect(_pPlot, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showValueToolTip(QMouseEvent*)));
}

void GraphViewer::clear()
{
    _pPlot->clearGraphs();
}

void GraphViewer::showValueToolTip(QMouseEvent *event)
{
    if  (_bEnableTooltip)
    {
        const double xPos = _pPlot->xAxis->pixelToCoord(event->pos().x());

        if (_pPlot->graphCount() > 0)
        {
            QString toolText;
            const QList<double> keyList = _pPlot->graph(0)->data()->keys();

            // Find if cursor is in range to show tooltip
            qint32 i = 0;
            bool bInRange = false;
            for (i = 1; i < keyList.size(); i++)
            {
                // find the two nearest points
                if (
                    (xPos > keyList[i - 1])
                    && (xPos <= keyList[i])
                    )
                {
                    const double leftPointPxl = _pPlot->xAxis->coordToPixel(keyList[i - 1]);
                    const double rightPointPxl = _pPlot->xAxis->coordToPixel(keyList[i]);
                    const double xCoordPxl = event->pos().x();
                    double keyIndex = -1;

                    if (
                        (xCoordPxl >= leftPointPxl)
                        && (xCoordPxl <= (leftPointPxl + _cPixelNearThreshold))
                        )
                    {
                        keyIndex = i - 1;
                    }
                    else if (
                         (xCoordPxl >= (rightPointPxl - _cPixelNearThreshold))
                         && (xCoordPxl <= rightPointPxl)
                        )
                    {
                        keyIndex = i;
                    }
                    else
                    {
                        // no point near enough
                        keyIndex = -1;
                    }

                    if (keyIndex != -1)
                    {
                        bInRange = true;

                        // Add tick key string
                        toolText = createTickLabelString(keyList[keyIndex]);

                        // Check all graphs
                        for (qint32 graphIndex = 0; graphIndex < _pPlot->graphCount(); graphIndex++)
                        {
                            if (_pPlot->graph(graphIndex)->visible())
                            {
                                const double value = _pPlot->graph(graphIndex)->data()->values()[keyIndex].value;
                                toolText += QString("\n%1: %2").arg(_pPlot->graph(graphIndex)->name()).arg(value);
                            }
                        }
                        break;
                    }
                }
            }

            if (!bInRange)
            {
                // Hide tooltip
                QToolTip::hideText();
            }
            else
            {
                QToolTip::showText(_pPlot->mapToGlobal(event->pos()), toolText, _pPlot);
            }
        }
    }
    else
    {
        if (QToolTip::isVisible())
        {
            QToolTip::hideText();
        }
    }
}

QString GraphViewer::createTickLabelString(qint32 tickKey)
{
    QString tickLabel;
    bool bNegative;
    quint32 tmp;

    if (tickKey < 0)
    {
        bNegative = true;
        tmp = -1 * tickKey;
    }
    else
    {
        bNegative = false;
        tmp = tickKey;
    }

    quint32 hours = tmp / (60 * 60 * 1000);
    tmp = tmp % (60 * 60 * 1000);

    quint32 minutes = tmp / (60 * 1000);
    tmp = tmp % (60 * 1000);

    quint32 seconds = tmp / 1000;
    quint32 milliseconds = tmp % 1000;

    tickLabel = QString("%1:%2:%3%4%5").arg(hours)
                                                .arg(minutes, 2, 10, QChar('0'))
                                                .arg(seconds, 2, 10, QChar('0'))
                                                .arg(QLocale::system().decimalPoint())
                                               .arg(milliseconds, 2, 10, QChar('0'));

    // Make sure minus sign is shown when tick number is negative
    if (bNegative)
    {
        tickLabel = "-" + tickLabel;
    }

    return tickLabel;
}

void GraphViewer::setupData(QList<QList<double> > * pDataLists, QStringList * pLabels)
{
   _pPlot->clearGraphs();

   for (qint32 i = 1; i < pDataLists->size(); i++)
   {
        const quint32 colorIndex = _pPlot->graphCount() % _colorlist.size();

        QCPGraph * pGraph = _pPlot->addGraph();

        pGraph->setName(pLabels->at(i));

        QPen pen;
        pen.setColor(_colorlist[colorIndex]);
        pen.setWidth(2);
        pen.setCosmetic(true);

        pGraph->setPen(pen);
   }

   _pPlot->legend->setVisible(true);

   updateData(pDataLists);

}

void GraphViewer::updateData(QList<QList<double> > * pDataLists)
{
   const QVector<double> timeData = pDataLists->at(0).toVector();

   for (qint32 i = 1; i < pDataLists->size(); i++)
   {
        //Add data to graphs
        QVector<double> graphData = pDataLists->at(i).toVector();
        _pPlot->graph(i - 1)->setData(timeData, graphData);
   }

   _pPlot->rescaleAxes(true);
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

QColor GraphViewer::getGraphColor(quint32 index)
{
    return _pPlot->graph(index)->pen().color();
}

void GraphViewer::autoScaleXAxis()
{
    _pPlot->xAxis->rescale(true);
    _pPlot->replot();
}

void GraphViewer::autoScaleYAxis()
{
    _pPlot->yAxis->rescale(true);
    _pPlot->replot();
}

void GraphViewer::enableValueTooltip(bool bState)
{
    _bEnableTooltip = bState;
}

void GraphViewer::enableSamplePoints(bool bState)
{
    for (qint32 graphIndex = 0; graphIndex < _pPlot->graphCount(); graphIndex++)
    {
        if (bState)
        {
            _pPlot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
        }
        else
        {
            _pPlot->graph(graphIndex)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        }
    }
    _pPlot->replot();
}

void GraphViewer::generateTickLabels()
{
    QVector<double> ticks = _pPlot->xAxis->tickVector();

    /* Clear ticks vector */
    tickLabels.clear();

    /* Generate correct labels */
    for (qint32 index = 0; index < ticks.size(); index++)
    {       
        tickLabels.append(createTickLabelString(ticks[index]));
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
    axis->rescale(true);
    _pPlot->replot();
}

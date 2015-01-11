#ifndef GraphViewer_H
#define GraphViewer_H

#include <QObject>
#include <QVector>
#include <QMouseEvent>

// Foward declaration
class QCustomPlot;

class GraphViewer : public QObject
{
    Q_OBJECT
public:
    explicit GraphViewer(QCustomPlot * pPlot, QObject *parent);

    void setupGraph(QList<QList<double> > *pDataLists);

private slots:
    void generateTickLabels();
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    //void mouseMove(QMouseEvent *event);

private:

    QCustomPlot * _pPlot;

    static const QList<QColor> _colorlist;

    QVector<QString> tickLabels;

    static const quint32 _cMinuteTripPoint = 5*60*1000; /* in ms */
    static const quint32 _cHourTripPoint = 10*60*60*1000; /* in ms */

};

#endif // GraphViewer_H

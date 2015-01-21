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

    void clear();
    void setupGraph(QList<QList<double> > *pDataLists, QStringList *pLabels);
    void exportGraphImage(QString imageFile);
    void manualScaleXAxis(qint64 min, qint64 max);
    void manualScaleYAxis(qint64 min, qint64 max);

public slots:
    void autoScaleXAxis();
    void autoScaleYAxis(); 

private slots:
    void generateTickLabels();
    void selectionChanged();
    void mousePress();
    void mouseWheel();

private:

    QCustomPlot * _pPlot;

    static const QList<QColor> _colorlist;

    QVector<QString> tickLabels;

    static const quint32 _cMinuteTripPoint = 5*60*1000; /* in ms */
    static const quint32 _cHourTripPoint = 10*60*60*1000; /* in ms */

};

#endif // GraphViewer_H

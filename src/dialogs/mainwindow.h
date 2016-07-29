#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QButtonGroup>
#include <QTimer>
#include <QMenu>

namespace Ui {
class MainWindow;
}

// Forward declaration
class QCustomPlot;
class GraphDataModel;
class GuiModel;
class ExtendedGraphView;
class MarkerInfo;
class ParserModel;
class WatchFile;
class Legend;
class DataFileParser;
class LoadFileDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList cmdArguments, QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:

    /* Menu handlers */
    void loadDataFile();
    void reloadDataFile();
    void exitApplication();
    void selectImageExportFile();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void menuBringToFrontGraphClicked(bool bState);
    void menuShowHideGraphClicked(bool bState);

    /* Model change handlers */
    void handleGraphVisibilityChange(const quint32 graphIdx);
    void handleGraphColorChange(const quint32 graphIdx);
    void handleGraphLabelChange(const quint32 graphIdx);

    void updateBringToFrontGrapMenu();
    void updateHighlightSampleMenu();
    void updateValueTooltipMenu();
    void rebuildGraphMenu();
    void updateWindowTitle();
    void enableWatchFile();
    void enableDynamicSession();

    void dataFileLoaded();
    void updateGuiState();
    void updateMarkerDockVisibility();

    /* Misc */
    void legendWidgetUndocked(bool bFloat);
    void showContextMenu(const QPoint& pos);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void loadDataFileAccepted();
    void updateData(void);

private:
    void parseData();
    bool resetGraph(DataFileParser * _pDataFileParser);
    void handleCommandLineArguments(QStringList cmdArguments);
    void loadDataFile(QString dataFilePath);

    Ui::MainWindow * _pUi;
    ParserModel * _pParserModel;

    ExtendedGraphView * _pGraphView;
    DataFileParser * _pParser;
    WatchFile * _pWatchFile;

    GraphDataModel * _pGraphDataModel;

    MarkerInfo * _pMarkerInfo;

    GuiModel * _pGuiModel;

    Legend * _pLegend;

    QMenu * _pGraphBringToFront;
    QMenu * _pGraphShowHide;
    QActionGroup * _pBringToFrontGroup;
    QActionGroup * _pLegendPositionGroup;

    LoadFileDialog * _pLoadDataFileDialog;

    static const quint32 _cGraphShowHideIndex = 2;
};

#endif // MAINWINDOW_H

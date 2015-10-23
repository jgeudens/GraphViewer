#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QTimer>

#include "guimodel.h"
#include "parsermodel.h"
#include "watchfile.h"
#include "datafileparser.h"
#include "extendedgraphview.h"
#include "loadfiledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /* Menu handlers */
    void loadDataFile();
    void reloadDataFile();
    void exitApplication();
    void prepareImageExport();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void menuBringToFrontGraphClicked(bool bState);
    void menuShowHideGraphClicked(bool bState);
    void changeLegendPosition(QAction* pAction);

    /* Model change handlers */
    void showHideGraph(const quint32 index);
    void updateBringToFrontGrapMenu();
    void updateHighlightSampleMenu();
    void updateValueTooltipMenu();
    void clearGraphMenu();
    void addGraphMenu();
    void updateWindowTitle();
    void enableGlobalMenu();
    void enableWatchFile();
    void enableDynamicSession();
    void updateLegendPositionMenu();

    /* Misc */
    void showContextMenu(const QPoint& pos);
    void handleFileChange();
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private:
    void parseData();
    bool resetGraph(DataFileParser *_pDataFileParser);
    void updateGraph(DataFileParser *_pDataFileParser);

    Ui::MainWindow * _pUi;

    GuiModel * _pGuiModel;
    ParserModel * _pParserModel;

    ExtendedGraphView * _pGraphView;
    DataFileParser * _pParser;
    WatchFile * _pWatchFile;
    QMenu * _pGraphShowHide;
    QMenu * _pGraphBringToFront;
    QActionGroup* _pBringToFrontGroup;
    QActionGroup * _pLegendPositionGroup;

    LoadFileDialog * _pLoadDataFileDialog;
};

#endif // MAINWINDOW_H

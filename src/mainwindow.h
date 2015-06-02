#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QTimer>

#include "settingsmodel.h"
#include "watchfile.h"
#include "datafileparser.h"
#include "dataparsersettings.h"
#include "graphviewer.h"
#include "loadfiledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList cmdArguments, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadDataFile();
    void parseData();
    void exitApplication();
    void actionReloadDataFile();
    void prepareImageExport();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void showHideGraph(const quint32 index);
    void updateBringToFrontGrapMenu();
    void updateHighlightSampleMenu();
    void updateValueTooltipMenu();
    void enableWatchFile();
    void enableDynamicSession();
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void showContextMenu(const QPoint& pos);
    void clearGraphMenu();
    void addGraphMenu();

    void actionBringToFrontGraph();
    void actionShowHideGraph(bool bState);
    void updateWindowTitle();
    void enableGlobalMenu();
    void handleFileChange();

private:
    bool resetGraph(DataFileParser *_pDataFileParser);
    void updateGraph(DataFileParser *_pDataFileParser);

    Ui::MainWindow * _pUi;

    SettingsModel * _pModel;

    GraphViewer * _pGraphViewer;
    DataFileParser * _pParser;
    WatchFile * _pWatchFile;
    QMenu * _pGraphShowHide;
    QMenu * _pGraphBringToFront;
    QActionGroup* _pBringToFrontGroup;

    LoadFileDialog _loadDataFileDialog;
};

#endif // MAINWINDOW_H

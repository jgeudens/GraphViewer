#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QTimer>

#include "settingsmodel.h"
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
    void fileDataChange();
    void dynamicUpdate();
    void addFileWatchFail(QString path);
    void prepareImageExport();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void showHideGraph(const quint32 index);
    void updateBringToFrontGrapMenu();
    void updateHighlightSampleMenu();
    void updateValueTooltipMenu();
    void enableWatchFileChanged(bool bState);
    void enableDynamicSessionChanged(bool bState);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void showContextMenu(const QPoint& pos);
    void clearGraphMenu();
    void addGraphMenu();

    void actionBringToFrontGraph();
    void actionShowHideGraph(bool bState);
    void updateWindowTitle();

private:
    bool resetGraph(DataFileParser *_pDataFileParser);
    void updateGraph(DataFileParser *_pDataFileParser);

    Ui::MainWindow * _pUi;

    SettingsModel * _pModel;

    GraphViewer * _pGraphViewer;
    DataFileParser * _pParser;
    QMenu * _pGraphShowHide;
    QMenu * _pGraphBringToFront;
    QActionGroup* _pBringToFrontGroup;

    QTimer _dynamicUpdateTimer;

    LoadFileDialog _loadDataFileDialog;

    static const int cDynamicMaxUpdateInterval;
};

#endif // MAINWINDOW_H

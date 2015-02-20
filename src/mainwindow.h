#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getDataFileSettings();
    void parseData();
    void exitApplication();
    void reloadDataFile();
    void fileDataChange();
    void addFileWatchFail(QString path);
    void prepareImageExport();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void showHideGraph(bool bState);
    void bringToFrontGraph(bool bState);
    void enableWatchFileChanged(bool bState);
    void enableDynamicSessionChanged(bool bState);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void showContextMenu(const QPoint& pos);

private:
    bool updateGraph(DataFileParser *_pDataFileParser);

    Ui::MainWindow * _pUi;

    GraphViewer * _pGraphViewer;
    DataFileParser * _pParser;
    QMenu * _pGraphShowHide;
    QMenu * _pGraphBringToFront;
    QActionGroup* _pBringToFrontGroup;

    LoadFileDialog _loadDataFileDialog;

    static const QString _cWindowTitle;
};

#endif // MAINWINDOW_H

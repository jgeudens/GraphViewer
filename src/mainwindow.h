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
    void exitApplication();
    void reloadDataFile();
    void fileDataChange();
    void addFileWatchFail(QString path);
    void prepareImageExport();
    void showAbout();
    void showXAxisScaleDialog();
    void showYAxisScaleDialog();
    void showHideGraph(bool bState);
    void enableWatchFileChanged(bool bState);
    void enableDynamicSessionChanged(bool bState);

private:
    bool updateGraph(DataFileParser *_pDataFileParser);

    Ui::MainWindow * _pUi;

    GraphViewer * _pGraphViewer;
    DataFileParser * _pParser;
    QMenu * _pGraphShowHide;

    static const QString _cWindowTitle;
};

#endif // MAINWINDOW_H

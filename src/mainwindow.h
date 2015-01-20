#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datafileparser.h"
#include "datatypes.h"
#include "graphviewer.h"
#include "dialog.h"

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

private:
    void updateGraph();

    Ui::MainWindow * _ui;
    DataTypes::DataFileSettings _dataFileSettings;
    GraphViewer * _graphViewer;
    DataFileParser _parser;

    static const QString _cWindowTitle;
};

#endif // MAINWINDOW_H

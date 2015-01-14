#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    explicit MainWindow(QStringList cmdArguments, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void getDataFileSettings();
    void exitApplication();

private:
    Ui::MainWindow * _ui;

    DataTypes::DataFileSettings _dataFileSettings;
    GraphViewer * _graphViewer;

    static const QString _cWindowTitle;
};

#endif // MAINWINDOW_H

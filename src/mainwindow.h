#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow * _ui;

    Dialog::DataFileSettings dataFileSettings;
};

#endif // MAINWINDOW_H

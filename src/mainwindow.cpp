
#include "datafileparser.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    
    
    /*
    TODO: enable
    if (cmdArguments.size() > 1)
    {
        loadProjectFile(cmdArguments[1]);
    }
    */

    connect(_ui->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(getDataFileSettings()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::getDataFileSettings()
{
    Dialog dialog;

    if (dialog.exec())
    {
        dialog.getDataSettings(&dataFileSettings);

       DataFileParser parser;

       if (parser.loadDataFile(dataFileSettings.path))
       {
           // TODO
       }


    }

}


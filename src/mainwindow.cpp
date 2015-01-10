
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
}

MainWindow::~MainWindow()
{
    delete _ui;
}



#include "datatypes.h"
#include "datafileparser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _graphViewer(NULL)
{
    _ui->setupUi(this);
    
    _graphViewer = new GraphViewer(_ui->customPlot, this);
    
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
        dialog.getDataSettings(&_dataFileSettings);

        DataFileParser parser;

        if (parser.loadDataFile(_dataFileSettings.path))
        {
            // TODO
            QList<QList<double> > data;
            QStringList labels;

            if (parser.parseData(_dataFileSettings, data, labels))
            {
                _graphViewer->setupGraph(&data, &labels);
            }

        }
    }
}


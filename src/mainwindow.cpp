
#include "datatypes.h"
#include "datafileparser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_cWindowTitle = QString("CsvGraphViewer");

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _graphViewer(NULL)
{
    _ui->setupUi(this);

    this->setWindowTitle(_cWindowTitle);
    
    _graphViewer = new GraphViewer(_ui->customPlot, this);
    
    /*
    TODO: enable
    if (cmdArguments.size() > 1)
    {
        loadProjectFile(cmdArguments[1]);
    }
    */

    connect(_ui->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(getDataFileSettings()));
    connect(_ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
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

        if (dialog.result() == QDialog::Accepted)
        {
            DataFileParser parser;

            if (parser.loadDataFile(_dataFileSettings.path))
            {
                // TODO
                QList<QList<double> > data;
                QStringList labels;

                if (parser.parseData(_dataFileSettings, data, labels))
                {
                    _graphViewer->setupGraph(&data, &labels);

                    setWindowTitle(QString(tr("%1 - %2")).arg(_cWindowTitle, QFileInfo(_dataFileSettings.path).fileName()));
                }
            }
        }
    }
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}



#include "datatypes.h"
#include "datafileparser.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_cWindowTitle = QString("CsvGraphViewer");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _graphViewer(NULL)
{
    _ui->setupUi(this);

    this->setWindowTitle(_cWindowTitle);
    
    _graphViewer = new GraphViewer(_ui->customPlot, this);

    connect(_ui->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(getDataFileSettings()));
    connect(_ui->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(reloadDataFile()));
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
            _parser.setDataFileSettings(_dataFileSettings);

            updateGraph();
        }
    }
}

void MainWindow::updateGraph()
{
    if (_parser.loadDataFile())
    {
        // TODO
        QList<QList<double> > data;
        QStringList labels;

        if (_parser.parseData(data, labels))
        {
            _graphViewer->setupGraph(&data, &labels);

            setWindowTitle(QString(tr("%1 - %2")).arg(_cWindowTitle, QFileInfo(_dataFileSettings.path).fileName()));

            _ui->actionReloadDataFile->setEnabled(true);
        }
    }
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}

void MainWindow::reloadDataFile()
{
    updateGraph();
}


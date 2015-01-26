
#include "datatypes.h"
#include "datafileparser.h"
#include "axisscaledialog.h"
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
    connect(_ui->actionExportImage, SIGNAL(triggered()), this, SLOT(prepareImageExport()));
    connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    connect(_ui->actionAutoScaleXAxis, SIGNAL(triggered()), _graphViewer, SLOT(autoScaleXAxis()));
    connect(_ui->actionAutoScaleYAxis, SIGNAL(triggered()), _graphViewer, SLOT(autoScaleYAxis()));

    connect(_ui->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_ui->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));

    _graphShowHide = new QMenu("Show/Hide");
    _graphShowHide->setEnabled(false);
    _ui->menuGraph->addSeparator();
    _ui->menuGraph->addMenu(_graphShowHide);
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
        if (dialog.result() == QDialog::Accepted)
        {
            dialog.getDataSettings(&_dataFileSettings);
            _parser.setDataFileSettings(_dataFileSettings);

            updateGraph();
        }
    }
}

void MainWindow::updateGraph()
{
    bool bFailed = false;
    if (_parser.loadDataFile())
    {
        QList<QList<double> > data;
        QStringList labels;

        if (_parser.parseData(data, labels))
        {
            _graphViewer->setupGraph(&data, &labels);

            setWindowTitle(QString(tr("%1 - %2")).arg(_cWindowTitle, QFileInfo(_dataFileSettings.path).fileName()));

            _ui->actionReloadDataFile->setEnabled(true);
            _ui->actionExportImage->setEnabled(true);

            _ui->actionAutoScaleXAxis->setEnabled(true);
            _ui->actionAutoScaleYAxis->setEnabled(true);
            _ui->actionSetManualScaleXAxis->setEnabled(true);
            _ui->actionSetManualScaleYAxis->setEnabled(true);

            // Clear actions
            _graphShowHide->clear();

            // Add menu-items
            for (qint32 i = 1; i < labels.size(); i++)
            {
                QAction *act = _graphShowHide->addAction(labels[i]);

                act->setData(i - 1);
                act->setCheckable(true);
                act->setChecked(true);

                QObject::connect(act, SIGNAL(toggled(bool)), this, SLOT(showHideGraph(bool)));
            }

            _graphShowHide->setEnabled(true);
        }
        else
        {
            bFailed = true;
        }
    }
    else
    {
        bFailed = true;
    }

    if (bFailed)
    {
        setWindowTitle(QString(tr("%1 - %2 ) - Load Failed")).arg(_cWindowTitle, QFileInfo(_dataFileSettings.path).fileName()));
        _graphViewer->clear();

        _ui->actionReloadDataFile->setEnabled(false);
        _ui->actionExportImage->setEnabled(false);

        _ui->actionAutoScaleXAxis->setEnabled(false);
        _ui->actionAutoScaleYAxis->setEnabled(false);
        _ui->actionSetManualScaleXAxis->setEnabled(false);
        _ui->actionSetManualScaleYAxis->setEnabled(false);

        _graphShowHide->setEnabled(false);
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

void MainWindow::prepareImageExport()
{
    QString filePath;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::HideNameFilterDetails, false);
    dialog.setDefaultSuffix("png");
    dialog.setWindowTitle(tr("Select png file"));
    dialog.setNameFilter(tr("PNG files (*.png)"));

    QStringList docPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if (docPath.size() > 0)
    {
        dialog.setDirectory(docPath[0]);
    }

    if (dialog.exec())
    {
        filePath = dialog.selectedFiles().first();
        _graphViewer->exportGraphImage(filePath);
    }
}

void MainWindow::showAbout()
{
    QString lnkAuthor("<a href='mailto:jensgeudens@hotmail.com'>jgeudens</a>");
    QString lnkGpl("<a href='http://www.gnu.org/licenses/gpl.html#content'>GPL</a>");
    QString lnkGitHub("<a href='https://github.com/jgeudens/CsvGraphViewer'>GitHub</a>");

    QString lnkQt("<a href='http://qt-project.org/'>Qt</a>");
    QString lnkQCustomPlot("<a href='http://www.qcustomplot.com/'>QCustomPlot</a>");

    QString version = QString(tr("<b>CsvGraphViewer v%1</b><br><br>")).arg(APP_VERSION);

    QString aboutTxt = tr(
                        "%1"
                        "CsvGraphViewer is created and maintained by %2. This software is released under the %3 license. "
                        "The source is freely available at %4.<br><br>"
                        "CsvGraphViewer uses following libraries:<br>"
                        "%5<br>"
                        "%6<br>").arg(version, lnkAuthor, lnkGpl, lnkGitHub, lnkQt, lnkQCustomPlot);

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("About");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText(aboutTxt);
    msgBox.exec();
}

void MainWindow::showXAxisScaleDialog()
{
    AxisScaleDialog scaleDialog(AxisScaleDialog::AXIS_X);

    if (scaleDialog.exec())
    {
        if (scaleDialog.result() == QDialog::Accepted)
        {
            _graphViewer->manualScaleXAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
        }
    }
}

void MainWindow::showYAxisScaleDialog()
{
    AxisScaleDialog scaleDialog(AxisScaleDialog::AXIS_Y);

    if (scaleDialog.exec())
    {
        if (scaleDialog.result() == QDialog::Accepted)
        {
            _graphViewer->manualScaleYAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
        }
    }
}

void MainWindow::showHideGraph(bool bState)
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());

    _graphViewer->showGraph(pAction->data().toInt(), bState);
}


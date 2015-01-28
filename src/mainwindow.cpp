
#include "dataparsersettings.h"
#include "datafileparser.h"
#include "axisscaledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_cWindowTitle = QString("CsvGraphViewer");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _pGraphViewer(NULL),
    _pParser(NULL)
{
    _ui->setupUi(this);

    this->setWindowTitle(_cWindowTitle);
    
    _pGraphViewer = new GraphViewer(_ui->customPlot, this);

    connect(_ui->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(getDataFileSettings()));
    connect(_ui->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(reloadDataFile()));
    connect(_ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(_ui->actionExportImage, SIGNAL(triggered()), this, SLOT(prepareImageExport()));
    connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    connect(_ui->actionAutoScaleXAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleXAxis()));
    connect(_ui->actionAutoScaleYAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleYAxis()));

    connect(_ui->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_ui->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));

    _pGraphShowHide = new QMenu("Show/Hide");
    _pGraphShowHide->setEnabled(false);
    _ui->menuGraph->addSeparator();
    _ui->menuGraph->addMenu(_pGraphShowHide);
}

MainWindow::~MainWindow()
{
    delete _pGraphViewer;
    delete _pGraphShowHide;
    delete _pParser;
    delete _ui;
}

void MainWindow::getDataFileSettings()
{
    DataFileParser * pNewParser = new DataFileParser();
    bool bSucceeded;
    LoadFileDialog loadDataFileDialog(pNewParser->getDataParseSettingsPointer());

    bSucceeded = false;
    if (loadDataFileDialog.exec())
    {
        if (loadDataFileDialog.result() == QDialog::Accepted)
        {
            if (updateGraph(pNewParser))
            {
                bSucceeded = true;
            }
        }
    }

    if (bSucceeded)
    {
        // Data file parse succeeded

        // delete previous parser is necessairy
        if (_pParser)
        {
            delete _pParser;
        }

        // Set pointer to new parser object
        _pParser = pNewParser;

        connect(_pParser, SIGNAL(fileChanged()), this, SLOT(dataFileChange()));
    }
    else // New file load failed
    {
        // Delete new parser, because invalid
        delete pNewParser;
    }
}

bool MainWindow::updateGraph(DataFileParser * _pDataFileParser)
{
    bool bSucceeded = false;
    if (_pDataFileParser->loadDataFile())
    {
        QList<QList<double> > data;
        QStringList labels;

        if (_pDataFileParser->parseData(data, labels))
        {
            _pGraphViewer->setupGraph(&data, &labels);

            setWindowTitle(QString(tr("%1 - %2")).arg(_cWindowTitle, QFileInfo(_pDataFileParser->getDataParseSettingsPointer()->getPath()).fileName()));

            _ui->actionReloadDataFile->setEnabled(true);
            _ui->actionExportImage->setEnabled(true);

            _ui->actionAutoScaleXAxis->setEnabled(true);
            _ui->actionAutoScaleYAxis->setEnabled(true);
            _ui->actionSetManualScaleXAxis->setEnabled(true);
            _ui->actionSetManualScaleYAxis->setEnabled(true);

            // Clear actions
            _pGraphShowHide->clear();

            // Add menu-items
            for (qint32 i = 1; i < labels.size(); i++)
            {
                QAction *act = _pGraphShowHide->addAction(labels[i]);

                act->setData(i - 1);
                act->setCheckable(true);
                act->setChecked(true);

                QObject::connect(act, SIGNAL(toggled(bool)), this, SLOT(showHideGraph(bool)));
            }

            _pGraphShowHide->setEnabled(true);

            bSucceeded = true;
        }
    }

    if (!bSucceeded)
    {
        setWindowTitle(QString(tr("%1 - %2 ) - Load Failed")).arg(_cWindowTitle, QFileInfo(_pDataFileParser->getDataParseSettingsPointer()->getPath()).fileName()));
    }

    return bSucceeded;
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}

void MainWindow::reloadDataFile()
{
    // Reload data with current parser data
    updateGraph(_pParser);
}

void MainWindow::dataFileChange()
{
    static QMutex mutex;

    if(mutex.tryLock())
    {
        QFile file(_pParser->getDataParseSettingsPointer()->getPath());
        if(file.size() > 0)
        {
            if(_pParser->getDataParseSettingsPointer()->getDynamicSession())
            {
                reloadDataFile();
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Data file changed", "Reload data file?", QMessageBox::Yes|QMessageBox::No);
                if(reply == QMessageBox::Yes)
                {
                    reloadDataFile();
                }
            }
        }

        mutex.unlock();
    }
}

void MainWindow::prepareImageExport()
{
    QString filePath;
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setOption(QFileDialog::HideNameFilterDetails, false);
    fileDialog.setDefaultSuffix("png");
    fileDialog.setWindowTitle(tr("Select png file"));
    fileDialog.setNameFilter(tr("PNG files (*.png)"));

    QStringList docPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if (docPath.size() > 0)
    {
        fileDialog.setDirectory(docPath[0]);
    }

    if (fileDialog.exec())
    {
        filePath = fileDialog.selectedFiles().first();
        _pGraphViewer->exportGraphImage(filePath);
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
            _pGraphViewer->manualScaleXAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
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
            _pGraphViewer->manualScaleYAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
        }
    }
}

void MainWindow::showHideGraph(bool bState)
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());

    _pGraphViewer->showGraph(pAction->data().toInt(), bState);
}


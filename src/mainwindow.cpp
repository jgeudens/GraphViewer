
#include "dataparsersettings.h"
#include "datafileparser.h"
#include "axisscaledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString MainWindow::_cWindowTitle = QString("GraphViewer");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow),
    _pGraphViewer(NULL),
    _pParser(NULL)
{
    _pUi->setupUi(this);

    this->setWindowTitle(_cWindowTitle);
    
    _pGraphViewer = new GraphViewer(_pUi->customPlot, this);

    connect(_pUi->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(getDataFileSettings()));
    connect(_pUi->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(reloadDataFile()));
    connect(_pUi->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(_pUi->actionExportImage, SIGNAL(triggered()), this, SLOT(prepareImageExport()));
    connect(_pUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    connect(_pUi->actionAutoScaleXAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleXAxis()));
    connect(_pUi->actionAutoScaleYAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleYAxis()));

    connect(_pUi->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_pUi->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));

    connect(_pUi->actionWatchFile, SIGNAL(toggled(bool)), this, SLOT(enableWatchFileChanged(bool)));
    connect(_pUi->actionDynamicSession, SIGNAL(toggled(bool)), this, SLOT(enableDynamicSessionChanged(bool)));

    _pGraphShowHide = _pUi->menuShowHide;

    this->setAcceptDrops(true);

    _pUi->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_pUi->customPlot, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    delete _pGraphViewer;
    delete _pGraphShowHide;
    delete _pParser;
    delete _pUi;
}

void MainWindow::getDataFileSettings()
{
    if (_loadDataFileDialog.exec() == QDialog::Accepted)
    {
        parseData();
    }
}

void MainWindow::parseData()
{
    DataFileParser * pNewParser = new DataFileParser();

    //Get settings from dialog
    _loadDataFileDialog.getParserSettings(pNewParser->getDataParseSettings());

    if (updateGraph(pNewParser))
    {
        // Data file parse succeeded

        // delete previous parser is necessairy
        if (_pParser)
        {
            delete _pParser;
        }

        // Set pointer to new parser object
        _pParser = pNewParser;

        connect(_pParser, SIGNAL(fileDataChanged()), this, SLOT(fileDataChange()));
        connect(_pParser->getDataParseSettings(), SIGNAL(watchFileDataChanged(bool)), _pUi->actionWatchFile, SLOT(setChecked(bool)));
        connect(_pParser->getDataParseSettings(), SIGNAL(dynamicSessionChanged(bool)), _pUi->actionDynamicSession, SLOT(setChecked(bool)));
        connect(_pParser, SIGNAL(addFileWatchFailed(QString)), this, SLOT(addFileWatchFail(QString)));

        _pUi->actionWatchFile->setEnabled(true);
        _pUi->actionWatchFile->setChecked(_pParser->getDataParseSettings()->getWatchFileData());
        _pUi->actionDynamicSession->setEnabled(true);
        _pUi->actionDynamicSession->setChecked(_pParser->getDataParseSettings()->getDynamicSession());
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

            setWindowTitle(QString(tr("%1 - %2")).arg(_cWindowTitle, QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName()));

            _pUi->actionReloadDataFile->setEnabled(true);
            _pUi->actionExportImage->setEnabled(true);

            _pUi->actionAutoScaleXAxis->setEnabled(true);
            _pUi->actionAutoScaleYAxis->setEnabled(true);
            _pUi->actionSetManualScaleXAxis->setEnabled(true);
            _pUi->actionSetManualScaleYAxis->setEnabled(true);

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
            _pUi->menuScale->setEnabled(true);

            bSucceeded = true;
        }
    }

    if (!bSucceeded)
    {
        setWindowTitle(QString(tr("%1 - %2 ) - Load Failed")).arg(_cWindowTitle, QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName()));
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

void MainWindow::fileDataChange()
{
    static QMutex mutex;

    if(_pParser->getDataParseSettings()->getWatchFileData())
    {
        if(mutex.tryLock())
        {
            QFile file(_pParser->getDataParseSettings()->getPath());
            if(file.size() > 0)
            {
                if(_pParser->getDataParseSettings()->getDynamicSession())
                {
                    reloadDataFile();
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Data file changed", "Reload data file? Press cancel to disable the auto reload  function.", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);
                    if(reply == QMessageBox::Yes)
                    {
                        reloadDataFile();
                    }
                    else if(reply == QMessageBox::Cancel)
                    {
                        _pParser->getDataParseSettings()->setWatchFileData(false);
                    }
                }
            }

            mutex.unlock();
        }
    }
}

void MainWindow::addFileWatchFail(QString path)
{
    QMessageBox::warning(this, "Add file watch failed", "Failed to watch \"" + path + "\". Please check your system configuration!");
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
    QString lnkGitHub("<a href='https://github.com/jgeudens/GraphViewer'>GitHub</a>");

    QString lnkQt("<a href='http://qt-project.org/'>Qt</a>");
    QString lnkQCustomPlot("<a href='http://www.qcustomplot.com/'>QCustomPlot</a>");

    QString version = QString(tr("<b>GraphViewer v%1</b><br><br>")).arg(APP_VERSION);

    QString aboutTxt = tr(
                        "%1"
                        "GraphViewer is created and maintained by %2. This software is released under the %3 license. "
                        "The source is freely available at %4.<br><br>"
                        "GraphViewer uses following libraries:<br>"
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

void MainWindow::enableWatchFileChanged(bool bState)
{
    _pUi->actionWatchFile->setChecked(bState);
    _pUi->actionDynamicSession->setEnabled(bState);

    if(_pParser != NULL)
    {
        _pParser->getDataParseSettings()->setWatchFileData(bState);
    }
}

void MainWindow::enableDynamicSessionChanged(bool bState)
{
    if(_pParser != NULL)
    {
        _pParser->getDataParseSettings()->setDynamicSession(bState);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        const QString &fileName = url.toLocalFile();
        if (_loadDataFileDialog.exec(fileName) == QDialog::Accepted)
        {
            parseData();
        }
    }
}

void MainWindow::showContextMenu(const QPoint& pos)
{
    _pUi->menuView->popup(_pUi->customPlot->mapToGlobal(pos));
}

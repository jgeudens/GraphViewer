
#include "dataparsersettings.h"
#include "datafileparser.h"
#include "axisscaledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow),
    _pGraphViewer(NULL),
    _pParser(NULL)
{
    _pUi->setupUi(this);
    
    _pModel = new SettingsModel();
    _pGraphViewer = new GraphViewer(_pModel, _pUi->customPlot, this);
    _pWatchFile = new WatchFile(_pModel);

    /* Add slot for file watcher */
    connect(_pWatchFile, SIGNAL(fileDataChanged()), this, SLOT(handleFileChange()));

    /*-- Connect menu actions --*/
    connect(_pUi->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(loadDataFile()));
    connect(_pUi->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(actionReloadDataFile()));
    connect(_pUi->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(_pUi->actionExportImage, SIGNAL(triggered()), this, SLOT(prepareImageExport()));
    connect(_pUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(_pUi->actionAutoScaleXAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleXAxis()));
    connect(_pUi->actionAutoScaleYAxis, SIGNAL(triggered()), _pGraphViewer, SLOT(autoScaleYAxis()));
    connect(_pUi->actionShowValueTooltip, SIGNAL(toggled(bool)), _pModel, SLOT(setValueTooltip(bool)));
    connect(_pUi->actionHighlightSamplePoints, SIGNAL(toggled(bool)), _pModel, SLOT(setHighlightSamples(bool)));
    connect(_pUi->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_pUi->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));
    connect(_pUi->actionWatchFile, SIGNAL(toggled(bool)), _pModel, SLOT(setWatchFile(bool)));
    connect(_pUi->actionDynamicSession, SIGNAL(toggled(bool)), _pModel, SLOT(setDynamicSession(bool)));
    connect(_pUi->actionShowHideLegend, SIGNAL(toggled(bool)), _pModel, SLOT(setLegendVisibility(bool)));


    /*-- connect model to view --*/
    connect(_pModel, SIGNAL(graphVisibilityChanged(const quint32)), this, SLOT(showHideGraph(const quint32)));
    connect(_pModel, SIGNAL(graphVisibilityChanged(const quint32)), _pGraphViewer, SLOT(showGraph(const quint32)));
    connect(_pModel, SIGNAL(frontGraphChanged()), this, SLOT(updateBringToFrontGrapMenu()));
    connect(_pModel, SIGNAL(frontGraphChanged()), _pGraphViewer, SLOT(bringToFront()));
    connect(_pModel, SIGNAL(highlightSamplesChanged()), this, SLOT(updateHighlightSampleMenu()));
    connect(_pModel, SIGNAL(highlightSamplesChanged()), _pGraphViewer, SLOT(enableSamplePoints()));
    connect(_pModel, SIGNAL(valueTooltipChanged()), this, SLOT(updateValueTooltipMenu()));
    connect(_pModel, SIGNAL(valueTooltipChanged()), _pGraphViewer, SLOT(enableValueTooltip()));
    connect(_pModel, SIGNAL(graphCleared()), _pGraphViewer, SLOT(clearGraphs()));
    connect(_pModel, SIGNAL(graphCleared()), this, SLOT(clearGraphMenu()));
    connect(_pModel, SIGNAL(graphsAdded(QList<QList<double> >)), _pGraphViewer, SLOT(addGraphs(QList<QList<double> >)));
    connect(_pModel, SIGNAL(graphsAdded(QList<QList<double> >)), this, SLOT(addGraphMenu()));
    connect(_pModel, SIGNAL(windowTitleChanged()), this, SLOT(updateWindowTitle()));
    connect(_pModel, SIGNAL(loadedFileChanged()), this, SLOT(enableGlobalMenu()));
    connect(_pModel, SIGNAL(watchFileChanged()), this, SLOT(enableWatchFile()));
    connect(_pModel, SIGNAL(dynamicSessionChanged()), this, SLOT(enableDynamicSession()));
    connect(_pModel, SIGNAL(legendVisibilityChanged()), _pGraphViewer, SLOT(showHideLegend()));

    _pGraphShowHide = _pUi->menuShowHide;
    _pGraphBringToFront = _pUi->menuBringToFront;
    _pBringToFrontGroup = new QActionGroup(this);

    this->setAcceptDrops(true);

    _pUi->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_pUi->customPlot, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    _pModel->setWindowTitleDetail("");

    /* Update interface via model */
    _pModel->triggerUpdate();

    /*-- proces command line arguments --*/
    QCommandLineParser argumentParser;

    argumentParser.setApplicationDescription("GraphViewer");
    argumentParser.addHelpOption();

    // datafile option
    argumentParser.addPositionalArgument("datafile", QCoreApplication::translate("main", "Specify datafile to parse (optionally)"));

    // skipDialog option
    QCommandLineOption skipOption(QStringList() << "s" << "skip-dialog", QCoreApplication::translate("main", "Skip parse settings dialog if possible (option is ignored when datafile is not specified)"));
    argumentParser.addOption(skipOption);

    // Process arguments
    argumentParser.process(cmdArguments);

    if (!argumentParser.positionalArguments().isEmpty())
    {
        QString filename = argumentParser.positionalArguments().first();

        bool bSkipDialog = argumentParser.isSet(skipOption);

        if (_loadDataFileDialog.exec(filename, bSkipDialog) == QDialog::Accepted)
        {
            parseData();
        }
    }
}

MainWindow::~MainWindow()
{
    delete _pWatchFile;
    delete _pGraphViewer;
    delete _pGraphShowHide;
    delete _pGraphBringToFront;
    delete _pParser;
    delete _pUi;
}

void MainWindow::loadDataFile()
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

    if (resetGraph(pNewParser))
    {
        // Data file parse succeeded

        // delete previous parser is necessairy
        if (_pParser)
        {
            delete _pParser;
        }

        // Set pointer to new parser object
        _pParser = pNewParser;

        _pModel->setWatchFile(true);
        // Dynamic session is set in loadFileDialog
    }
    else // New file load failed
    {
        // Delete new parser, because invalid
        delete pNewParser;
    }
}

bool MainWindow::resetGraph(DataFileParser * _pDataFileParser)
{
    bool bSucceeded = false;
    if (_pDataFileParser->forceProcessDataFile())
    {
        _pModel->clearGraph();
        _pModel->addGraphs(_pDataFileParser->getDataLabels(), _pDataFileParser->getDataRows());
        _pModel->setFrontGraph(0);
        _pModel->setLoadedFile(QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName());
        _pModel->setWindowTitleDetail(_pModel->loadedFile());
        _pModel->setDynamicSession(_pDataFileParser->getDataParseSettings()->getDynamicSession());

        bSucceeded = true;
    }

    return bSucceeded;
}

void MainWindow::updateGraph(DataFileParser *_pDataFileParser)
{
    if (_pDataFileParser->processDataFile())
    {
        _pGraphViewer->updateData(&_pDataFileParser->getDataRows());
    }
    else
    {
        _pModel->setWindowTitleDetail(QString(tr("Load Failed - %1")).arg(QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName()));

        // disable watch
        _pModel->setWatchFile(false);
    }
}

void MainWindow::exitApplication()
{
    QApplication::quit();
}

void MainWindow::actionReloadDataFile()
{
    // Reload data with current parser data
    resetGraph(_pParser);
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

    QString appVersion = QString(tr("v%1")).arg(APP_VERSION);

#ifdef DEBUG
    appVersion.append(QString(tr(" (git: %1:%2)")).arg(GIT_BRANCH).arg(GIT_HASH));
#endif

    QString version = QString(tr("<b>GraphViewer %1</b><br><br>")).arg(appVersion);

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


void MainWindow::actionShowHideGraph(bool bState)
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());

    _pModel->setGraphVisibility(pAction->data().toInt(), bState);

}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(_pModel->windowTitle());
}

void MainWindow::enableGlobalMenu()
{
    _pUi->actionReloadDataFile->setEnabled(true);
    _pUi->actionExportImage->setEnabled(true);
    _pUi->actionAutoScaleXAxis->setEnabled(true);
    _pUi->actionAutoScaleYAxis->setEnabled(true);
    _pUi->actionSetManualScaleXAxis->setEnabled(true);
    _pUi->actionSetManualScaleYAxis->setEnabled(true);
    _pUi->actionShowValueTooltip->setEnabled(true);
    _pUi->actionHighlightSamplePoints->setEnabled(true);
    _pUi->menuScale->setEnabled(true);
    _pUi->actionWatchFile->setEnabled(true);
    _pUi->actionShowHideLegend->setEnabled(true);
}

void MainWindow::showHideGraph(const quint32 index)
{
    _pGraphShowHide->actions().at(index)->setChecked(_pModel->graphVisibility(index));

    // Show/Hide corresponding "BringToFront" action
    _pGraphBringToFront->actions().at(index)->setVisible(_pModel->graphVisibility((index)));

    // Enable/Disable global BringToFront menu
    bool bVisible = false;    _pGraphBringToFront->setEnabled(true);
    foreach(QAction * pAction, _pGraphBringToFront->actions())
    {
        if (pAction->isVisible())
        {
            bVisible = true;
            break;
        }
    }
    _pGraphBringToFront->setEnabled(bVisible);
}

void MainWindow::updateValueTooltipMenu()
{
    /* set menu to checked */
    _pUi->actionShowValueTooltip->setChecked(_pModel->valueTooltip());
}

void MainWindow::clearGraphMenu()
{
    // Clear actions
    _pGraphShowHide->clear();
    _pBringToFrontGroup->actions().clear();
    _pGraphBringToFront->clear();
}

void MainWindow::addGraphMenu()
{

    for (quint32 idx = 0; idx < _pModel->graphCount(); idx++)
    {
        QString label = _pModel->graphLabel(idx);
        QAction * pShowHideAction = _pGraphShowHide->addAction(label);
        QAction * pBringToFront = _pGraphBringToFront->addAction(label);

        QPixmap pixmap(20,5);
        pixmap.fill(_pModel->graphColor(idx));
        QIcon * pBringToFrontIcon = new QIcon(pixmap);
        QIcon * pShowHideIcon = new QIcon(pixmap);

        pShowHideAction->setData(idx);
        pShowHideAction->setIcon(*pBringToFrontIcon);
        pShowHideAction->setCheckable(true);
        pShowHideAction->setChecked(_pModel->graphVisibility(idx));

        pBringToFront->setData(idx);
        pBringToFront->setIcon(*pShowHideIcon);
        pBringToFront->setCheckable(true);
        pBringToFront->setActionGroup(_pBringToFrontGroup);

        QObject::connect(pShowHideAction, SIGNAL(toggled(bool)), this, SLOT(actionShowHideGraph(bool)));
        QObject::connect(pBringToFront, SIGNAL(toggled(bool)), this, SLOT(actionBringToFrontGraph()));
    }

    _pGraphShowHide->setEnabled(true);
    _pGraphBringToFront->setEnabled(true);

}

void MainWindow::updateHighlightSampleMenu()
{
    /* set menu to checked */
    _pUi->actionHighlightSamplePoints->setChecked(_pModel->highlightSamples());
}

void MainWindow::updateBringToFrontGrapMenu()
{
    if (_pBringToFrontGroup->actions().size() > 0)
    {
        _pBringToFrontGroup->actions().at(_pModel->frontGraph())->setChecked(true);
    }
}

void MainWindow::actionBringToFrontGraph()
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());
    _pModel->setFrontGraph(pAction->data().toInt());
}

void MainWindow::enableWatchFile()
{
    // Setup file watcher
    if (_pModel->watchFile())
    {
        _pWatchFile->enableFileWatch(_pParser->getDataParseSettings()->getPath());
    }
    else
    {
        _pWatchFile->disableFileWatch();
    }

    _pUi->actionWatchFile->setChecked(_pModel->watchFile());
    _pUi->actionDynamicSession->setEnabled(_pModel->watchFile());
}

void MainWindow::enableDynamicSession()
{
    _pUi->actionDynamicSession->setChecked(_pModel->dynamicSession());
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
        if (_loadDataFileDialog.exec(fileName, false) == QDialog::Accepted)
        {
            parseData();
        }
    }
}

void MainWindow::showContextMenu(const QPoint& pos)
{
    _pUi->menuView->popup(_pUi->customPlot->mapToGlobal(pos));
}

void MainWindow::handleFileChange()
{
    updateGraph(_pParser);
}

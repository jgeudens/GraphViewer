
#include "dataparsersettings.h"
#include "datafileparser.h"
#include "axisscaledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow),
    _pGraphView(NULL),
    _pParser(NULL)
{
    _pUi->setupUi(this);
    
    _pGuiModel = new GuiModel();
    _pGraphView = new ExtendedGraphView(_pGuiModel, _pUi->customPlot, this);
    _pWatchFile = new WatchFile(_pGuiModel);

    /* Add slot for file watcher */
    connect(_pWatchFile, SIGNAL(fileDataChanged()), this, SLOT(handleFileChange()));

    /*-- Connect menu actions --*/
    connect(_pUi->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(loadDataFile()));
    connect(_pUi->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(reloadDataFile()));
    connect(_pUi->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(_pUi->actionExportImage, SIGNAL(triggered()), this, SLOT(prepareImageExport()));
    connect(_pUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(_pUi->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_pUi->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));
    connect(_pUi->actionAutoScaleXAxis, SIGNAL(triggered()), _pGraphView, SLOT(autoScaleXAxis()));
    connect(_pUi->actionAutoScaleYAxis, SIGNAL(triggered()), _pGraphView, SLOT(autoScaleYAxis()));
    connect(_pUi->actionShowValueTooltip, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setValueTooltip(bool)));
    connect(_pUi->actionHighlightSamplePoints, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setHighlightSamples(bool)));
    connect(_pUi->actionWatchFile, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setWatchFile(bool)));
    connect(_pUi->actionDynamicSession, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setDynamicSession(bool)));
    connect(_pUi->actionShowHideLegend, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setLegendVisibility(bool)));

    /*-- connect model to view --*/
    connect(_pGuiModel, SIGNAL(graphVisibilityChanged(const quint32)), this, SLOT(showHideGraph(const quint32)));
    connect(_pGuiModel, SIGNAL(graphVisibilityChanged(const quint32)), _pGraphView, SLOT(showGraph(const quint32)));
    connect(_pGuiModel, SIGNAL(frontGraphChanged()), this, SLOT(updateBringToFrontGrapMenu()));
    connect(_pGuiModel, SIGNAL(frontGraphChanged()), _pGraphView, SLOT(bringToFront()));
    connect(_pGuiModel, SIGNAL(highlightSamplesChanged()), this, SLOT(updateHighlightSampleMenu()));
    connect(_pGuiModel, SIGNAL(highlightSamplesChanged()), _pGraphView, SLOT(enableSamplePoints()));
    connect(_pGuiModel, SIGNAL(valueTooltipChanged()), this, SLOT(updateValueTooltipMenu()));
    connect(_pGuiModel, SIGNAL(valueTooltipChanged()), _pGraphView, SLOT(enableValueTooltip()));
    connect(_pGuiModel, SIGNAL(graphCleared()), _pGraphView, SLOT(clearGraphs()));
    connect(_pGuiModel, SIGNAL(graphCleared()), this, SLOT(clearGraphMenu()));
    connect(_pGuiModel, SIGNAL(graphsAdded(QList<QList<double> >)), _pGraphView, SLOT(addGraphs(QList<QList<double> >)));
    connect(_pGuiModel, SIGNAL(graphsAdded(QList<QList<double> >)), this, SLOT(addGraphMenu()));
    connect(_pGuiModel, SIGNAL(windowTitleChanged()), this, SLOT(updateWindowTitle()));
    connect(_pGuiModel, SIGNAL(loadedFileChanged()), this, SLOT(enableGlobalMenu()));
    connect(_pGuiModel, SIGNAL(watchFileChanged()), this, SLOT(enableWatchFile()));
    connect(_pGuiModel, SIGNAL(dynamicSessionChanged()), this, SLOT(enableDynamicSession()));
    connect(_pGuiModel, SIGNAL(legendVisibilityChanged()), _pGraphView, SLOT(showHideLegend()));

    _pGraphShowHide = _pUi->menuShowHide;
    _pGraphBringToFront = _pUi->menuBringToFront;
    _pBringToFrontGroup = new QActionGroup(this);

    this->setAcceptDrops(true);

    _pUi->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_pUi->customPlot, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    _pGuiModel->setWindowTitleDetail("");

    /* Update interface via model */
    _pGuiModel->triggerUpdate();

    handleCommandLineArguments(cmdArguments);
}

MainWindow::~MainWindow()
{
    delete _pBringToFrontGroup;
    delete _pWatchFile;
    delete _pGraphView;
    delete _pGraphShowHide;
    delete _pGraphBringToFront;
    delete _pParser;
    delete _pGuiModel;
    delete _pUi;
}

void MainWindow::loadDataFile()
{
    if (_loadDataFileDialog.exec() == QDialog::Accepted)
    {
        parseData();
    }
}

void MainWindow::reloadDataFile()
{
    // Reload data with current parser data
    resetGraph(_pParser);
}

void MainWindow::exitApplication()
{
    QApplication::quit();
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
        _pGraphView->exportGraphImage(filePath);
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
            _pGraphView->manualScaleXAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
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
            _pGraphView->manualScaleYAxis(scaleDialog.getMinimum(), scaleDialog.getMaximum());
        }
    }
}

void MainWindow::menuBringToFrontGraphClicked(bool bState)
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());

    if (bState)
    {
        _pGuiModel->setFrontGraph(pAction->data().toInt());
    }
}

void MainWindow::menuShowHideGraphClicked(bool bState)
{
    QAction * pAction = qobject_cast<QAction *>(QObject::sender());

    _pGuiModel->setGraphVisibility(pAction->data().toInt(), bState);

}

void MainWindow::showHideGraph(const quint32 index)
{
    _pGraphShowHide->actions().at(index)->setChecked(_pGuiModel->graphVisibility(index));

    // Show/Hide corresponding "BringToFront" action
    _pGraphBringToFront->actions().at(index)->setVisible(_pGuiModel->graphVisibility((index)));

    // Enable/Disable global BringToFront menu
    bool bVisible = false;
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

void MainWindow::updateBringToFrontGrapMenu()
{
    if (_pBringToFrontGroup->actions().size() > 0)
    {
        _pBringToFrontGroup->actions().at(_pGuiModel->frontGraph())->setChecked(true);
    }
}

void MainWindow::updateHighlightSampleMenu()
{
    /* set menu to checked */
    _pUi->actionHighlightSamplePoints->setChecked(_pGuiModel->highlightSamples());
}

void MainWindow::updateValueTooltipMenu()
{
    /* set menu to checked */
    _pUi->actionShowValueTooltip->setChecked(_pGuiModel->valueTooltip());
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

    for (quint32 idx = 0; idx < _pGuiModel->graphCount(); idx++)
    {
        QString label = _pGuiModel->graphLabel(idx);
        QAction * pShowHideAction = _pGraphShowHide->addAction(label);
        QAction * pBringToFront = _pGraphBringToFront->addAction(label);

        QPixmap pixmap(20,5);
        pixmap.fill(_pGuiModel->graphColor(idx));
        QIcon * pBringToFrontIcon = new QIcon(pixmap);
        QIcon * pShowHideIcon = new QIcon(pixmap);

        pShowHideAction->setData(idx);
        pShowHideAction->setIcon(*pBringToFrontIcon);
        pShowHideAction->setCheckable(true);
        pShowHideAction->setChecked(_pGuiModel->graphVisibility(idx));

        pBringToFront->setData(idx);
        pBringToFront->setIcon(*pShowHideIcon);
        pBringToFront->setCheckable(true);
        pBringToFront->setActionGroup(_pBringToFrontGroup);

        QObject::connect(pShowHideAction, SIGNAL(toggled(bool)), this, SLOT(menuShowHideGraphClicked(bool)));
        QObject::connect(pBringToFront, SIGNAL(toggled(bool)), this, SLOT(menuBringToFrontGraphClicked(bool)));
    }

    _pGraphShowHide->setEnabled(true);
    _pGraphBringToFront->setEnabled(true);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(_pGuiModel->windowTitle());
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

void MainWindow::enableWatchFile()
{
    // Setup file watcher
    if (_pGuiModel->watchFile())
    {
        _pWatchFile->enableFileWatch(_pParser->getDataParseSettings()->getPath());
    }
    else
    {
        _pWatchFile->disableFileWatch();
    }

    _pUi->actionWatchFile->setChecked(_pGuiModel->watchFile());
    _pUi->actionDynamicSession->setEnabled(_pGuiModel->watchFile());
}

void MainWindow::enableDynamicSession()
{
    _pUi->actionDynamicSession->setChecked(_pGuiModel->dynamicSession());
}

void MainWindow::showContextMenu(const QPoint& pos)
{
    _pUi->menuView->popup(_pUi->customPlot->mapToGlobal(pos));
}

void MainWindow::handleFileChange()
{
    updateGraph(_pParser);
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

        _pGuiModel->setWatchFile(true);
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
        _pGuiModel->clearGraph();
        _pGuiModel->addGraphs(_pDataFileParser->getDataLabels(), _pDataFileParser->getDataRows());
        _pGuiModel->setFrontGraph(0);
        _pGuiModel->setLoadedFile(QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName());
        _pGuiModel->setWindowTitleDetail(_pGuiModel->loadedFile());
        _pGuiModel->setDynamicSession(_pDataFileParser->getDataParseSettings()->getDynamicSession());

        bSucceeded = true;
    }

    return bSucceeded;
}

void MainWindow::updateGraph(DataFileParser *_pDataFileParser)
{
    if (_pDataFileParser->processDataFile())
    {
        _pGraphView->updateData(&_pDataFileParser->getDataRows());
    }
    else
    {
        _pGuiModel->setWindowTitleDetail(QString(tr("Load Failed - %1")).arg(QFileInfo(_pDataFileParser->getDataParseSettings()->getPath()).fileName()));

        // disable watch
        _pGuiModel->setWatchFile(false);
    }
}

void MainWindow::handleCommandLineArguments(QStringList cmdArguments)
{
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

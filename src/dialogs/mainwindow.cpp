
#include "datafileparser.h"
#include "axisscaledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "graphdatamodel.h"
#include "graphdata.h"
#include "aboutdialog.h"
#include "markerinfo.h"
#include "guimodel.h"
#include "extendedgraphview.h"
#include "util.h"

#include "parsermodel.h"
#include "watchfile.h"
#include "legend.h"
#include "datafileparser.h"
#include "loadfiledialog.h"


#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QStringList cmdArguments, QWidget *parent) :
    QMainWindow(parent),
    _pUi(new Ui::MainWindow)
{
    _pUi->setupUi(this);

    _pParser = NULL;
    
    _pGuiModel = new GuiModel();
    _pParserModel = new ParserModel();
    _pGraphDataModel = new GraphDataModel();
    _pGraphView = new ExtendedGraphView(_pGuiModel, _pGraphDataModel, _pUi->customPlot, this);
    _pWatchFile = new WatchFile(_pGuiModel, _pParserModel);

    _pMarkerInfo = _pUi->markerInfo;
    _pMarkerInfo->setModel(_pGuiModel, _pGraphDataModel);

    _pLoadDataFileDialog = new LoadFileDialog(_pGuiModel, _pParserModel, this);
    connect(_pLoadDataFileDialog, SIGNAL(accepted()), this, SLOT(loadDataFileAccepted()));

    /* Add slot for file watcher */
    connect(_pWatchFile, SIGNAL(fileDataChanged()), this, SLOT(updateData()));

    _pLegend = _pUi->legend;
    _pLegend->setModels(_pGuiModel, _pGraphDataModel);
    _pLegend->setGraphview(_pGraphView);

    /*-- Connect menu actions --*/
    connect(_pUi->actionLoadDataFile, SIGNAL(triggered()), this, SLOT(loadDataFile()));
    connect(_pUi->actionReloadDataFile, SIGNAL(triggered()), this, SLOT(reloadDataFile()));
    connect(_pUi->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(_pUi->actionExportImage, SIGNAL(triggered()), this, SLOT(selectImageExportFile()));
    connect(_pUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(_pUi->actionSetManualScaleXAxis, SIGNAL(triggered()), this, SLOT(showXAxisScaleDialog()));
    connect(_pUi->actionSetManualScaleYAxis, SIGNAL(triggered()), this, SLOT(showYAxisScaleDialog()));
    connect(_pUi->actionAutoScaleXAxis, SIGNAL(triggered()), _pGraphView, SLOT(autoScaleXAxis()));
    connect(_pUi->actionAutoScaleYAxis, SIGNAL(triggered()), _pGraphView, SLOT(autoScaleYAxis()));
    connect(_pUi->actionHighlightSamplePoints, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setHighlightSamples(bool)));
    connect(_pUi->actionClearMarkers, SIGNAL(triggered()), _pGuiModel, SLOT(clearMarkersState()));
    connect(_pUi->actionWatchFile, SIGNAL(toggled(bool)), _pGuiModel, SLOT(setWatchFile(bool)));
    connect(_pUi->actionDynamicSession, SIGNAL(toggled(bool)), _pParserModel, SLOT(setDynamicSession(bool)));

    /*-- connect model to view --*/
    connect(_pGuiModel, SIGNAL(frontGraphChanged()), this, SLOT(updateBringToFrontGrapMenu()));
    connect(_pGuiModel, SIGNAL(frontGraphChanged()), _pGraphView, SLOT(bringToFront()));
    connect(_pGuiModel, SIGNAL(highlightSamplesChanged()), this, SLOT(updateHighlightSampleMenu()));
    connect(_pGuiModel, SIGNAL(highlightSamplesChanged()), _pGraphView, SLOT(enableSamplePoints()));
    connect(_pGuiModel, SIGNAL(cursorValuesChanged()), _pGraphView, SLOT(updateTooltip()));
    connect(_pGuiModel, SIGNAL(cursorValuesChanged()), _pLegend, SLOT(updateDataInLegend()));

    connect(_pGuiModel, SIGNAL(windowTitleChanged()), this, SLOT(updateWindowTitle()));
    connect(_pGuiModel, SIGNAL(watchFileChanged()), this, SLOT(enableWatchFile()));
    connect(_pParserModel, SIGNAL(dynamicSessionChanged()), this, SLOT(enableDynamicSession()));
    connect(_pGuiModel, SIGNAL(dataFilePathChanged()), this, SLOT(dataFileLoaded()));
    connect(_pGuiModel, SIGNAL(guiStateChanged()), this, SLOT(updateGuiState()));

    connect(_pGuiModel, SIGNAL(xAxisScalingChanged()), _pGraphView, SLOT(rescalePlot()));
    connect(_pGuiModel, SIGNAL(yAxisScalingChanged()), _pGraphView, SLOT(rescalePlot()));

    connect(_pGuiModel, SIGNAL(markerStateChanged()), _pGraphView, SLOT(updateMarkersVisibility()));
    connect(_pGuiModel, SIGNAL(markerStateChanged()), this, SLOT(updateMarkerDockVisibility()));
    connect(_pGuiModel, SIGNAL(startMarkerPosChanged()), _pGraphView, SLOT(setStartMarker()));
    connect(_pGuiModel, SIGNAL(endMarkerPosChanged()), _pGraphView, SLOT(setEndMarker()));

    connect(_pGraphDataModel, SIGNAL(visibilityChanged(quint32)), this, SLOT(handleGraphVisibilityChange(const quint32)));
    connect(_pGraphDataModel, SIGNAL(visibilityChanged(quint32)), _pGraphView, SLOT(showGraph(const quint32)));
    connect(_pGraphDataModel, SIGNAL(graphsAddData(QList<double>, QList<QList<double> >)), _pGraphView, SLOT(addData(QList<double>, QList<QList<double> >)));
    connect(_pGraphDataModel, SIGNAL(activeChanged(quint32)), this, SLOT(rebuildGraphMenu()));
    connect(_pGraphDataModel, SIGNAL(activeChanged(quint32)), _pGraphView, SLOT(updateGraphs()));
    connect(_pGraphDataModel, SIGNAL(colorChanged(quint32)), this, SLOT(handleGraphColorChange(quint32)));
    connect(_pGraphDataModel, SIGNAL(colorChanged(quint32)), _pGraphView, SLOT(changeGraphColor(quint32)));
    connect(_pGraphDataModel, SIGNAL(labelChanged(quint32)), this, SLOT(handleGraphLabelChange(quint32)));
    connect(_pGraphDataModel, SIGNAL(labelChanged(quint32)), _pGraphView, SLOT(changeGraphLabel(quint32)));
    connect(_pGraphDataModel, SIGNAL(added(quint32)), this, SLOT(rebuildGraphMenu()));
    connect(_pGraphDataModel, SIGNAL(added(quint32)), _pGraphView, SLOT(updateGraphs()));

    connect(_pGraphDataModel, SIGNAL(removed(quint32)), this, SLOT(rebuildGraphMenu()));
    connect(_pGraphDataModel, SIGNAL(removed(quint32)), _pGraphView, SLOT(updateGraphs()));

    connect(_pGuiModel, SIGNAL(watchFileChanged()), this, SLOT(enableWatchFile()));
    connect(_pParserModel, SIGNAL(dynamicSessionChanged()), this, SLOT(enableDynamicSession()));

    // Update cursor values in legend
    connect(_pGraphView, SIGNAL(cursorValueUpdate()), _pLegend, SLOT(updateDataInLegend()));

    _pGraphShowHide = _pUi->menuShowHide;
    _pGraphBringToFront = _pUi->menuBringToFront;
    _pBringToFrontGroup = new QActionGroup(this);

    this->setAcceptDrops(true);

    // For dock undock
    connect(_pUi->legendDock, SIGNAL(topLevelChanged(bool)), this, SLOT(legendWidgetUndocked(bool)));

    // For rightclick menu
    _pUi->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_pUi->customPlot, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    // Default to full auto scaling
    _pGuiModel->setxAxisScale(BasicGraphView::SCALE_AUTO);
    _pGuiModel->setyAxisScale(BasicGraphView::SCALE_AUTO);

    /* Update interface via model */
    _pGuiModel->triggerUpdate();
    _pParserModel->triggerUpdate();

    handleCommandLineArguments(cmdArguments);
}

MainWindow::~MainWindow()
{
    delete _pBringToFrontGroup;
    delete _pWatchFile;
    delete _pGraphView;
    delete _pGraphShowHide;
    delete _pGraphBringToFront;
    delete _pLoadDataFileDialog;
    if (_pParser)
    {
        delete _pParser;
    }
    delete _pParserModel;
    delete _pGuiModel;
    delete _pUi;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        _pGuiModel->setCursorValues(true);
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (!(event->modifiers() & Qt::ControlModifier))
    {
        _pGuiModel->setCursorValues(false);
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::loadDataFile()
{
    _pLoadDataFileDialog->open();
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

void MainWindow::selectImageExportFile()
{
    QString filePath;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOption(QFileDialog::HideNameFilterDetails, false);
    dialog.setDefaultSuffix("png");
    dialog.setWindowTitle(tr("Select png file"));
    dialog.setNameFilter(tr("PNG files (*.png)"));
    dialog.setDirectory(_pGuiModel->lastDir());

    /* Add question wether to save when legend is undocked */
    bool bProceed = false;
    if (_pUi->legendDock->isFloating())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save screenshot?", "The legend dock is floating, it won't be included in the screenshot. \n\nAre you sure want to proceed?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            bProceed = true;
        }
        else
        {
            bProceed = false;
        }
    }
    else
    {
        bProceed = true;
    }

    if (bProceed)
    {
        if (dialog.exec())
        {
            filePath = dialog.selectedFiles().first();
            _pGuiModel->setLastDir(QFileInfo(filePath).dir().absolutePath());

            QPixmap pixMap = this->window()->grab();
            pixMap.save(filePath);
        }
    }
}

void MainWindow::showAbout()
{
    AboutDialog aboutDialog(_pGraphView,this);

    aboutDialog.exec();
}

void MainWindow::showXAxisScaleDialog()
{
    AxisScaleDialog scaleDialog(AxisScaleDialog::AXIS_X, this);

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
    AxisScaleDialog scaleDialog(AxisScaleDialog::AXIS_Y, this);

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

    const qint32 graphIdx = _pGraphDataModel->convertToGraphIndex(pAction->data().toInt());
    _pGraphDataModel->setVisible(graphIdx, bState);
}

void MainWindow::handleGraphVisibilityChange(const quint32 graphIdx)
{
    if (_pGraphDataModel->isActive(graphIdx))
    {
        const quint32 activeIdx = _pGraphDataModel->convertToActiveGraphIndex(graphIdx);

        _pGraphShowHide->actions().at(activeIdx)->setChecked(_pGraphDataModel->isVisible(graphIdx));

        // Show/Hide corresponding "BringToFront" action
        _pGraphBringToFront->actions().at(activeIdx)->setVisible(_pGraphDataModel->isVisible((graphIdx)));

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
}

void MainWindow::handleGraphColorChange(const quint32 graphIdx)
{
    if (_pGraphDataModel->isActive(graphIdx))
    {
        const quint32 activeIdx = _pGraphDataModel->convertToActiveGraphIndex(graphIdx);

        QPixmap pixmap(20,5);
        pixmap.fill(_pGraphDataModel->color(graphIdx));

        QIcon showHideIcon = QIcon(pixmap);

        _pGraphShowHide->actions().at(activeIdx)->setIcon(showHideIcon);
        _pGraphBringToFront->actions().at(activeIdx)->setIcon(showHideIcon);
    }
}

void MainWindow::handleGraphLabelChange(const quint32 graphIdx)
{
    if (_pGraphDataModel->isActive(graphIdx))
    {
        const quint32 activeIdx = _pGraphDataModel->convertToActiveGraphIndex(graphIdx);

        _pGraphShowHide->actions().at(activeIdx)->setText(_pGraphDataModel->label(graphIdx));
        _pGraphBringToFront->actions().at(activeIdx)->setText(_pGraphDataModel->label(graphIdx));
    }
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

void MainWindow::rebuildGraphMenu()
{
    // Regenerate graph menu
    _pGraphShowHide->clear();
    _pGraphBringToFront->clear();

    QList<quint16> activeGraphList;
    _pGraphDataModel->activeGraphIndexList(&activeGraphList);

    for(qint32 activeIdx = 0; activeIdx < activeGraphList.size(); activeIdx++)
    {

        QString label = _pGraphDataModel->label(activeGraphList[activeIdx]);
        QAction * pShowHideAction = _pGraphShowHide->addAction(label);
        QAction * pBringToFront = _pGraphBringToFront->addAction(label);

        QPixmap pixmap(20,5);
        pixmap.fill(_pGraphDataModel->color(activeGraphList[activeIdx]));
        QIcon icon = QIcon(pixmap);

        pShowHideAction->setData(activeIdx);
        pShowHideAction->setIcon(icon);
        pShowHideAction->setCheckable(true);
        pShowHideAction->setChecked(_pGraphDataModel->isVisible(activeGraphList[activeIdx]));

        pBringToFront->setData(activeIdx);
        pBringToFront->setIcon(icon);
        pBringToFront->setCheckable(true);
        pBringToFront->setActionGroup(_pBringToFrontGroup);

        QObject::connect(pShowHideAction, SIGNAL(toggled(bool)), this, SLOT(menuShowHideGraphClicked(bool)));
        QObject::connect(pBringToFront, SIGNAL(toggled(bool)), this, SLOT(menuBringToFrontGraphClicked(bool)));
    }

    if (activeGraphList.size() > 0)
    {
        _pGraphShowHide->setEnabled(true);
        _pGraphBringToFront->setEnabled(true);
    }
    else
    {
        _pGraphShowHide->setEnabled(false);
        _pGraphBringToFront->setEnabled(false);
    }
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(_pGuiModel->windowTitle());
}

void MainWindow::enableWatchFile()
{
    // Setup file watcher
    if (_pGuiModel->watchFile())
    {
        _pWatchFile->enableFileWatch(_pParserModel->path());
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
    _pUi->actionDynamicSession->setChecked(_pParserModel->dynamicSession());
}

void MainWindow::updateGuiState()
{

    if (_pGuiModel->guiState() == GuiModel::INIT)
    {

        _pUi->actionLoadDataFile->setEnabled(true);
        _pUi->actionExportImage->setEnabled(false);
        _pUi->actionReloadDataFile->setEnabled(false);

        _pUi->actionWatchFile->setEnabled(false);
        _pUi->actionDynamicSession->setEnabled(false);

        _pUi->actionAutoScaleXAxis->setEnabled(false);
        _pUi->actionAutoScaleYAxis->setEnabled(false);
        _pUi->actionSetManualScaleXAxis->setEnabled(false);
        _pUi->actionSetManualScaleYAxis->setEnabled(false);
        _pUi->menuScale->setEnabled(false);

    }
    else if (_pGuiModel->guiState() == GuiModel::DATA_LOADED)
    {
        _pUi->actionLoadDataFile->setEnabled(true);
        _pUi->actionExportImage->setEnabled(true);
        _pUi->actionReloadDataFile->setEnabled(true);

        _pUi->actionWatchFile->setEnabled(true);

        _pUi->actionAutoScaleXAxis->setEnabled(true);
        _pUi->actionAutoScaleYAxis->setEnabled(true);
        _pUi->actionSetManualScaleXAxis->setEnabled(true);
        _pUi->actionSetManualScaleYAxis->setEnabled(true);
        _pUi->menuScale->setEnabled(true);

    }
}

void MainWindow::showContextMenu(const QPoint& pos)
{
    /* Don't show context menu when control key is pressed */
    if (!(QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        _pUi->menuView->popup(_pUi->customPlot->mapToGlobal(pos));
    }
}

void MainWindow::dataFileLoaded()
{
    if (_pGuiModel->dataFilePath().isEmpty())
    {
        _pGuiModel->setWindowTitleDetail("");
    }
    else
    {
        _pGuiModel->setWindowTitleDetail(QFileInfo(_pGuiModel->dataFilePath()).fileName());
    }
}

void MainWindow::updateMarkerDockVisibility()
{
    if (_pGuiModel->markerState())
    {
        splitDockWidget(_pUi->legendDock, _pUi->markerInfoDock, Qt::Vertical);
    }

    _pUi->markerInfoDock->setVisible(_pGuiModel->markerState());
}

void MainWindow::legendWidgetUndocked(bool bFloat)
{
    if (bFloat)
    {
        // TODO: fix scaling to minimum
        //_pUi->legendDock->adjustSize();
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
        _pLoadDataFileDialog->open(fileName);
    }
}

void MainWindow::loadDataFileAccepted()
{
    parseData();
}

void MainWindow::parseData()
{
    DataFileParser * pNewParser = new DataFileParser(_pParserModel);

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
        _pGraphDataModel->clear();
        _pGuiModel->setFrontGraph(-1);

        _pGraphDataModel->add(_pDataFileParser->dataLabels(), _pDataFileParser->timeRow(), _pDataFileParser->dataRows());
        _pGuiModel->setFrontGraph(0);

        _pGuiModel->setDataFilePath(QFileInfo(_pParserModel->path()).fileName());

        _pGuiModel->setxAxisScale(BasicGraphView::SCALE_AUTO);
        _pGuiModel->setyAxisScale(BasicGraphView::SCALE_AUTO);

        _pGuiModel->clearMarkersState();

        _pGuiModel->setGuiState(GuiModel::DATA_LOADED);

        bSucceeded = true;
    }

    return bSucceeded;
}

void MainWindow::updateData(void)
{
    if (_pParser->processDataFile())
    {
        _pGraphView->addData(_pParser->timeRow(), _pParser->dataRows());
    }
    else
    {
        _pGuiModel->setWindowTitleDetail(QString(tr("Load Failed - %1")).arg(QFileInfo(_pParserModel->path()).fileName()));

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

	// OpenGL argument
    QCommandLineOption openGlOption("opengl", QCoreApplication::translate("main", "Use openGL to render plot"));
    argumentParser.addOption(openGlOption);

    // Process arguments
    argumentParser.process(cmdArguments);

    bool bOpenGl = argumentParser.isSet(openGlOption);
    _pGraphView->setOpenGl(bOpenGl);

    if (!argumentParser.positionalArguments().isEmpty())
    {
        QString filename = argumentParser.positionalArguments().first();
        _pLoadDataFileDialog->open(filename);
    }
}

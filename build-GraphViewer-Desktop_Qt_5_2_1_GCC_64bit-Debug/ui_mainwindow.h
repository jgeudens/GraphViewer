/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoadDataFile;
    QAction *actionExit;
    QAction *actionReloadDataFile;
    QAction *actionAbout;
    QAction *actionExportImage;
    QAction *actionAutoScaleXAxis;
    QAction *actionAutoScaleYAxis;
    QAction *actionSetManualScaleXAxis;
    QAction *actionSetManualScaleYAxis;
    QAction *actionWatchFile;
    QAction *actionDynamicSession;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QCustomPlot *customPlot;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menu;
    QMenu *menuGraph;
    QMenu *menuView;
    QMenu *menuScale;
    QMenu *menuShowHide;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(956, 643);
        actionLoadDataFile = new QAction(MainWindow);
        actionLoadDataFile->setObjectName(QStringLiteral("actionLoadDataFile"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionReloadDataFile = new QAction(MainWindow);
        actionReloadDataFile->setObjectName(QStringLiteral("actionReloadDataFile"));
        actionReloadDataFile->setEnabled(false);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExportImage = new QAction(MainWindow);
        actionExportImage->setObjectName(QStringLiteral("actionExportImage"));
        actionExportImage->setEnabled(false);
        actionAutoScaleXAxis = new QAction(MainWindow);
        actionAutoScaleXAxis->setObjectName(QStringLiteral("actionAutoScaleXAxis"));
        actionAutoScaleXAxis->setEnabled(false);
        actionAutoScaleYAxis = new QAction(MainWindow);
        actionAutoScaleYAxis->setObjectName(QStringLiteral("actionAutoScaleYAxis"));
        actionAutoScaleYAxis->setEnabled(false);
        actionSetManualScaleXAxis = new QAction(MainWindow);
        actionSetManualScaleXAxis->setObjectName(QStringLiteral("actionSetManualScaleXAxis"));
        actionSetManualScaleXAxis->setEnabled(false);
        actionSetManualScaleYAxis = new QAction(MainWindow);
        actionSetManualScaleYAxis->setObjectName(QStringLiteral("actionSetManualScaleYAxis"));
        actionSetManualScaleYAxis->setEnabled(false);
        actionWatchFile = new QAction(MainWindow);
        actionWatchFile->setObjectName(QStringLiteral("actionWatchFile"));
        actionWatchFile->setCheckable(true);
        actionWatchFile->setEnabled(false);
        actionDynamicSession = new QAction(MainWindow);
        actionDynamicSession->setObjectName(QStringLiteral("actionDynamicSession"));
        actionDynamicSession->setCheckable(true);
        actionDynamicSession->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setMinimumSize(QSize(10, 0));

        verticalLayout->addWidget(customPlot);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 956, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menuGraph = new QMenu(menuBar);
        menuGraph->setObjectName(QStringLiteral("menuGraph"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuScale = new QMenu(menuView);
        menuScale->setObjectName(QStringLiteral("menuScale"));
        menuScale->setEnabled(false);
        menuShowHide = new QMenu(menuView);
        menuShowHide->setObjectName(QStringLiteral("menuShowHide"));
        menuShowHide->setEnabled(false);
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuGraph->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menu->menuAction());
        menuFile->addAction(actionLoadDataFile);
        menuFile->addAction(actionReloadDataFile);
        menuFile->addSeparator();
        menuFile->addAction(actionExportImage);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menu->addAction(actionAbout);
        menuGraph->addAction(actionWatchFile);
        menuGraph->addAction(actionDynamicSession);
        menuView->addAction(menuScale->menuAction());
        menuView->addAction(menuShowHide->menuAction());
        menuScale->addAction(actionAutoScaleXAxis);
        menuScale->addAction(actionAutoScaleYAxis);
        menuScale->addSeparator();
        menuScale->addAction(actionSetManualScaleXAxis);
        menuScale->addAction(actionSetManualScaleYAxis);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionLoadDataFile->setText(QApplication::translate("MainWindow", "Load Data File...", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionReloadDataFile->setText(QApplication::translate("MainWindow", "Reload Data File", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0));
        actionExportImage->setText(QApplication::translate("MainWindow", "Export Image", 0));
        actionAutoScaleXAxis->setText(QApplication::translate("MainWindow", "Auto scale x-axis", 0));
        actionAutoScaleYAxis->setText(QApplication::translate("MainWindow", "Auto scale y-axis", 0));
        actionSetManualScaleXAxis->setText(QApplication::translate("MainWindow", "Set Manual scale x-axis...", 0));
        actionSetManualScaleYAxis->setText(QApplication::translate("MainWindow", "Set Manual scale y-axis...", 0));
        actionWatchFile->setText(QApplication::translate("MainWindow", "Watch File", 0));
        actionDynamicSession->setText(QApplication::translate("MainWindow", "Dynamic Session", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menu->setTitle(QApplication::translate("MainWindow", "?", 0));
        menuGraph->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuScale->setTitle(QApplication::translate("MainWindow", "Scale", 0));
        menuShowHide->setTitle(QApplication::translate("MainWindow", "Show/Hide", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CsvGraphViewer
TEMPLATE = app


VERSION = 0.1.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEBUG_OUTPUT

INCLUDEPATH += \
    ../libraries/qcustomplot \
    ../src

SOURCES +=  \
    ../src/mainwindow.cpp \
    ../src/main.cpp \
    ../libraries/qcustomplot/qcustomplot.cpp \
    ../src/datafileparser.cpp \
    ../src/dialog.cpp \
    ../src/graphviewer.cpp \
    ../src/axisscaledialog.cpp \
    ../src/dataparsersettings.cpp

FORMS    += \
    ../src/mainwindow.ui \
    ../src/dialog.ui \
    ../src/axisscaledialog.ui

HEADERS += \
    ../src/mainwindow.h \
    ../libraries/qcustomplot/qcustomplot.h \
    ../src/datafileparser.h \
    ../src/dialog.h \
    ../src/graphviewer.h \
    ../src/util.h \
    ../src/axisscaledialog.h \
    ../src/dataparsersettings.h

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CsvGraphViewer
TEMPLATE = app


VERSION = 0.0.1

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += QT_DEBUG_OUTPUT

INCLUDEPATH += \
    ../libraries/qcustomplot \
    ../src

SOURCES +=  \
    ../src/mainwindow.cpp \
    ../src/main.cpp \
    ../libraries/qcustomplot/qcustomplot.cpp \

FORMS    += \
    ../src/mainwindow.ui

HEADERS += \
    ../src/mainwindow.h \
    ../libraries/qcustomplot/qcustomplot.h \

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphViewer
TEMPLATE = app

win32: RC_ICONS = ../icon/application.ico

VERSION = 0.3.2

DEFINES += NDEBUG

DEFINES += GIT_HASH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --short HEAD)\\\""
DEFINES += GIT_BRANCH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --abbrev-ref HEAD)\\\""

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
    ../src/graphviewer.cpp \
    ../src/axisscaledialog.cpp \
    ../src/dataparsersettings.cpp \
    ../src/loadfiledialog.cpp \
    ../src/presetparser.cpp

FORMS    += \
    ../src/mainwindow.ui \
    ../src/axisscaledialog.ui \
    ../src/loadfiledialog.ui

HEADERS += \
    ../src/mainwindow.h \
    ../libraries/qcustomplot/qcustomplot.h \
    ../src/datafileparser.h \
    ../src/graphviewer.h \
    ../src/util.h \
    ../src/axisscaledialog.h \
    ../src/dataparsersettings.h \
    ../src/loadfiledialog.h \
    ../src/presetparser.h

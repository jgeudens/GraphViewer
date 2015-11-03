QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphViewer
TEMPLATE = app

win32: RC_ICONS = ../icon/application.ico

VERSION = 0.4.0

DEFINES += DEBUG

DEFINES += GIT_HASH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --short HEAD)\\\""
DEFINES += GIT_BRANCH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --abbrev-ref HEAD)\\\""

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += QT_DEBUG_OUTPUT

INCLUDEPATH += \
    ../libraries/qcustomplot \
    ../src    \
    ../src/dialogs \
    ../src/graphview \
    ../src/models \
    ../src/parser \
    ../src/util \

SOURCES +=  \
    ../src/main.cpp \
    ../libraries/qcustomplot/qcustomplot.cpp \
    ../src/dialogs/axisscaledialog.cpp \
    ../src/dialogs/loadfiledialog.cpp \
    ../src/dialogs/mainwindow.cpp \
    ../src/graphview/basicgraphview.cpp \
    ../src/graphview/extendedgraphview.cpp \
    ../src/models/guimodel.cpp \
    ../src/parser/datafileparser.cpp \
    ../src/parser/presetparser.cpp \
    ../src/watchfile.cpp \
    ../src/models/parsermodel.cpp \
    ../src/parser/settingsauto.cpp

FORMS    += \
    ../src/dialogs/axisscaledialog.ui \
    ../src/dialogs/loadfiledialog.ui \
    ../src/dialogs/mainwindow.ui

HEADERS += \
    ../libraries/qcustomplot/qcustomplot.h \
    ../src/dialogs/axisscaledialog.h \
    ../src/dialogs/loadfiledialog.h \
    ../src/dialogs/mainwindow.h \
    ../src/graphview/basicgraphview.h \
    ../src/graphview/extendedgraphview.h \
    ../src/models/graphdata.h \
    ../src/models/guimodel.h \
    ../src/parser/datafileparser.h \
    ../src/parser/presetparser.h \
    ../src/util/util.h \
    ../src/watchfile.h \
    ../src/models/parsermodel.h \
    ../src/parser/settingsauto.h

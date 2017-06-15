QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GraphViewer
TEMPLATE = app

CONFIG += c++11

win32: RC_ICONS = ../icon/application.ico

VERSION = 1.1.0

DEFINES += DEBUG

DEFINES += GIT_HASH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --short HEAD)\\\""
DEFINES += GIT_BRANCH="\\\"$(shell git --git-dir \""$$PWD/../.git"\" rev-parse --abbrev-ref HEAD)\\\""

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += QT_DEBUG_OUTPUT
DEFINES += QCUSTOMPLOT_USE_OPENGL

INCLUDEPATH += \
    ../libraries/qcustomplot \
    ../src    \
    ../src/customwidgets \
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
    ../src/parser/settingsauto.cpp \
    ../src/util/updatenotify.cpp \
    ../src/util/versiondownloader.cpp \
    ../src/dialogs/aboutdialog.cpp \
    ../src/customwidgets/legend.cpp \
    ../src/customwidgets/legenditem.cpp \
    ../src/customwidgets/markerinfo.cpp \
    ../src/customwidgets/markerinfoitem.cpp \
    ../src/customwidgets/verticalscrollareacontents.cpp \
    ../src/models/graphdata.cpp \
    ../src/models/graphdatamodel.cpp \
    ../src/util/util.cpp \
    ../src/graphview/myqcustomplot.cpp \
    ../src/dialogs/markerinfodialog.cpp \
    ../src/graphview/myqcpaxis.cpp \
    ../src/graphview/myqcpaxistickertime.cpp

FORMS    += \
    ../src/dialogs/axisscaledialog.ui \
    ../src/dialogs/loadfiledialog.ui \
    ../src/dialogs/mainwindow.ui \
    ../src/dialogs/aboutdialog.ui \
    ../src/dialogs/markerinfodialog.ui

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
    ../src/parser/settingsauto.h \
    ../src/util/updatenotify.h \
    ../src/util/versiondownloader.h \
    ../src/dialogs/aboutdialog.h \
    ../src/customwidgets/legend.h \
    ../src/customwidgets/legenditem.h \
    ../src/customwidgets/markerinfo.h \
    ../src/customwidgets/markerinfoitem.h \
    ../src/customwidgets/verticalscrollareacontents.h \
    ../src/models/graphdatamodel.h \
    ../src/graphview/myqcustomplot.h \
    ../src/dialogs/markerinfodialog.h \
    ../src/graphview/myqcpaxis.h \
    ../src/graphview/myqcpaxistickertime.h

RESOURCES += \
    ../resources/resource.qrc

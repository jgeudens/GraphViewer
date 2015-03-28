#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(a.arguments());

    QCoreApplication::setApplicationName("GraphViewer");

    w.show();

    return a.exec();
}

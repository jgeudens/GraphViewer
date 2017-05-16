#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setApplicationName("GraphViewer");

    MainWindow w(a.arguments());

    w.show();

    return a.exec();
}

#ifndef UTIL_H
#define UTIL_H

#include "QObject"
#include "QMessageBox"

class Util : public QObject
{
    Q_OBJECT

public:

    static const quint32 cCurrentDataLevel = 1;

    static void showError(QString text)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("GraphViewer"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(text);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

    static quint32 currentDataLevel()
    {
        return cCurrentDataLevel;
    }

    static const QString & currentVersion()
    {
        static const QString version(APP_VERSION);
        return version;
    }

private:

};


#endif // UTIL_H

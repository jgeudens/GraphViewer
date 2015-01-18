#ifndef UTIL_H
#define UTIL_H

#include "QObject"
#include "QMessageBox"

class Util : public QObject
{
    Q_OBJECT

public:

    static void showError(QString text)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("CsvGraphViewer"));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(text);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }

private:

};


#endif // UTIL_H

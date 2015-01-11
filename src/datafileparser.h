#ifndef DATAFILEPARSER_H
#define DATAFILEPARSER_H

#include "QObject"
#include "QStringList"
#include "QList"
#include "QFile"

class DataFileParser : public QObject
{
    Q_OBJECT

public:
    DataFileParser();
    bool loadDataFile(QString dataFilePath);
    bool parseData(QString fieldSeparator, quint32 dataRow, quint32 dataColumn, QList<QList<double> > &dataRows);

private:

    void showError(QString text);
    bool readLineFromFile(QFile *file, QString *pLine);

    QStringList _fileContents;

};

#endif // DATAFILEPARSER_H

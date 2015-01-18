#ifndef DATAFILEPARSER_H
#define DATAFILEPARSER_H

#include "QObject"
#include "QStringList"
#include "QList"
#include "QFile"
#include "datatypes.h"

class DataFileParser : public QObject
{
    Q_OBJECT

public:
    DataFileParser();
    bool loadDataFile(QString dataFilePath);
    bool parseData(DataTypes::DataFileSettings &settings, QList<QList<double> > &dataRows, QStringList &labels);

private:

    bool readLineFromFile(QFile *file, QString *pLine);

    QStringList _fileContents;

};

#endif // DATAFILEPARSER_H

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
    void setDataFileSettings(DataTypes::DataFileSettings &settings);
    bool loadDataFile(void);
    bool parseData(QList<QList<double> > &dataRows, QStringList &labels);

private:

    bool readLineFromFile(QFile *file, QString *pLine);

    QStringList _fileContents;

   DataTypes::DataFileSettings _parseSettings;

};

#endif // DATAFILEPARSER_H

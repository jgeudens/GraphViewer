#ifndef DATAFILEPARSER_H
#define DATAFILEPARSER_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QFile>
#include "parsermodel.h"

class DataFileParser : public QObject
{
    Q_OBJECT

public:
    DataFileParser(ParserModel *pParserModel);
    ~DataFileParser();

    bool forceProcessDataFile();
    bool processDataFile();
    QList<QList<double> > & getDataRows();
    QStringList & getDataLabels();

private:

    bool readData();
    bool loadDataFile();
    bool readLabels();
    bool readLineFromFile(QFile *file, QString *pLine);
    bool IsCommentLine(QString line);

    ParserModel * _pParserModel;

    QStringList _fileContents;
    int _fileContentsEnd; // Index of last parsed line in _fileContents
    qint64 _fileEndPos; // Last position in datafile
    qint32 _expectedFields;

    QList<QList<double> > _dataRows;
    QStringList _dataLabels;
};

#endif // DATAFILEPARSER_H

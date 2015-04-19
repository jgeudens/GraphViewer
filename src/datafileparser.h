#ifndef DATAFILEPARSER_H
#define DATAFILEPARSER_H

#include "QObject"
#include "QStringList"
#include "QList"
#include "QFile"
#include "dataparsersettings.h"
#include <QFileSystemWatcher>

class DataFileParser : public QObject
{
    Q_OBJECT

public:
    DataFileParser();
    ~DataFileParser();

    DataParserSettings * getDataParseSettings();
    bool forceProcessDataFile();
    bool processDataFile();
    QList<QList<double> > & getDataRows();
    QStringList & getDataLabels();

signals:
    void fileDataChanged();
    void addFileWatchFailed(QString);

private slots:
    void fileDataChange(QString);

private:

    bool readData();
    void setupFileWatchers();
    bool loadDataFile();
    bool readLabels();
    bool readLineFromFile(QFile *file, QString *pLine);
    bool IsCommentLine(QString line);

    QStringList _fileContents;
    int _fileContentsEnd; // Index of last parsed line in _fileContents
    qint64 _fileEndPos; // Last position in datafile
    qint32 _expectedFields;

    QList<QList<double> > _dataRows;
    QStringList _dataLabels;

    DataParserSettings _parseSettings;
    QFileSystemWatcher *_pFileWatcher;

};

#endif // DATAFILEPARSER_H

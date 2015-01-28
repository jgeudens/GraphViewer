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

    DataParserSettings * getDataParseSettingsPointer();
    bool loadDataFile(void);
    bool parseData(QList<QList<double> > &dataRows, QStringList &labels);

signals:
    void fileChanged();

private slots:
    void fileChange(QString);

private:

    bool readLineFromFile(QFile *file, QString *pLine);

    QStringList _fileContents;

    DataParserSettings _parseSettings;
    QFileSystemWatcher *_fileWatcher;

};

#endif // DATAFILEPARSER_H

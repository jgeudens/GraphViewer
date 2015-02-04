
#include <QtWidgets>
#include <QMessageBox>

#include "dataparsersettings.h"
#include "util.h"
#include "datafileparser.h"

DataFileParser::DataFileParser() :
    _pFileWatcher(new QFileSystemWatcher())
{
    connect(_pFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileDataChange(QString)));
}

DataFileParser::~DataFileParser()
{
    delete _pFileWatcher;
}

DataParserSettings * DataFileParser::getDataParseSettingsPointer()
{
    return &_parseSettings;
}

// Return false on error
bool DataFileParser::loadDataFile()
{
    bool bRet = true;
    QString line;
    QFile file(_parseSettings.getPath());

    /* If we can't open it, let's show an error message. */
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        _fileContents.clear();

        do
        {
            bool bResult = readLineFromFile(&file, &line);
            if(bResult)
            {
                _fileContents.append(line);
            }
            else
            {
                break;
            }
        } while(!file.atEnd());

        if (!file.atEnd())
        {
            Util::showError(tr("Error while reading data file: %1").arg(_parseSettings.getPath()));
            bRet = false;
        }

        if(_pFileWatcher->files().length() > 0)
        {
            _pFileWatcher->removePaths(_pFileWatcher->files());
        }
        if(_pFileWatcher->directories().length() > 0)
        {
            _pFileWatcher->removePaths(_pFileWatcher->directories());
        }

        if(!_pFileWatcher->addPath(_parseSettings.getPath()))
        {
            emit addFileWatchFailed(_parseSettings.getPath());
        }
    }
    else
    {
        Util::showError(tr("Couldn't open data file: %1").arg(_parseSettings.getPath()));
        bRet = false;
    }

    return bRet;
}

void DataFileParser::fileDataChange(QString path)
{
    if(_parseSettings.getWatchFileData() && path == _parseSettings.getPath())
    {
        emit fileDataChanged();
    }
}


bool DataFileParser::parseData(QList<QList<double> > &dataRows, QStringList &labels)
{
    bool bRet = true;

    // Get number of rows (from dataRow)
    const qint32 expectedFields = _fileContents[_parseSettings.getDataRow()].split(_parseSettings.getFieldSeparator()).size() - _parseSettings.getColumn();

    // Check number of expected fields
    if (expectedFields < 2)
    {
        Util::showError(tr("Can't parse data. Are you sure field separator and column are correct?"));
        bRet = false;
    }

    //Read labels
    QStringList tmpLabels;
    if (bRet)
    {
        tmpLabels = _fileContents[_parseSettings.getLabelRow()].split(_parseSettings.getFieldSeparator());
        if ((tmpLabels.size() - (qint32)_parseSettings.getColumn()) != expectedFields)
        {
            Util::showError(tr("The number of label columns doesn't match number of data columns (while checking labels)."));
            bRet = false;
        }
    }

    if (bRet)
    {
        // Init data row QLists to empty list
        QList<double> t;
        for (qint32 i = 0; i < expectedFields; i++)
        {
            dataRows.append(t);
        }
    }

    // Process labels
    if (bRet)
    {
        for (qint32 i = _parseSettings.getColumn(); i < tmpLabels.size(); i++)
        {
            labels.append(tmpLabels[i]);
        }
    }


    if (bRet)
    {
        for (qint32 index = _parseSettings.getDataRow(); index < _fileContents.size(); index++)
        {
            // ignore empty lines
            if(_fileContents[index].simplified() != "")
            {
                QStringList paramList = _fileContents[index].split(_parseSettings.getFieldSeparator());
                if ((paramList.size() - (qint32)_parseSettings.getColumn()) != expectedFields)
                {
                    QString txt = QString(tr("The number of label columns doesn't match number of data columns (while checking data: line %1).")).arg(index + 1);
                    Util::showError(txt);
                    bRet = false;
                    break;
                }

                for (qint32 i = _parseSettings.getColumn(); i < paramList.size(); i++)
                {
                    bool bError = false;

                    // Remove group separator
                    QString tmpData = paramList[i].simplified().replace(_parseSettings.getGroupSeparator(), "");

                    // Replace decimal point if needed
                    if (QLocale::system().decimalPoint() != _parseSettings.getDecimalSeparator())
                    {
                        tmpData = tmpData.replace(_parseSettings.getDecimalSeparator(), QLocale::system().decimalPoint());
                    }

                    const double number = QLocale::system().toDouble(tmpData, &bError);
                    if (bError == false)
                    {
                        QString error = QString(tr("Invalid data (while processing data)\n\n Line %1:\n\"%2\"").arg(index + 1).arg(_fileContents[index]));
                        Util::showError(error);
                        bRet = false;
                        break;
                    }
                    else
                    {
                        dataRows[i - _parseSettings.getColumn()].append(number);
                    }
                }
            }

            // Make sure we break the loop on an error
            if (bRet == false)
            {
                break;
            }

        }
    }

    return bRet;
}


// Return false on error
bool DataFileParser::readLineFromFile(QFile * file, QString *pLine)
{
    bool bRet = false;
    char buf[512];
    qint32 lineLength;

    // Read first line of data (labels)
    lineLength = file->readLine(buf, sizeof(buf));
    if (lineLength > 0)
    {
        bRet = true;
        *pLine = QString(buf);
    }

    return bRet;
}



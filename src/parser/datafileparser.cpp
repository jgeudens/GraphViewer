#include <QtWidgets>


#include "util.h"
#include "datafileparser.h"
#include "settingsauto.h"

const QString DataFileParser::_cPattern = QString("\\s*(\\d{1,2})[\\-\\/\\s](\\d{1,2})[\\-\\/\\s](\\d{4})\\s*([0-2][0-9]):([0-5][0-9]):([0-5][0-9])[.,]?(\\d{0,3})");

DataFileParser::DataFileParser(ParserModel *pParserModel)
{
    _pParserModel = pParserModel;
    _fileContentsEnd = 0;
    _fileEndPos = 0;

    _dateParseRegex.setPattern(_cPattern);
    _dateParseRegex.optimize();
}

DataFileParser::~DataFileParser()
{
}

bool DataFileParser::forceProcessDataFile()
{
    _dataLabels.clear();
    _dataRows.clear();

    _fileEndPos = 0;
    _fileContentsEnd = 0;
    _fileContents.clear();

    return processDataFile();
}

bool DataFileParser::processDataFile()
{
    bool bRet = true;

    // Load data file
    bRet = loadDataFile();

    // read data structure, labels when on first load
    if (_dataRows.isEmpty())
    {
        if (bRet)
        {
            // Get number of rows (from dataRow)
            _expectedFields = _fileContents[_pParserModel->dataRow()].split(_pParserModel->fieldSeparator()).size() - _pParserModel->column();

            // Check number of expected fields
            if (_expectedFields < 2)
            {
                Util::showError(tr("Can't parse data. Are you sure field separator and column are correct?"));
                bRet = false;
            }
        }

        if (bRet)
        {
            bRet = readLabels();
        }

        if (bRet)
        {
            // Init data row QLists to empty list
            QList<double> t;
            for (qint32 i = 0; i < _expectedFields; i++)
            {
                _dataRows.append(t);
            }
        }

        if (bRet)
        {
            _fileContentsEnd = _pParserModel->dataRow();
        }
    }

    if (bRet)
    {
        bRet = readData();

        // Get time data from data
        _timeRow = _dataRows[0];

        // Remove time data
        _dataLabels.removeAt(0);
    }

    if (bRet)
    {
        if (_pParserModel->stmStudioCorrection())
        {
            qDebug() << "Start correction";
            correctStmStudioData();
        }
    }

    return bRet;
}

QList<QList<double> > & DataFileParser::dataRows()
{
    return _dataRows;
}

QStringList & DataFileParser::dataLabels()
{
    return _dataLabels;
}

QList<double> DataFileParser::timeRow()
{
    return _timeRow;
}

bool DataFileParser::readData()
{
    bool bRet = true;

    for (qint32 index = _fileContentsEnd; index < _fileContents.size(); index++)
    {
        // ignore empty lines and comment lines
        if(
            (!_fileContents[index].simplified().isEmpty())
            && (!isCommentLine(_fileContents[index]))
          )
        {
            QStringList paramList = _fileContents[index].split(_pParserModel->fieldSeparator());
            if ((paramList.size() - (qint32)_pParserModel->column()) != _expectedFields)
            {
                QString txt = QString(tr("The number of label columns doesn't match number of data columns (while checking data: line %1).")).arg(index + 1);
                Util::showError(txt);
                bRet = false;
                break;
            }

            for (qint32 i = _pParserModel->column(); i < paramList.size(); i++)
            {
                bool bOk = true;

                // Remove group separator
                QString tmpData = paramList[i].simplified().replace(_pParserModel->groupSeparator(), "");

                // Replace decimal point if needed
                if (QLocale::system().decimalPoint() != _pParserModel->decimalSeparator())
                {
                    tmpData = tmpData.replace(_pParserModel->decimalSeparator(), QLocale::system().decimalPoint());
                }

                double number;
                if (tmpData.simplified().isEmpty())
                {
                    number = 0;
                    bOk = false;
                }
                else if (SettingsAuto::isNaN(tmpData))
                {
                    number = qQNaN();
                }
                else
                {
                    number = QLocale::system().toDouble(tmpData, &bOk);

                    if (bOk == false)
                    {
                         // Check for date when time column
                        if ((quint32)i == _pParserModel->column())
                        {
                            number = (double)parseDateTime(tmpData, &bOk);
                        }
                    }
                }

                if (bOk == false)
                {
                    QString error = QString(tr("Invalid data (while processing data)\n\n Line %1:\n\"%2\"").arg(index + 1).arg(_fileContents[index]));
                    Util::showError(error);
                    bRet = false;
                    break;
                }
                else
                {
                    /* Only multiply for first column (time data) */
                    if (
                        ((quint32)i == _pParserModel->column())
                        && (!_pParserModel->timeInMilliSeconds())
                        )
                    {
                        number *= 1000;
                    }
                    _dataRows[i - _pParserModel->column()].append(number);
                }
            }
        }

        // Make sure we break the loop on an error
        if (bRet == false)
        {
            break;
        }

    }

    _fileContentsEnd = _dataRows[0].size();

    return bRet;
}

bool DataFileParser::loadDataFile()
{
    bool bRet = true;
    QString line;
    QFile file(_pParserModel->path());

    /* If we can't open it, let's show an error message. */
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Go to last read position in datafile
        file.seek(_fileEndPos);

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

        if (file.atEnd())
        {
            _fileEndPos = file.pos();
        }
        else
        {
            Util::showError(tr("Error while reading data file: %1").arg(_pParserModel->path()));
            bRet = false;
        }
    }
    else
    {
        Util::showError(tr("Couldn't open data file: %1").arg(_pParserModel->path()));
        bRet = false;
    }

    return bRet;
}

bool DataFileParser::readLabels()
{
   bool bRet = true;

    //Read labels
    QStringList tmpLabels;

    if (_pParserModel->labelRow() != -1)
    {
        tmpLabels = _fileContents[_pParserModel->labelRow()].split(_pParserModel->fieldSeparator());
        if ((tmpLabels.size() - (qint32)_pParserModel->column()) != _expectedFields)
        {
            Util::showError(tr("The number of label columns doesn't match number of data columns (while checking labels)."));
            bRet = false;
        }
    }
    else
    {
        for (qint32 i = 0; i < ((qint32)_pParserModel->column() + _expectedFields); i++)
        {
            tmpLabels.append(QString(""));
        }
        tmpLabels[_pParserModel->column()] = tr("Time");
    }


    if (bRet)
    {
        // Trim white spaces of labels
        for (qint32 i = 0; i < tmpLabels.size(); i++)
        {
            tmpLabels[i] = tmpLabels[i].trimmed();

            if (tmpLabels[i].isEmpty())
            {
                tmpLabels[i] = QString(tr("Unknown column %1")).arg(i);
            }
        }
    }

    // Process labels
    if (bRet)
    {
        for (qint32 i = _pParserModel->column(); i < tmpLabels.size(); i++)
        {
            _dataLabels.append(tmpLabels[i]);
        }
    }

    return bRet;
}


// Return false on error
bool DataFileParser::readLineFromFile(QFile * file, QString *pLine)
{
    bool bRet = false;
    char buf[2048];
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

bool DataFileParser::isCommentLine(QString line)
{
    bool bRet = false;

    const QString commentSequence = _pParserModel->commentSequence();
    if (!commentSequence.isEmpty())
    {
        if (line.trimmed().left(commentSequence.length()) == commentSequence)
        {
            bRet = true;
        }
    }

    return bRet;
}

qint64 DataFileParser::parseDateTime(QString rawData, bool *bOk)
{
    QRegularExpressionMatch match = _dateParseRegex.match(rawData);

    QString day;
    QString month;
    QString year;
    QString hours;
    QString minutes;
    QString seconds;
    QString milliseconds;

    if (match.hasMatch())
    {
        day = match.captured(1);
        month = match.captured(2);
        year = match.captured(3);
        hours = match.captured(4);
        minutes = match.captured(5);
        seconds = match.captured(6);
        milliseconds = match.captured(7);
    }
    if (milliseconds.isEmpty())
    {
        milliseconds = "0";
    }

    QString dateStr = QString("%1-%2-%3 %4:%5:%6.%7").arg(day,2, '0')
                        .arg(month,2, '0')
                        .arg(year)
                        .arg(hours,2, '0')
                        .arg(minutes,2, '0')
                        .arg(seconds,2, '0')
                        .arg(milliseconds,3, '0');

    const QDateTime date = QDateTime::fromString(dateStr, "dd-MM-yyyy hh:mm:ss.zzz");

    *bOk = date.isValid();

    return date.toMSecsSinceEpoch();

}

void DataFileParser::correctStmStudioData(void)
{
    for (qint32 idx = 0; idx < _dataRows.size(); idx++)
    {
        /* We need at least 3 points */
        if (_dataRows[idx].size() > 3)
        {
            /* Skip first and last point */
            for (int32_t pointIdx = 1; pointIdx < _dataRows[idx].size() - 1; pointIdx++)
            {
                const int32_t leftPoint = _dataRows[idx][pointIdx - 1];
                const int32_t refPoint = _dataRows[idx][pointIdx];
                const int32_t rightPoint = _dataRows[idx][pointIdx + 1];

                /* Only correct 16 bit variables */
                if (
                    (refPoint < 65535)
                    && (refPoint > 0)
                    )
                {
                    const uint32_t leftDiff = qAbs(leftPoint - refPoint);
                    const uint32_t rightDiff = qAbs(refPoint - rightPoint);
                    const uint32_t outerDiff = qAbs(leftPoint - rightPoint);

                    /* difference between samples should be large enough */
                    if (
                        (leftDiff > (2 * outerDiff))
                        && (rightDiff > (2 * outerDiff))
                    )
                    {
                        if (isNibbleCorrupt((quint16)refPoint, leftPoint))
                        {
                            qDebug() << "left compare: ref: " << refPoint << ", compare: " << leftPoint;

                            _dataRows[idx][pointIdx] = leftPoint;
                        }
                        else if (isNibbleCorrupt((quint16)refPoint, rightPoint))
                        {
                            qDebug() << "right compare: ref: " << refPoint << ", compare: " << rightPoint;

                            _dataRows[idx][pointIdx] = rightPoint;
                        }
                        else
                        {
                            /* No change needed */
                        }

                    }

                }
            }

        }
    }
}

bool DataFileParser::isNibbleCorrupt(quint16 ref, quint16 compare)
{
    if (
            /* Zeroed nibbles */
            (ref == (compare & 0xFF00))
            || (ref == (compare & 0x00FF))
            /* Nibbles set to ones */
            || (ref == (compare | 0xFF00))
            || (ref == (compare | 0x00FF))
            || (ref == 0)
        )
    {
        return true;
    }
    return false;
}

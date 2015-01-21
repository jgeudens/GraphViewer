
#include <QtWidgets>
#include <QMessageBox>

#include "datatypes.h"
#include "util.h"
#include "datafileparser.h"

DataFileParser::DataFileParser()
{
}

void DataFileParser::setDataFileSettings(DataTypes::DataFileSettings &settings)
{
    _parseSettings.column = settings.column;
    _parseSettings.dataRow = settings.dataRow;
    _parseSettings.fieldSeparator = settings.fieldSeparator;
    _parseSettings.groupSeparator = settings.groupSeparator;
    _parseSettings.decimalSeparator = settings.decimalSeparator;
    _parseSettings.labelRow = settings.labelRow;
    _parseSettings.path = settings.path;
}


// Return false on error
bool DataFileParser::loadDataFile()
{
    bool bRet = true;
    QString line;
    QFile* file = new QFile(_parseSettings.path);

    /* If we can't open it, let's show an error message. */
    if (file->open(QIODevice::ReadOnly | QIODevice::Text))
    {

        _fileContents.clear();

        bool bResult = readLineFromFile(file, &line);
        while (bResult)
        {
            _fileContents.append(line);

            // Check end of file
            if (file->atEnd())
            {
                break;
            }

            // Read next line
            bResult = readLineFromFile(file, &line);
        }

        if (!bResult)
        {
            Util::showError(tr("Error while reading data file: %1").arg(_parseSettings.path));
            bRet = false;
        }
    }
    else
    {
        Util::showError(tr("Couldn't open data file: %1").arg(_parseSettings.path));
        bRet = false;
    }

    return bRet;
}


bool DataFileParser::parseData(QList<QList<double> > &dataRows, QStringList &labels)
{
    bool bRet = true;

    // Get number of rows (from dataRow)
    const qint32 expectedFields = _fileContents[_parseSettings.dataRow].split(_parseSettings.fieldSeparator).size() - _parseSettings.column;

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
        tmpLabels = _fileContents[_parseSettings.labelRow].split(_parseSettings.fieldSeparator);
        if ((tmpLabels.size() - (qint32)_parseSettings.column) != expectedFields)
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
        for (qint32 i = _parseSettings.column; i < tmpLabels.size(); i++)
        {
            labels.append(tmpLabels[i]);
        }
    }


    if (bRet)
    {
        for (qint32 index = _parseSettings.dataRow; index < _fileContents.size(); index++)
        {
            QStringList paramList = _fileContents[index].split(_parseSettings.fieldSeparator);
            if ((paramList.size() - (qint32)_parseSettings.column) != expectedFields)
            {
                QString txt = QString(tr("The number of label columns doesn't match number of data columns (while checking data: line %1).")).arg(index + 1);
                Util::showError(txt);
                bRet = false;
                break;
            }

            for (qint32 i = _parseSettings.column; i < paramList.size(); i++)
            {
                bool bError = false;

                // Remove group separator
                QString tmpData = paramList[i].simplified().replace(_parseSettings.groupSeparator, "");

                // Replace decimal point if needed
                if (QLocale::system().decimalPoint() != _parseSettings.decimalSeparator)
                {
                    tmpData = tmpData.replace(_parseSettings.decimalSeparator, QLocale::system().decimalPoint());
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
                    dataRows[i - _parseSettings.column].append(number);
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



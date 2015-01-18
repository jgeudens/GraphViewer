
#include <QtWidgets>
#include <QMessageBox>

#include "datatypes.h"
#include "util.h"
#include "datafileparser.h"

DataFileParser::DataFileParser()
{
}


// Return false on error
bool DataFileParser::loadDataFile(QString dataFilePath)
{
    bool bRet = true;
    QString line;
    QFile* file = new QFile(dataFilePath);

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
            Util::showError(tr("Error while reading data file: %1").arg(dataFilePath));
            bRet = false;
        }
    }
    else
    {
        Util::showError(tr("Couldn't open data file: %1").arg(dataFilePath));
        bRet = false;
    }

    return bRet;
}


bool DataFileParser::parseData(DataTypes::DataFileSettings &settings, QList<QList<double> > &dataRows, QStringList &labels)
{
    QString line;
    bool bRet = true;

    // Get number of rows (from dataRow)
    const qint32 expectedFields = _fileContents[settings.dataRow].split(settings.fieldSeparator).size() - settings.column;

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
        tmpLabels = _fileContents[settings.labelRow].split(settings.fieldSeparator);
        if ((tmpLabels.size() - (qint32)settings.column) != expectedFields)
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
        for (qint32 i = settings.column; i < tmpLabels.size(); i++)
        {
            labels.append(tmpLabels[i]);
        }
    }


    if (bRet)
    {
        for (qint32 index = settings.dataRow; index < _fileContents.size(); index++)
        {
            QStringList paramList = _fileContents[index].split(settings.fieldSeparator);
            if ((paramList.size() - (qint32)settings.column) != expectedFields)
            {
                QString txt = QString(tr("The number of label columns doesn't match number of data columns (while checking data: line %1).")).arg(index + 1);
                Util::showError(txt);
                bRet = false;
                break;
            }

            for (qint32 i = settings.column; i < paramList.size(); i++)
            {
                bool bError = false;
                const double number = paramList[i].toDouble(&bError);

                if (bError == false)
                {
                    QString error = QString(tr("Invalid data (while processing data)\n Line:\"%1\"").arg(line));
                    Util::showError(error);
                    bRet = false;
                    break;
                }
                else
                {
                    dataRows[i - settings.column].append(number);
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



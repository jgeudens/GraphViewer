
#include <QtWidgets>
#include <QMessageBox>
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
            showError(tr("Error while reading data file: %1").arg(dataFilePath));
            bRet = false;
        }
    }
    else
    {
        showError(tr("Couldn't open data file: %1").arg(dataFilePath));
        bRet = false;
    }

    return bRet;
}


bool DataFileParser::parseData(QString fieldSeparator, quint32 dataRow, quint32 dataColumn, QList<QList<double> > &dataRows)
{
    QString line;
    bool bRet = true;

    // Get number of rows
    const quint32 expectedFields = _fileContents[dataRow].split(fieldSeparator).size() - dataColumn;

    // Init data row QLists to empty list
    QList<double> t;
    for (quint32 i = 0; i < expectedFields; i++)
    {
        dataRows.append(t);
    }

    for (quint32 index = dataRow; index < _fileContents.size(); index++)
    {
        const quint32 lineIndex = index - 1;

        QStringList paramList = _fileContents[lineIndex].split(fieldSeparator);
        if (paramList.size() != (qint32)expectedFields)
        {
            bRet = false;
            break;
        }

        for (qint32 i = 0; i < paramList.size(); i++)
        {
            bool bError = false;
            const double number = paramList[i].toDouble(&bError);

            if (bError == false)
            {
                QString error = QString(tr("Invalid data (while processing data)\n Line:\"%1\"").arg(line));
                showError(error);
                bRet = false;
                break;
            }
            else
            {
                dataRows[i].append(number);
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

void DataFileParser::showError(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("CsvGraphViewer"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}



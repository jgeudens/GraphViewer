
#include "dataparsersettings.h"

DataParserSettings::DataParserSettings()
{

}

void DataParserSettings::copyTo(DataParserSettings * pDest)
{
    pDest->setPath(_path);
    pDest->setFieldSeparator(_fieldSeparator);
    pDest->setGroupSeparator(_groupSeparator);
    pDest->setDecimalSeparator(_decimalSeparator);
    pDest->setDataRow(_dataRow);
    pDest->setColumn(_column);
    pDest->setLabelRow(_labelRow);
    pDest->setDynamicSession(_bDynamicSession);
}

QString DataParserSettings::getPath()
{
    return _path;
}

QString DataParserSettings::getFieldSeparator()
{
    return _fieldSeparator;
}

QString DataParserSettings::getGroupSeparator()
{
    return _groupSeparator;
}

QString DataParserSettings::getDecimalSeparator()
{
    return _decimalSeparator;
}

quint32 DataParserSettings::getDataRow()
{
    return _dataRow;
}

quint32 DataParserSettings::getColumn()
{
    return _column;
}

quint32 DataParserSettings::getLabelRow()
{
    return _labelRow;
}

bool DataParserSettings::getDynamicSession()
{
    return _bDynamicSession;
}

bool DataParserSettings::getWatchFileData()
{
    return _bWatchFileData;
}



void DataParserSettings::setPath(QString path)
{
    _path = path;
}

void DataParserSettings::setFieldSeparator(QString fieldSeparator)
{
    _fieldSeparator = fieldSeparator;
}

void DataParserSettings::setGroupSeparator(QString groupSeparator)
{
    _groupSeparator = groupSeparator;
}

void DataParserSettings::setDecimalSeparator(QString decimalSeparator)
{
    _decimalSeparator = decimalSeparator;
}

void DataParserSettings::setDataRow(quint32 dataRow)
{
    _dataRow = dataRow;
}

void DataParserSettings::setColumn(quint32 column)
{
    _column = column;
}

void DataParserSettings::setLabelRow(quint32 labelRow)
{
    _labelRow = labelRow;
}

void DataParserSettings::setDynamicSession(bool bDynamicSession)
{
    _bDynamicSession = bDynamicSession;

    emit dynamicSessionChanged(_bDynamicSession);
}

void DataParserSettings::setWatchFileData(bool bWatchFileData)
{
    if(_bWatchFileData != bWatchFileData)
    {
        _bWatchFileData = bWatchFileData;
        emit watchFileDataChanged(_bWatchFileData);
    }
}

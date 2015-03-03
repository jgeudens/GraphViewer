#ifndef DataParserSettings_H
#define DataParserSettings_H

#include "QObject"

class DataParserSettings : public QObject
{
    Q_OBJECT

public:

    DataParserSettings();

    void copyTo(DataParserSettings *pDest);

    QString getPath();
    QString getFieldSeparator();
    QString getGroupSeparator();
    QString getDecimalSeparator();
    quint32 getDataRow();
    quint32 getColumn();
    qint32 getLabelRow();
    bool getDynamicSession();
    bool getWatchFileData();

    void setPath(QString path);
    void setFieldSeparator(QString fieldSeparator);
    void setGroupSeparator(QString groupSeparator);
    void setDecimalSeparator(QString decimalSeparator);
    void setDataRow(quint32 dataRow);
    void setColumn(quint32 column);
    void setLabelRow(qint32 labelRow);
    void setDynamicSession(bool bDynamicSession);
    void setWatchFileData(bool bWatchFileData);

signals:
    void dynamicSessionChanged(bool);
    void watchFileDataChanged(bool);

private:

    QString _path;
    QString _fieldSeparator;
    QString _groupSeparator;
    QString _decimalSeparator;
    quint32 _dataRow;
    quint32 _column;
    qint32 _labelRow;
    bool _bDynamicSession;
    bool _bWatchFileData;

};



#endif // DataParserSettings_H

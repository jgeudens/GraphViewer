#ifndef Datatypes_H
#define Datatypes_H

#include "QObject"

class DataTypes : public QObject
{
    Q_OBJECT

public:

    typedef struct
    {
        QString path;
        QString fieldSeparator;
        quint32 dataRow;
        quint32 column;
        quint32 labelRow;
    } DataFileSettings;

private:

};



#endif // Datatypes_H

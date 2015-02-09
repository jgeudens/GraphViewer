#ifndef PRESETPARSER_H
#define PRESETPARSER_H

#include <QObject>
#include <QMessageBox>
#include <QDomDocument>

class PresetParser : public QObject
{
    Q_OBJECT
public:
		
    typedef struct _Preset
    {
        _Preset() : bName(false), bFieldSeparator(false), bDecimalSeparator(false),
                    bThousandSeparator(false), bColumn(false), bLabelRow(false),
                    bDataRow(false), bDynamicSession(false), bKeyword(false) {}

        bool bName;
        QString name;

        bool bFieldSeparator;
        QString fieldSeparator;

        bool bDecimalSeparator;
        QString decimalSeparator;

        bool bThousandSeparator;
        QString thousandSeparator;

        bool bColumn;
        quint32 column;
        
        bool bLabelRow;
        quint32 labelRow;
        
        bool bDataRow;
        quint32 dataRow;
        
        bool bDynamicSession;

        bool bKeyword;
        QString keyword;
        
    } Preset;

    explicit PresetParser();

    bool parseFile(QIODevice *device, QList<Preset> *pPresetList);

signals:

public slots:

private:

    bool parsePresetTag(const QDomElement &element, Preset *pPreset);

    QDomDocument _domDocument;

    QMessageBox _msgBox;

};

#endif // PRESETPARSER_H

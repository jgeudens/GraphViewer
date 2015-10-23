
#include "dataparsersettings.h"

DataParserSettings::DataParserSettings(ParserModel *pParserModel)
{
    _pParserModel = pParserModel;
}

QList<QStringList> DataParserSettings::previewData()
{
    return _previewData;
}

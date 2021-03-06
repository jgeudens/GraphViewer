
#include <QtWidgets>
#include "util.h"
#include "presetparser.h"

const QString PresetParser::_presetFilename = QString("presets.xml");

PresetParser::PresetParser()
{
}

QList<PresetParser::Preset> PresetParser::presetList()
{
    return _presetList;
}

void PresetParser::loadPresetsFromFile()
{
    QString presetFile;
    /* Check if preset file exists (2 locations)
    *   <document_folder>\GraphViewer\
    *   directory of executable
    */
    QString documentsfolder = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();

    presetFile = documentsfolder + "/GraphViewer/" + _presetFilename;
    if (!QFileInfo(presetFile).exists())
    {
        // xml in documents folder doesn't exist, check directory of executable
        presetFile = _presetFilename;
        if (!QFileInfo(presetFile).exists())
        {
            presetFile = "";
            _lastModified = QDateTime();
        }
    }

    if (presetFile != "")
    {
        if (_lastModified != QFileInfo(presetFile).lastModified())
        {
            _lastModified = QFileInfo(presetFile).lastModified();

            _presetList.clear();

            QFile file(presetFile);

            /* If we can't open it, let's show an error message. */
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                PresetParser presetParser;
                if (presetParser.parseFile(&file, &_presetList))
                {
                    // Parsing failed
                }

                file.close();
            }
            else
            {
                Util::showError(tr("Couldn't open preset file: %1").arg(presetFile));
            }
        }

    }
    else
    {
        _presetList.clear();
    }
}

bool PresetParser::parseFile(QIODevice *device, QList<Preset> *pPresetList)
{
    bool bRet = true;
    QString errorStr;
    qint32 errorLine;
    qint32 errorColumn;

    if (!_domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        Util::showError(tr("Parse error at line %1, column %2:\n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorStr));

        bRet = false;
    }
    else
    {
        QDomElement root = _domDocument.documentElement();
        if (root.tagName() != "graphviewer")
        {
            Util::showError(tr("The file is not a valid graphviewer preset file."));
            bRet = false;
        }
        else
        {
            QDomElement tag = root.firstChildElement();
            while (!tag.isNull())
            {
                if (tag.tagName() == "parsepreset")
                {
		                Preset preset;
                    bRet = parsePresetTag(tag, &preset);
                    if (bRet)
                    {
                        pPresetList->append(preset);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                	// unknown: ignore
                }
                tag = tag.nextSiblingElement();
            }
        }
    }

    return bRet;
}


bool PresetParser::parsePresetTag(const QDomElement &element, Preset *pPreset)
{
    bool bFieldseparator = false;
    bool bName = false;
    bool bDecimalSeparator = false;

    bool bRet = true;
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        if (child.tagName() == "name")
        {
            bName = true;
            pPreset->name = child.text();
        }
        else if (child.tagName() == "fieldseparator")
        {
            bFieldseparator = true;
            if (child.text().trimmed().compare(QString("\\t"), Qt::CaseInsensitive) == 0)
            {
                pPreset->fieldSeparator = '\t';
            }
            else if (child.text().size() > 0)
            {
                pPreset->fieldSeparator = child.text().at(0);
            }
            else
            {
                // No data
                bRet = false;
                Util::showError(tr("Field separator is empty."));
                break;
            }
        }
        else if (child.tagName() == "decimalseparator")
        {
            bDecimalSeparator = true;
            if (child.text().size() > 0)
            {
                pPreset->decimalSeparator = child.text().at(0);
            }
            else
            {
                bRet = false;
                Util::showError(tr("Decimal separator is empty."));
                break;
            }
        }
        else if (child.tagName() == "thousandseparator")
        {
            if (child.text().size() > 0)
            {
                pPreset->thousandSeparator = child.text().at(0);
            }
            else
            {
                pPreset->thousandSeparator = ' ';
            }
        }
        else if (child.tagName() == "commentSequence")
        {
            pPreset->commentSequence = child.text();
        }
        else if (child.tagName() == "column")
        {
            pPreset->column = child.text().toUInt(&bRet);
            if (!bRet)
            {
                Util::showError(tr("Column ( %1 ) is not a valid number.").arg(child.text()));
                break;
            }
        }
        else if (child.tagName() == "labelrow")
        {
            pPreset->labelRow = child.text().toInt(&bRet);
            if (
                (!bRet)
                || (pPreset->labelRow < -1)
               )
            {
                Util::showError(tr("Label row ( %1 ) is not a valid number. Specify -1 to indicate the absence of label row").arg(child.text()));
                break;
            }
        }
        else if (child.tagName() == "datarow")
        {
            pPreset->dataRow = child.text().toUInt(&bRet);
            if (!bRet)
            {
                Util::showError(tr("Data row ( %1 ) is not a valid number.").arg(child.text()));
                break;
            }
        }
        else if (child.tagName() == "dynamicsession")
        {
            if (!child.text().toLower().compare("true"))
            {
                pPreset->bDynamicSession = true;
            }
            else
            {
                pPreset->bDynamicSession = false;
            }
        }
        else if (child.tagName() == "timeinmilliseconds")
        {
            if (!child.text().toLower().compare("true"))
            {
                pPreset->bTimeInMilliSeconds = true;
            }
            else
            {
                pPreset->bTimeInMilliSeconds = false;
            }
        }
        else if (child.tagName() == "stmstudiocorrection")
        {
            if (!child.text().toLower().compare("true"))
            {
                pPreset->bStmStudioCorrection = true;
            }
            else
            {
                pPreset->bStmStudioCorrection = false;
            }
        }
        else if (child.tagName() == "keyword")
        {
            pPreset->keyword = child.text();
        }
        else
        {
            // unkown tag: ignore
        }
        child = child.nextSiblingElement();
    }

    if (!bName)
    {
        Util::showError(tr("Name is not specified."));
        bRet = false;
    }
    else if (!bFieldseparator)
    {
        Util::showError(tr("Field separator is not specified (%1).").arg(pPreset->name));
        bRet = false;
    }
    else if (!bDecimalSeparator)
    {
        Util::showError(tr("Decimal separator is not specified (%1).").arg(pPreset->name));
        bRet = false;
    }

    return bRet;
}

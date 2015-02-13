
#include <QtWidgets>
#include "util.h"
#include "presetparser.h"

PresetParser::PresetParser()
{
    _msgBox.setWindowTitle(tr("GraphViewer preset file load error"));
    _msgBox.setIcon(QMessageBox::Warning);
}


bool PresetParser::parseFile(QIODevice *device, QList<Preset> *pPresetList)
{
    bool bRet = true;
    QString errorStr;
    qint32 errorLine;
    qint32 errorColumn;

    if (!_domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))
    {
        _msgBox.setText(tr("Parse error at line %1, column %2:\n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorStr));
        _msgBox.exec();

        bRet = false;
    }
    else
    {
        QDomElement root = _domDocument.documentElement();
        if (root.tagName() != "graphviewer")
        {
            _msgBox.setText(tr("The file is not a valid graphviewer preset file."));
            _msgBox.exec();
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
    bool bRet = true;
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        if (child.tagName() == "name")
        {
            pPreset->bName = true;
            pPreset->name = child.text();
        }
        else if (child.tagName() == "fieldseparator")
        {
            pPreset->bFieldSeparator = true;
            pPreset->fieldSeparator = child.text();
        }
        else if (child.tagName() == "decimalseparator")
        {
            pPreset->bDecimalSeparator = true;
            pPreset->decimalSeparator = child.text();
        }
        else if (child.tagName() == "thousandseparator")
        {
            pPreset->bThousandSeparator = true;
            pPreset->thousandSeparator = child.text();
        }
        else if (child.tagName() == "column")
        {
            pPreset->bColumn = true;
            pPreset->column = child.text().toUInt(&bRet);
            if (!bRet)
            {
                _msgBox.setText(tr("Column ( %1 ) is not a valid number.").arg(child.text()));
                _msgBox.exec();
                break;
            }
        }
        else if (child.tagName() == "labelrow")
        {
            pPreset->bLabelRow = true;
            pPreset->labelRow = child.text().toUInt(&bRet);
            if (!bRet)
            {
                _msgBox.setText(tr("Label row ( %1 ) is not a valid number.").arg(child.text()));
                _msgBox.exec();
                break;
            }
        }
        else if (child.tagName() == "datarow")
        {
            pPreset->bDataRow = true;
            pPreset->dataRow = child.text().toUInt(&bRet);
            if (!bRet)
            {
                _msgBox.setText(tr("Data row ( %1 ) is not a valid number.").arg(child.text()));
                _msgBox.exec();
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
        else if (child.tagName() == "keyword")
        {
            pPreset->bKeyword = true;
            pPreset->keyword = child.text();
        }
        else
        {
            // unkown tag: ignore
        }
        child = child.nextSiblingElement();
    }

    return bRet;
}

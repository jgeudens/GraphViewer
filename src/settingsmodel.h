#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>

class SettingsModel : public QObject
{
    Q_OBJECT
public:
    explicit SettingsModel(QObject *parent = 0);
    ~SettingsModel();

    void triggerUpdate(void);

    bool GraphVisibility(quint32 index) const;
    void setGraphVisibility(quint32 index, const bool &value);

    quint32 frontGraph() const;
    void setFrontGraph(const quint32 &frontGraph);

    QString loadedFile() const;
    void setLoadedFile(const QString &loadedFile);

    bool WatchFile() const;
    void setWatchFile(bool bWatchFile);

    bool DynamicSession() const;
    void setDynamicSession(bool bDynamicSession);

    bool HighlightSamples() const;
    void setHighlightSamples(bool bHighlightSamples);

    bool ValueTooltip() const;
    void setValueTooltip(bool bValueTooltip);

signals:
    void settingsChanged();

    void graphVisibilityChanged(const quint32 index);
    void frontGraphChanged();
    void loadedFileChanged();
    void highlightSamplesChanged();
    void valueTooltipChanged();


public slots:


private:

    QList<bool> _graphVisibility; /* TODO */

    quint32 _frontGraph; /* TODO */

    QString _loadedFile; /* TODO */

    bool _bWatchFile; /* TODO */
    bool _bDynamicSession; /* TODO */

    bool _bHighlightSamples; /* TODO */
    bool _bValueTooltip; /* TODO */
};

#endif // SETTINGSMODEL_H

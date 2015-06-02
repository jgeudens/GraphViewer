#ifndef WATCHFILE_H
#define WATCHFILE_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>
#include "settingsmodel.h"

class WatchFile : public QObject
{
    Q_OBJECT
public:
    explicit WatchFile(SettingsModel * pModel);
    ~WatchFile();

    void disableFileWatch();
    void enableFileWatch(QString path);

signals:
    void fileDataChanged();

private slots:
    void fileDataChange();
    void dynamicUpdate();

private:
    void clearWatchList();

    SettingsModel * _pModel;
    QFileSystemWatcher *_pFileWatcher;

    QTimer _dynamicUpdateTimer;

    static const int cDynamicMaxUpdateInterval;
};

#endif // WATCHFILE_H

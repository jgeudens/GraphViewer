#ifndef WATCHFILE_H
#define WATCHFILE_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>
#include "guimodel.h"

class WatchFile : public QObject
{
    Q_OBJECT
public:
    explicit WatchFile(GuiModel * pGuiModel);
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

    GuiModel * _pGuiModel;
    QFileSystemWatcher *_pFileWatcher;

    QTimer _dynamicUpdateTimer;

    static const int cDynamicMaxUpdateInterval;
};

#endif // WATCHFILE_H

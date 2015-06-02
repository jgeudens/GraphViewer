#include <QMutex>
#include <QFile>
#include <QMessageBox>

#include "util.h"
#include "QDebug"
#include "watchfile.h"

const int WatchFile::cDynamicMaxUpdateInterval = 100;

WatchFile::WatchFile(SettingsModel * pModel):
    _pFileWatcher(new QFileSystemWatcher())
{
    _pModel = pModel;

    connect(_pFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileDataChange()));
}

WatchFile::~WatchFile()
{
    _dynamicUpdateTimer.stop();
    delete _pFileWatcher;
}

void WatchFile::fileDataChange()
{
    if(_pModel->watchFile())
    {
        if (!_dynamicUpdateTimer.isActive())
        {
            _dynamicUpdateTimer.singleShot(cDynamicMaxUpdateInterval, this, SLOT(dynamicUpdate()));
        }
    }
}

void WatchFile::dynamicUpdate()
{
    if(_pModel->watchFile())
    {
        static QMutex mutex;

        if(mutex.tryLock())
        {
            qDebug() << "@dynamicUpdate: " << _pFileWatcher->files();
            //QFile file(_pFileWatcher->files().first());
            //if(file.size() > 0)
            {
                if(_pModel->dynamicSession())
                {
                    emit fileDataChanged();
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Data file changed");
                    msgBox.setText("Reload data file? Press cancel to disable the auto reload  function.");
                    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    int button = msgBox.exec();

                    if(button == QMessageBox::Yes)
                    {
                        emit fileDataChanged();
                    }
                    else if(button == QMessageBox::Cancel)
                    {
                        _pModel->setWatchFile(false);
                    }
                }
            }

            mutex.unlock();
        }
    }
}


void WatchFile::disableFileWatch()
{
    clearWatchList();
}

void WatchFile::enableFileWatch(QString path)
{
    clearWatchList();

    // add new watcher
    if(!_pFileWatcher->addPath(path))
    {
        Util::showError("Failed to watch \"" + path + "\". Please check your system configuration!");
    }

    qDebug() << "@init: " << _pFileWatcher->files();
}

void WatchFile::clearWatchList()
{
    // Remove existing watchers
    if(_pFileWatcher->files().length() > 0)
    {
        _pFileWatcher->removePaths(_pFileWatcher->files());
    }

    if(_pFileWatcher->directories().length() > 0)
    {
        _pFileWatcher->removePaths(_pFileWatcher->directories());
    }
}

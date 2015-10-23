#include <QMutex>
#include <QFile>
#include <QMessageBox>

#include "util.h"
#include "watchfile.h"

const int WatchFile::cDynamicMaxUpdateInterval = 100;

WatchFile::WatchFile(GuiModel * pGuiModel, ParserModel * pParserModel):
    _pFileWatcher(new QFileSystemWatcher())
{
    _pGuiModel = pGuiModel;
    _pParserModel = pParserModel;

    connect(_pFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileDataChange()));
}

WatchFile::~WatchFile()
{
    _dynamicUpdateTimer.stop();
    delete _pFileWatcher;
}

void WatchFile::fileDataChange()
{
    if(_pGuiModel->watchFile())
    {
        if (!_dynamicUpdateTimer.isActive())
        {
            _dynamicUpdateTimer.singleShot(cDynamicMaxUpdateInterval, this, SLOT(dynamicUpdate()));
        }
    }
}

void WatchFile::dynamicUpdate()
{
    if(_pGuiModel->watchFile())
    {
        static QMutex mutex;

        if(mutex.tryLock())
        {
            /*
             * Known issue: some editors (on Linux) don't edit a file, they create a copy to edit and then copy over the original file
             * QT will detect a removal of the file and then QT will remove the file from the list
             *
             * http://stackoverflow.com/questions/18300376/qt-qfilesystemwatcher-singal-filechanged-gets-emited-only-once
            */

            if(_pParserModel->dynamicSession())
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
                    _pGuiModel->setWatchFile(false);
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

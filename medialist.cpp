#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QDebug>
#include "singletons.h"
#include "medialist.h"

MediaList::MediaList(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
}

void MediaList::dragEnterEvent ( QDragEnterEvent * event )
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            if(!checkFileExtension(url.path()))
                return;
        }
        event->acceptProposedAction();
    }
}

void MediaList::dropEvent ( QDropEvent * event )
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            if(!checkFileExtension(url.path()))
                return;

            Singletons::tripController()->textEvent(eMediaFileAdd, url.path());
        }
    }
    event->acceptProposedAction();
}

void MediaList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

bool MediaList::checkFileExtension(const QString& file)
{
    QString lowercase = file.toLower();
    if(lowercase.endsWith("jpg") ||
       lowercase.endsWith("avi") ||
       lowercase.endsWith("png") ||
       lowercase.endsWith("mov") ||
       lowercase.endsWith("wmv") ||
       lowercase.endsWith("mpg") ||
       lowercase.endsWith("jpeg"))
        return true;

    return false;
}

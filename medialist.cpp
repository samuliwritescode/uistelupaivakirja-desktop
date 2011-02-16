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
            if(!url.path().endsWith("gpx"))
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
            if(!url.path().endsWith("gpx"))
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

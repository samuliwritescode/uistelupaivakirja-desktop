#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDebug>
#include <QUrl>
#include "waypointlist.h"
#include "singletons.h"

WayPointList::WayPointList(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setAlternatingRowColors(true);
}

void WayPointList::dragEnterEvent ( QDragEnterEvent * event )
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

void WayPointList::dropEvent ( QDropEvent * event )
{
    qDebug() << "drop";
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            insertFile(url);
        }
    }
    event->acceptProposedAction();
}

void WayPointList::insertFile(const QUrl& url)
{
    if(!url.path().endsWith("gpx"))
        return;

    qDebug() << "drop"<<url.path();
    Singletons::tripController()->textEvent(eWaypointsAdd, url.path());
}

void WayPointList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

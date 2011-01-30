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
            if(!url.path().endsWith("gpx"))
                return;

            qDebug() << "drop"<<url.path();
            Singletons::tripController()->textEvent(eWaypointsAdd, url.path());
        }
    }
    event->acceptProposedAction();
}

void WayPointList::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

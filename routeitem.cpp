#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QStandardItemModel>
#include "singletons.h"
#include "routeitem.h"

RouteItem::RouteItem(QLabel *parent) :
    QLabel(parent)
{
    setMinimumSize(100,0);
    setText("");
    setWordWrap(true);
}

RouteItem::RouteItem(const QString& text, QLabel *parent):
        QLabel(text, parent)
{

}


void RouteItem::dragEnterEvent ( QDragEnterEvent * event )
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

void RouteItem::dropEvent ( QDropEvent * event )
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            if(!url.path().endsWith("gpx"))
                return;

            Singletons::tripController()->textEvent(eRouteAdd, url.path());
        }
    }
    event->acceptProposedAction();
}

void RouteItem::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void RouteItem::setText(const QString & text)
{
    if(text.isEmpty())
    {
        QLabel::setText(tr("Raahaa reittiloki"));
        setStyleSheet("background-color: rgb(186, 95, 95)");
    }
    else
    {
        QLabel::setText(text);
        setStyleSheet("background-color: rgb(95, 186, 95)");
    }
}

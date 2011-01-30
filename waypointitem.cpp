#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QStandardItemModel>
#include "waypointitem.h"
#include "singletons.h"

WayPointItem::WayPointItem(QLabel *parent) :
    QLabel(parent)
{
    setMinimumSize(100,0);
    setText("");
}

WayPointItem::WayPointItem(const QString& text, QLabel *parent):
        QLabel(text, parent)
{

}


void WayPointItem::dragEnterEvent ( QDragEnterEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QStandardItemModel model;
        model.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
        if( !model.item(0,0)->data(Qt::UserRole+2).isNull())
        {
            event->acceptProposedAction();
        }
    }
}

void WayPointItem::dropEvent ( QDropEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QStandardItemModel model;
        model.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
        int index = model.item(0,0)->data(Qt::UserRole+2).toInt();
        Singletons::tripController()->intEvent(eWayPointSet, index);
        event->acceptProposedAction();
    }
}

void WayPointItem::setText(const QString & text)
{
    if(text.isEmpty())
    {
        QLabel::setText(tr("Raahaa reittipiste"));
        setStyleSheet("background-color: rgb(186, 95, 95)");
    }
    else
    {
        QLabel::setText(text);
        setStyleSheet("background-color: rgb(95, 186, 95)");
    }
}


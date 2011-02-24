#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QStandardItemModel>
#include "lureitem.h"
#include "singletons.h"

LureItem::LureItem(QLabel *parent) :
    QLabel(parent)
{
    setMinimumSize(100,0);
    setText("");
    setWordWrap(true);
}

LureItem::LureItem(const QString& text, QLabel *parent):
        QLabel(text, parent)
{

}


void LureItem::dragEnterEvent ( QDragEnterEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QStandardItemModel model;
        model.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
        if(!model.item(0,0)->data().isNull())
        {
            event->acceptProposedAction();
        }
    }
}

void LureItem::dropEvent ( QDropEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QStandardItemModel model;
        model.dropMimeData(event->mimeData(), Qt::CopyAction, 0,0, QModelIndex());
        int lureindex = model.item(0,0)->data().toInt();
        Singletons::tripController()->intEvent(eSelectLure, lureindex);
        event->acceptProposedAction();
    }
}

void LureItem::setText(const QString & text)
{
    if(text.isEmpty())
    {
        QLabel::setText(tr("Raahaa viehe"));
        setStyleSheet("background-color: rgb(186, 95, 95)");
    }
    else
    {
        QLabel::setText(text);
        setStyleSheet("background-color: rgb(95, 186, 95)");
    }
}

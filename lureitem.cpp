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
    setStyleSheet("background-color: rgb(186, 95, 95)");
    setMinimumSize(100,0);
    setText(tr("raahaa viehe"));
}

LureItem::LureItem(const QString& text, QLabel *parent):
        QLabel(text, parent)
{

}


void LureItem::dragEnterEvent ( QDragEnterEvent * event )
{
    if(event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->acceptProposedAction();
    }


    if(event->mimeData()->hasUrls())
    {
        qDebug() << "Has file";
        QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            qDebug() << url;
        }
        event->acceptProposedAction();
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

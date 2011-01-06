#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include "lureitem.h"

LureItem::LureItem(QLabel *parent) :
    QLabel(parent)
{
}

LureItem::LureItem(const QString& text, QLabel *parent):
        QLabel(text, parent)
{

}


void LureItem::dragEnterEvent ( QDragEnterEvent * event )
{
    qDebug() << "drag enter";
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
    qDebug() << "drop";
    event->acceptProposedAction();
}

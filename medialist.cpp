#include <QDragEnterEvent>
#include <QDesktopServices>
#include <QDropEvent>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <Qmenu>
#include "mainwindow.h"
#include "singletons.h"
#include "medialist.h"

MediaList::MediaList(QWidget *parent) :
    QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openFile(QListWidgetItem*)));
    m_removeAction = new QAction(tr("Poista reissuta"), this);
    m_openAction = new QAction(tr("Avaa ulkoisessa ohjelmassa"), this);
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

void MediaList::contextMenuEvent ( QContextMenuEvent * e )
{
    QMenu contextMenu(tr("Muokkaa mediatiedostoa"), this);

    contextMenu.addAction(m_openAction);
    contextMenu.addAction(m_removeAction);
    QAction* action = contextMenu.exec(mapToGlobal(e->pos()));
    if(action == m_removeAction)
    {
        QList<QListWidgetItem*> items = selectedItems();
        foreach(QListWidgetItem* item, items)
        {
            QString filename = item->data(Qt::UserRole+1).toString();
            Singletons::tripController()->textEvent(eMediaFileRemove, filename);
        }
    }
    else if(action == m_openAction)
    {
        QList<QListWidgetItem*> items = selectedItems();
        foreach(QListWidgetItem* item, items)
        {
            openFile( item );
        }
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

void MediaList::setMediaFiles(QStringList files)
{
    files.sort();
    clear();
    foreach(QString file, files)
    {
        QFileInfo fileinfo(file);
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(fileinfo.fileName());
        item->setData(Qt::UserRole+1, file);
        addItem(item);
    }
}

void MediaList::openFile( QListWidgetItem * item )
{
    QString filename = item->data(Qt::UserRole+1).toString();
    if(!QDesktopServices::openUrl(QUrl("file://"+filename)))
    {
        MainWindow::showErrorNotification(tr("Tiedosto ei auennut ulkoisessa ohjelmassa"));
    }
}

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
    m_removeAction = new QAction(tr("Poista reissulta"), this);
    m_openAction = new QAction(tr("Avaa ulkoisessa ohjelmassa"), this);
    setAlternatingRowColors(true);
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
    QList<QListWidgetItem*> items = selectedItems();
    if(items.count() != 1)
        return;

    QListWidgetItem* item = items.at(0);

    QMenu contextMenu(tr("Muokkaa mediatiedostoa"), this);

    contextMenu.addAction(m_openAction);
    contextMenu.addAction(m_removeAction);
    QAction* action = contextMenu.exec(mapToGlobal(e->pos()));
    if(action == m_removeAction)
    {
        QString filename = item->data(Qt::UserRole+1).toString();
        Singletons::tripController()->textEvent(eMediaFileRemove, filename);
    }
    else if(action == m_openAction)
    {
        openFile(item);
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
    if(checkMovieExtension(file) ||
       checkImageExtension(file) )
        return true;

    return false;
}

bool MediaList::checkMovieExtension(const QString& file)
{
    QString lowercase = file.toLower();
    if(lowercase.endsWith("avi") ||
       lowercase.endsWith("mov") ||
       lowercase.endsWith("wmv") ||
       lowercase.endsWith("mpg") )
        return true;

    return false;
}

bool MediaList::checkImageExtension(const QString& file)
{
    QString lowercase = file.toLower();
    if(lowercase.endsWith("jpg") ||
       lowercase.endsWith("png") ||
       lowercase.endsWith("jpeg") )
        return true;

    return false;
}

void MediaList::setMediaFiles(QStringList files)
{

    files.sort();
    clear();

    if(files.length()==0)
    {
        addItem(tr("Lisää mediatiedostoja tänne raahaamalla"));
    }

    foreach(QString file, files)
    {
        QFileInfo fileinfo(file);
        QListWidgetItem* item = new QListWidgetItem();
        if(checkMovieExtension(fileinfo.fileName()))
        {
            item->setIcon(QIcon(":/res/video-x-generic.png"));
        }else if(checkImageExtension(fileinfo.fileName()))
        {
            item->setIcon(QIcon(":/res/image-x-generic.png"));
        }
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
        Singletons::tripController()->showErrorMessage(tr("Tiedosto ei auennut ulkoisessa ohjelmassa"));
    }
}

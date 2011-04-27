#ifndef WAYPOINTLIST_H
#define WAYPOINTLIST_H

#include <QListWidget>
#include <QWidget>
#include <QUrl>

class WayPointList : public QListWidget
{
    Q_OBJECT
public:
    WayPointList(QWidget *parent = 0);
    void insertFile(const QUrl& url);

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent *event);

};

#endif // WAYPOINTLIST_H

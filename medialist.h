#ifndef MEDIALIST_H
#define MEDIALIST_H

#include <QListWidget>

class MediaList : public QListWidget
{
    Q_OBJECT
public:
    explicit MediaList(QWidget *parent = 0);

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent *event);
private:
    bool checkFileExtension(const QString&);
};

#endif // MEDIALIST_H

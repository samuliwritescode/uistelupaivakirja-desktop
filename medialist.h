#ifndef MEDIALIST_H
#define MEDIALIST_H

#include <QListWidget>
#include <QStringList>
#include <QListWidgetItem>

class MediaList : public QListWidget
{
    Q_OBJECT
public:
    explicit MediaList(QWidget *parent = 0);

    void setMediaFiles(QStringList);

private slots:
    void openFile( QListWidgetItem * item );

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent *event);
private:
    bool checkFileExtension(const QString&);
};

#endif // MEDIALIST_H

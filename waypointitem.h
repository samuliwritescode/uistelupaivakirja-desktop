#ifndef WAYPOINTITEM_H
#define WAYPOINTITEM_H

#include <QLabel>

class WayPointItem : public QLabel
{
    Q_OBJECT
public:
    explicit WayPointItem(QLabel *parent = 0);
    explicit WayPointItem(const QString& text, QLabel *parent = 0);

    virtual void setText(const QString &);

protected:
    void dragEnterEvent ( QDragEnterEvent * event );
    void dropEvent ( QDropEvent * event );

signals:

public slots:

};

#endif // WAYPOINTITEM_H

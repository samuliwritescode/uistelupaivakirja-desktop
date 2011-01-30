#ifndef LUREITEM_H
#define LUREITEM_H

#include <QLabel>

class LureItem : public QLabel
{
    Q_OBJECT
public:
    explicit LureItem(QLabel *parent = 0);
    explicit LureItem(const QString& text, QLabel *parent = 0);
    virtual void setText(const QString&);

protected:
    void dragEnterEvent ( QDragEnterEvent * event );
    void dropEvent ( QDropEvent * event );

signals:

public slots:

};

#endif // LUREITEM_H

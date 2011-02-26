#ifndef LURECONTROLLER_H
#define LURECONTROLLER_H
#include "controller.h"
#include "lure.h"

class LureController : public Controller
{
public:
    LureController();

    virtual void buttonEvent(EUISource source);
    virtual void booleanEvent(EUISource source, bool value);
    virtual void textEvent(EUISource source, const QString& value);
    virtual void intEvent(EUISource source, int value);

    virtual bool getBooleanValue(EUISource source);
    virtual QString getTextValue(EUISource source);
    virtual int getIntValue(EUISource);

    QList<QPair<QString, int> > getLureList();
    QList<QList<QString> > getLureListLong();
    QStringList getLureColumns();
    QStringList getAlternatives(EUISource source);
    QString lureName(Lure*);

private:
    bool match(Lure*);
    void selectPlace(int value);
    Lure* m_lure;
    bool m_onlyFavorites;
    QString m_searchString;
};

#endif // LURECONTROLLER_H

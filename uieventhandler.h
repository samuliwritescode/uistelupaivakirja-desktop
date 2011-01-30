#ifndef UIEVENTHANDLER_H
#define UIEVENTHANDLER_H
#include <QComboBox>
#include <QObject>
#include "controller.h"
#include "singletons.h"

namespace Ui {
    class MainWindow;
}
class MainWindow;

class UIEventHandler : public QObject
{
    Q_OBJECT
public:
    UIEventHandler(MainWindow*, QObject *parent = 0);
    ~UIEventHandler();

signals:

private slots:
    void observerEvent(int type);
    void observerEventLure(int type);
    void observerEventPlace(int type);

private:
    QString format(const QString& str);
    void setCombo(EUISource source, QComboBox* target);
    MainWindow* mw;
    Ui::MainWindow* ui;
    TripController* m_tripController;
    LureController* m_lureController;
    PlaceController* m_placeController;
};

#endif // UIEVENTHANDLER_H

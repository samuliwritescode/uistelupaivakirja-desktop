#ifndef STATISTICSFORM_H
#define STATISTICSFORM_H

#include <QWidget>
#include "singletons.h"

namespace Ui {
    class StatisticsForm;
}

class StatisticsForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsForm(QWidget *parent = 0);
    ~StatisticsForm();

private:
    StatisticsController* m_statsController;
    Ui::StatisticsForm *ui;

private slots:
    void on_unitCombo_currentIndexChanged(int index);
    void on_columnCombo_currentIndexChanged(QString );
    void on_calculatefromfieldCombo_currentIndexChanged(QString );
    void on_unitCombo_currentIndexChanged(QString );
    void observerEvent(int type);
    void on_columnCombo_textChanged(QString );
    void on_calculatefromfieldCombo_textChanged(QString );
    void on_unitCombo_textChanged(QString );
};

#endif // STATISTICSFORM_H

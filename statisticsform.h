#ifndef STATISTICSFORM_H
#define STATISTICSFORM_H

#include <QWidget>
#include "singletons.h"

namespace Ui {
    class StatisticsForm;
}

class GLStatWidget;

class StatisticsForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsForm(QWidget *parent = 0);
    ~StatisticsForm();

private:
    StatisticsController* m_statsController;
    Ui::StatisticsForm *ui;
    GLStatWidget* m_statWidget;

private slots:
    void on_byColumnCombo_currentIndexChanged(QString );
    void on_statisticsCombo_currentIndexChanged(QString );
    void on_unitCombo_currentIndexChanged(int index);
    void on_columnCombo_currentIndexChanged(QString );
    void on_calculatefromfieldCombo_currentIndexChanged(QString );
    void on_unitCombo_currentIndexChanged(QString );
    void observerEvent(int type);
    void progressBarChanged(int);
    void on_columnCombo_textChanged(QString );
    void on_calculatefromfieldCombo_textChanged(QString );
    void on_unitCombo_textChanged(QString );
};

#endif // STATISTICSFORM_H

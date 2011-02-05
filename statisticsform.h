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
};

#endif // STATISTICSFORM_H

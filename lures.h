#ifndef LURES_H
#define LURES_H

#include <QDialog>

namespace Ui {
    class Lures;
}

class Lures : public QDialog
{
    Q_OBJECT

public:
    explicit Lures(QWidget *parent = 0);
    ~Lures();

private:
    Ui::Lures *ui;
};

#endif // LURES_H

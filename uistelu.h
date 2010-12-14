#ifndef UISTELU_H
#define UISTELU_H

#include <QDialog>

namespace Ui {
    class Uistelu;
}

class Uistelu : public QDialog
{
    Q_OBJECT

public:
    explicit Uistelu(QWidget *parent = 0);
    ~Uistelu();

private:
    Ui::Uistelu *ui;
};

#endif // UISTELU_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tripcontroller.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TripController* m_controller;

private slots:

private slots:
    void on_place_currentIndexChanged(QString );
    void on_time23_4_clicked(bool checked);
    void on_time18_23_clicked(bool checked);
    void on_time14_18_clicked(bool checked);
    void on_time11_14_clicked(bool checked);
    void on_time9_11_clicked(bool checked);
    void on_time4_9_clicked(bool checked);
    void on_dateEdit_dateChanged(QDate date);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "tripform.h"
#include "lureform.h"
#include "placeform.h"
#include "statisticsform.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static void showErrorNotification(const QString&);

private:    
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef LUREFORM_H
#define LUREFORM_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include "singletons.h"

namespace Ui {
    class LureForm;
}

class LureForm : public QWidget
{
    Q_OBJECT

public:
    explicit LureForm(QWidget *parent = 0);
    ~LureForm();

private slots:
    void on_lure_save_clicked();
    void on_lure_undo_clicked();
    void on_lure_type_textChanged(QString );
    void on_lure_color_textChanged(QString );
    void on_lure_size_textChanged(QString );
    void on_lure_model_textChanged(QString );
    void on_lure_manufacturer_textChanged(QString );
    void observerEvent(int type);

    void on_lure_list_itemSelectionChanged();
    void on_lure_favorite_clicked(bool checked);
    void on_lure_delete_clicked();
    void on_lure_new_clicked();

private:
    void setCombo(EUISource source, QComboBox* target);
    LureController* m_lureController;
    Ui::LureForm *ui;
};

#endif // LUREFORM_H

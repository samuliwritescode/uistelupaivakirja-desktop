#ifndef LUREFORM_H
#define LUREFORM_H

#include <QWidget>
#include <QListWidgetItem>
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
    void observerEvent(int type);

    void on_lure_list_itemClicked(QListWidgetItem* item);
    void on_lure_list_itemSelectionChanged();
    void on_lure_list_itemActivated(QListWidgetItem* item);
    void on_lure_favorite_clicked(bool checked);
    void on_lure_color_textEdited(QString );
    void on_lure_size_textEdited(QString );
    void on_lure_model_textEdited(QString );
    void on_lure_manufacturer_textEdited(QString );
    void on_lure_delete_clicked();
    void on_lure_new_clicked();

private:
    LureController* m_lureController;
    Ui::LureForm *ui;
};

#endif // LUREFORM_H

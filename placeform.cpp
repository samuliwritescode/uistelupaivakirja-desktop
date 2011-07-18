#include "placeform.h"
#include "ui_placeform.h"

PlaceForm::PlaceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaceForm)
{
    ui->setupUi(this);
    m_placeController = Singletons::placeController();
    ui->place_list->sortByColumn(0, Qt::AscendingOrder);

    observerEvent(Controller::ePlaceListUpdated);

    connect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

PlaceForm::~PlaceForm()
{
    disconnect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
}

void PlaceForm::observerEvent(int type)
{
    ui->place_save->setDisabled(!m_placeController->getBooleanValue(eUnsavedChanges));
    ui->place_undo->setDisabled(!m_placeController->getBooleanValue(eUnsavedChanges));
    ui->place_delete->setDisabled(!m_placeController->getBooleanValue(ePlaceList));

    ui->place_name->setDisabled(!m_placeController->getBooleanValue(ePlaceList));
    ui->place_city->setDisabled(!m_placeController->getBooleanValue(ePlaceList));
    ui->place_invisible->setDisabled(!m_placeController->getBooleanValue(ePlaceList));
    ui->place_misc->setDisabled(!m_placeController->getBooleanValue(ePlaceList));

    if(type == Controller::ePlaceListUpdated)
    {
        ui->place_list->blockSignals(true);
        ui->place_list->setSortingEnabled(false);
        ui->place_list->clear();
        ui->place_list->clearContents();
        ui->place_list->setRowCount(0);
        ui->place_list->setColumnCount(m_placeController->getPlaceColumns().count());
        ui->place_list->setHorizontalHeaderLabels(m_placeController->getPlaceColumns());

        int selectedPlace = m_placeController->getIntValue(ePlaceList);
        QList<QList<QString> > places = m_placeController->getPlaceListFull();
        for(int loop=0; loop < places.size(); loop++)
        {
            QList<QString> place = places.at(loop);
            ui->place_list->insertRow(loop);
            QString id = place.at(place.count()-1);

            for(int col = 0; col < place.count()-1; col++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(place.at(col));
                item->setData(Qt::UserRole+1, id);
                ui->place_list->setItem(loop, col, item);
            }
            if(id.toInt() == selectedPlace)
            {
                ui->place_list->selectRow(loop);
            }
        }
        ui->place_list->setSortingEnabled(true);
        ui->place_list->blockSignals(false);
    } else if(type == Controller::ePlaceUpdated)
    {
        ui->place_name->blockSignals(true);
        ui->place_city->blockSignals(true);
        ui->place_misc->blockSignals(true);
        ui->place_invisible->blockSignals(true);

        ui->place_name->setText( m_placeController->getTextValue(ePlaceName) );
        ui->place_city->setText( m_placeController->getTextValue(ePlaceCity) );
        ui->place_misc->setPlainText( m_placeController->getTextValue(ePlaceMiscText) );
        ui->place_invisible->setChecked(m_placeController->getBooleanValue(ePlaceInvisible));

        ui->place_name->blockSignals(false);
        ui->place_city->blockSignals(false);
        ui->place_misc->blockSignals(false);
        ui->place_invisible->blockSignals(false);
    }
}

void PlaceForm::on_place_name_textEdited(QString value)
{
    m_placeController->textEvent(ePlaceName, value);
}

void PlaceForm::on_place_city_textEdited(QString value)
{
    m_placeController->textEvent(ePlaceCity, value);
}

void PlaceForm::on_place_invisible_clicked(bool checked)
{
    m_placeController->booleanEvent(ePlaceInvisible, checked);
}

void PlaceForm::on_place_misc_textChanged()
{
    m_placeController->textEvent(ePlaceMiscText, ui->place_misc->toPlainText());
}

void PlaceForm::on_place_new_clicked()
{
    m_placeController->buttonEvent(ePlaceNew);
    ui->place_name->setFocus();
}

void PlaceForm::on_place_delete_clicked()
{
    m_placeController->buttonEvent(ePlaceDelete);
}

void PlaceForm::on_place_list_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selected = ui->place_list->selectedItems();
    if(selected.size() >= 1)
        m_placeController->intEvent(ePlaceList, selected.at(0)->data(Qt::UserRole+1).toInt());
}

void PlaceForm::on_place_undo_clicked()
{
    m_placeController->buttonEvent(ePlaceUndo);
}

void PlaceForm::on_place_save_clicked()
{
    m_placeController->buttonEvent(ePlaceSave);
}

#include "placeform.h"
#include "ui_placeform.h"

PlaceForm::PlaceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaceForm)
{
    ui->setupUi(this);
    m_placeController = Singletons::placeController();

    observerEvent(Controller::ePlaceListUpdated);

    connect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

PlaceForm::~PlaceForm()
{
    disconnect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
}

void PlaceForm::observerEvent(int type)
{
    if(type == Controller::ePlaceListUpdated)
    {
         ui->place_list->clear();

        QList<QPair<QString, int> > places = m_placeController->getPlaceList();
        for(int loop=0; loop < places.size(); loop++)
        {
            QPair<QString, int> pair = places.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first,  ui->place_list, pair.second);
             ui->place_list->insertItem(0, item);

        }
    } else if(type == Controller::ePlaceUpdated)
    {
         ui->place_name->setText( m_placeController->getTextValue(ePlaceName) );
         ui->place_city->setText( m_placeController->getTextValue(ePlaceCity) );
         ui->place_misc->setText( m_placeController->getTextValue(ePlaceMiscText) );
         ui->place_invisible->setChecked(m_placeController->getBooleanValue(ePlaceInvisible));
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
}

void PlaceForm::on_place_delete_clicked()
{
    m_placeController->buttonEvent(ePlaceDelete);
}

void PlaceForm::on_place_list_itemActivated(QListWidgetItem* item)
{
    m_placeController->intEvent(ePlaceList, item->type());
}

void PlaceForm::on_place_list_itemSelectionChanged()
{
    QList<QListWidgetItem*> selected = ui->place_list->selectedItems();
    if(selected.size() > 1)
        m_placeController->intEvent(ePlaceList, selected.at(0)->type());
}

void PlaceForm::on_place_list_itemClicked(QListWidgetItem* item)
{
    m_placeController->intEvent(ePlaceList, item->type());
}



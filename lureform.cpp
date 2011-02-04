#include "lureform.h"
#include "ui_lureform.h"

LureForm::LureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LureForm)
{
    ui->setupUi(this);
    m_lureController = Singletons::lureController();

    observerEvent(Controller::eLureListUpdated);

    connect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));

}

LureForm::~LureForm()
{
    disconnect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
}



void LureForm::observerEvent(int type)
{
    if(type == Controller::eLureListUpdated)
    {
         ui->lure_list->clear();

        QList<QPair<QString, int> > lures = m_lureController->getLureList();
        for(int loop=0; loop < lures.size(); loop++)
        {
            QPair<QString, int> pair = lures.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first,  ui->lure_list, pair.second);
             ui->lure_list->insertItem(0, item);
        }
    } else if(type == Controller::eLureUpdated)
    {
         ui->lure_manufacturer->setText(m_lureController->getTextValue(eLureMaker));
         ui->lure_model->setText(m_lureController->getTextValue(eLureModel));
         ui->lure_size->setText(m_lureController->getTextValue(eLureSize));
         ui->lure_color->setText(m_lureController->getTextValue(eLureColor));
         ui->lure_favorite->setChecked(m_lureController->getBooleanValue(eLureFavorite));
    }
}


void LureForm::on_lure_new_clicked()
{
    m_lureController->buttonEvent(eLureNew);
}

void LureForm::on_lure_delete_clicked()
{
    m_lureController->buttonEvent(eLureDelete);
}

void LureForm::on_lure_manufacturer_textEdited(QString text)
{
    m_lureController->textEvent(eLureMaker, text);
}

void LureForm::on_lure_model_textEdited(QString text)
{
    m_lureController->textEvent(eLureModel, text);
}

void LureForm::on_lure_size_textEdited(QString text)
{
    m_lureController->textEvent(eLureSize, text);
}

void LureForm::on_lure_color_textEdited(QString text)
{
    m_lureController->textEvent(eLureColor, text);
}

void LureForm::on_lure_favorite_clicked(bool checked)
{
    m_lureController->booleanEvent(eLureFavorite, checked);
}

void LureForm::on_lure_list_itemActivated(QListWidgetItem* item)
{
    m_lureController->intEvent(eLureList, item->type());
}

void LureForm::on_lure_list_itemSelectionChanged()
{
    QList<QListWidgetItem*> selected = ui->lure_list->selectedItems();
    if(selected.size() > 1)
        m_lureController->intEvent(eLureList, selected.at(0)->type());
}

void LureForm::on_lure_list_itemClicked(QListWidgetItem* item)
{
    m_lureController->intEvent(eLureList, item->type());
}

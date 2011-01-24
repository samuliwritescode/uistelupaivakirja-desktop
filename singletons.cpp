#include "singletons.h"

TrollingModel* Singletons::s_model = NULL;
TripController* Singletons::s_tripController = NULL;
PlaceController* Singletons::s_placeController = NULL;

TrollingModel* Singletons::model()
{
    if(!s_model)
    {
        s_model = new TrollingModel();
        s_model->initialize();
    }

    return s_model;
}

TripController* Singletons::tripController()
{
    if(!s_tripController)
        s_tripController = new TripController();

    return s_tripController;
}

PlaceController* Singletons::placeController()
{
    if(!s_placeController)
        s_placeController = new PlaceController();

    return s_placeController;
}

Singletons::Singletons()
{

}

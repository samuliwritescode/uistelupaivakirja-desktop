#include "singletons.h"

TrollingModel* Singletons::s_model = NULL;
TripController* Singletons::s_tripController = NULL;
PlaceController* Singletons::s_placeController = NULL;
LureController* Singletons::s_lureController = NULL;
StatisticsController* Singletons::s_statsController = NULL;
SynchronizerController* Singletons::s_syncController = NULL;

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

LureController* Singletons::lureController()
{
    if(!s_lureController)
        s_lureController = new LureController();

    return s_lureController;
}

StatisticsController* Singletons::statsController()
{
    if(!s_statsController)
        s_statsController = new StatisticsController();

    return s_statsController;
}

SynchronizerController* Singletons::syncController()
{
    if(!s_syncController)
        s_syncController = new SynchronizerController();

    return s_syncController;
}

Singletons::Singletons()
{

}

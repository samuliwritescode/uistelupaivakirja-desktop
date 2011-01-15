#ifndef SINGLETONS_H
#define SINGLETONS_H

#include "trollingmodel.h"
#include "tripcontroller.h"
#include "placecontroller.h"

class Singletons
{
public:
    static TrollingModel* model();
    static TripController* tripController();
    static PlaceController* placeController();
private:
    Singletons();
    static TrollingModel* s_model;
    static TripController* s_tripController;
    static PlaceController* s_placeController;
};

#endif // SINGLETONS_H

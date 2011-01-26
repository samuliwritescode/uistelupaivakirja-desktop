#ifndef SINGLETONS_H
#define SINGLETONS_H

#include "trollingmodel.h"
#include "tripcontroller.h"
#include "placecontroller.h"
#include "lurecontroller.h"

class Singletons
{
public:
    static TrollingModel* model();
    static TripController* tripController();
    static PlaceController* placeController();
    static LureController* lureController();
private:
    Singletons();
    static TrollingModel* s_model;
    static TripController* s_tripController;
    static PlaceController* s_placeController;
    static LureController* s_lureController;
};

#endif // SINGLETONS_H

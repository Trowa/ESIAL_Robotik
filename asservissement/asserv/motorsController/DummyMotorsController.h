#ifndef DUMMYMOTORSCONTROLLER
#define DUMMYMOTORSCONTROLLER

#include "MotorsController.h"

class DummyMotorsController : public MotorsController
{
public :
    void vitesseG(int vitMoteurG) { }
    void vitesseD(int vitMoteurD) { }
    ~DummyMotorsController() { }
};

#endif

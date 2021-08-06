#ifndef GAMEINTRO_H_INCLUDED
#define GAMEINTRO_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../objects/gameintro/GameIntroController.h"

class GameIntro : public is::GameDisplay
{
public:
    GameIntro(is::GameSystemExtended &gameSysExt);

    void loadResources();

    //////////////////////////////////////////////////////
    // void step();
    //
    // This method is no longer used because the SDM is
    // responsible for update the events and objects of the scene
    // for us.
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    // void draw();
    //
    // This method is no longer used because the SDM is
    // responsible for displaying the objects of the scene
    // for us.
    //////////////////////////////////////////////////////
};

#endif // GAMEINTRO_H_INCLUDED

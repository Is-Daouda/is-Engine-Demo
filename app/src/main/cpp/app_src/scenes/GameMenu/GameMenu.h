#ifndef GAMEMENU_H_INCLUDED
#define GAMEMENU_H_INCLUDED

#include "../../../isEngine/system/function/GameFunction.h"
#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/Background.h"
#include "../../objects/gamemenu/MenuController.h"
#include "../../objects/widgets/CancelButton.h"

class GameMenu : public is::GameDisplay
{
public:
    GameMenu(is::GameSystemExtended &gameSysExt);

    void loadResources();
    void SDMmanageSceneEvents();
    void SDMmanageSceneMsgAnswers();

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

#endif // GAMEMENU_H_INCLUDED

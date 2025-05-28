#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/function/GameKeyData.h"
#include "../../../isEngine/system/entity/Background.h"

#include "../../objects/gamelevel/Player.h"
#include "../../objects/gamelevel/Block.h"
#include "../../objects/gamelevel/GameController.h"
#include "../../objects/gamelevel/HUD.h"
#include "../../objects/gamelevel/Bonus.h"
#include "../../objects/gamelevel/Enemy.h"
#include "../../objects/gamelevel/FireBall.h"
#include "../../objects/gamelevel/FinishObject.h"
#include "../../objects/gamelevel/Tiles.h"
#include "../../objects/gamelevel/SpecialObject.h"
#include "../../objects/gamelevel/StartTransition.h"
#include "../../objects/widgets/CancelButton.h"
#include "../../objects/widgets/PauseOption.h"
#include "../../objects/widgets/GameDialog.h"

#include "../../levels/Level.h"
#include "../../language/GameLanguage.h"

/// uncomment to use Box 2D
//#include "../../../isEngine/ext_lib/Box2D/Box2D.h"

class GameLevel : public is::GameDisplay
{
public:
    GameLevel(is::GameSystemExtended &gameSysExt);

    void SDMmanageSceneEvents();
    void SDMmanageSceneMsgAnswers();
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

private:
    int const CURRENT_LEVEL;
};

#endif // GAMELEVEL_H

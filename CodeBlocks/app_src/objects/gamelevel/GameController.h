#ifndef GAMECONTROLLER_H_INCLUDED
#define GAMECONTROLLER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/function/GameTime.h"
#include "Player.h"
#include "Limiter.h"
#include "FinishObject.h"
#include "../widgets/CancelButton.h"
#include "../widgets/GameDialog.h"
#include "../widgets/PauseOption.h"

////////////////////////////////////////////////////////////
/// This class is used to manage game level objects :
/// - view
/// - collision of blocks with the other objects of the level
/// - level chronometer
/// - background 3D animation
/// - show RPG dialog box
/// - pause game and manage pause menu option
/// - manage finish object when the player win the level
////////////////////////////////////////////////////////////
class GameController : public is::MainObject
{
public:
    bool m_timeUp;
    int m_restartTime;
    is::GameTime m_gameTime;

    GameController(int const &CURRENT_LEVEL, is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void blocCollision(float const &DELTA_TIME);
    void levelSpecialAction();
    void stopSounds(bool val);
    void pauseGame();

    /// Allows to know if the game play objects (player, bonus, enemies, ...) can be activated
    bool canActivate()
    {
        return (m_scene->getIsPlaying() && !static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"))->showDialog());
    }

private:
    is::GameDisplay *m_scene;
    int const &CURRENT_LEVEL;
    bool m_countLevelTime;
    float m_timeCancelCollision;
};

#endif // GAMECONTROLLER_H_INCLUDED

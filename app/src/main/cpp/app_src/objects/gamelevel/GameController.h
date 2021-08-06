#ifndef GAMECONTROLLER_H_INCLUDED
#define GAMECONTROLLER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/function/GameTime.h"
#include "Player.h"
#include "FireBall.h"
#include "Enemy.h"
#include "FinishObject.h"
#include "Bonus.h"
#include "Effect.h"
#include "../widgets/CancelButton.h"
#include "../widgets/GameDialog.h"
#include "../widgets/PauseOption.h"

////////////////////////////////////////////////////////////
/// This class is used to manage game level objects :
/// - view
/// - collision of blocks with the other objects of the level
/// - level chronometer
/// - show RPG dialog box
/// - pause game and manage pause menu option
/// - manage finish object when the player win the level
////////////////////////////////////////////////////////////
class GameController : public is::MainObject
{
public:
    bool m_timeUp = false;
    bool m_moveView = true;
    is::GameTime m_gameTime;

    GameController(int const &CURRENT_LEVEL, is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void blockCollision(float const &DELTA_TIME);
    void levelSpecialAction();
    void stopSounds(bool val);
    void setCountLevelTime(bool val) {m_countLevelTime = val;}
    void setLaunchTransition(bool val) {m_launchTransition = val;}
    void addBonus();
    void pauseGame();

    /// Allows to know if the game play objects (player, bonus, enemies, ...) can be activated
    bool canActivate()
    {
        return (m_scene->getIsPlaying() && !static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"))->showDialog() &&
                !m_launchTransition);
    }
    bool getCountLevelTime() {return m_countLevelTime;}
    bool getPlayWarningSnd() {return m_playWarningSnd;}

private:
    is::GameDisplay *m_scene;

    int const &CURRENT_LEVEL;
    bool m_countLevelTime, m_playWarningSnd, m_launchTransition;
    float m_timeCancelCollision;
};

#endif // GAMECONTROLLER_H_INCLUDED

#ifndef GAMEACTIVITY_H_INCLUDED
#define GAMEACTIVITY_H_INCLUDED

#include "SwooshFiles.h"
#include "../scenes/GameIntro/GameIntro.h"
#include "../scenes/GameMenu/GameMenu.h"
#include "../scenes/GameLevel/GameLevel.h"
#include "../scenes/GameOver/GameOver.h"

using namespace swoosh::intent;

////////////////////////////////////////////////////////////
/// Allows to manage the different scenes of the game and
/// to interconnect the engine and the Swoosh library to make
/// transition effects
////////////////////////////////////////////////////////////
class GameActivity : public Activity
{
private:
    std::shared_ptr<is::GameDisplay> m_gameScene;
    bool m_changeActivity;

public:
    GameActivity(ActivityController& controller, is::GameSystemExtended &gameSysExt) :
        Activity(&controller), m_changeActivity(false)
    {
        m_gameScene = nullptr;
        switch (gameSysExt.m_launchOption)
        {
        case is::DisplayOption::INTRO:
            m_gameScene = std::make_shared<GameIntro>(controller.getWindow(),
                                                         getView(),
                                                         *(this->controller->getSurface()),
                                                         gameSysExt);
        break;
        case is::DisplayOption::MAIN_MENU:
            m_gameScene = std::make_shared<GameMenu>(controller.getWindow(),
                                                        getView(),
                                                        *(this->controller->getSurface()),
                                                        gameSysExt);
        break;
        case is::DisplayOption::GAME_LEVEL:
			m_gameScene = std::make_shared<GameLevel>(controller.getWindow(),
                                                         getView(),
                                                         *(this->controller->getSurface()),
                                                         gameSysExt);
        break;
        case is::DisplayOption::GAME_OVER:
        case is::DisplayOption::GAME_END_SCREEN:
            m_gameScene = std::make_shared<GameOver>(controller.getWindow(),
                                                        getView(),
                                                        *(this->controller->getSurface()),
                                                        gameSysExt);
        break;

        default:
			is::showLog("Error : Scene not found !", true);
		break;
        }
        m_gameScene->loadResources();
        this->setBGColor(m_gameScene->getBgColor());
    }

    virtual void onStart() {;}

    virtual void onUpdate(double elapsed)
    {
        if (m_gameScene->getIsRunning()) m_gameScene->step();
        else
        {
            if (!m_changeActivity)
            {
                switch (m_gameScene->getGameSystem().m_launchOption)
                {
                    case is::DisplayOption::MAIN_MENU:
                        {
                            m_gameScene->getGameSystem().m_checkPoint = false;
                            using transition = segue<BlackWashFade>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                    break;

                    case is::DisplayOption::GAME_LEVEL:
                        {
                            using transition = segue<VerticalSlice, sec<2>>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                    break;

                    case is::DisplayOption::RESTART_LEVEL : // restart level (when player loses)
                        // restart level if life > 1
                        if (m_gameScene->getGameSystem().m_currentLives > 1)
                        {
                            m_gameScene->getGameSystem().m_currentLives--;
                            m_gameScene->getGameSystem().m_currentScore = 0;
                            m_gameScene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;
                            m_gameScene->getGameSystem().saveData(is::GameConfig::GAME_DATA_FILE);

                            using transition = segue<BlackWashFade>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                        else // else GAME OVER
                        {
                            // reinitialize all variables excepting the current level and the game progress
                            m_gameScene->getGameSystem().initData(false);
                            m_gameScene->getGameSystem().m_currentBonus = 0;
                            m_gameScene->getGameSystem().m_currentLives = 3;
                            m_gameScene->getGameSystem().m_launchOption = is::DisplayOption::GAME_OVER;

                            using transition = segue<VerticalSlice, sec<2>>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                    break;

                    case is::DisplayOption::NEXT_LEVEL : // go to the next level
                       // reinitialize level global variable
                        m_gameScene->getGameSystem().m_levelTime = 0;
                        m_gameScene->getGameSystem().m_currentScore = 0;
                        m_gameScene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;

                        // go to game end screen when player finish all levels
                        if (m_gameScene->getGameSystem().m_currentLevel > m_gameScene->getGameSystem().m_levelNumber)
                        {
                            m_gameScene->getGameSystem().m_launchOption = is::DisplayOption::GAME_END_SCREEN;

                            using transition = segue<VerticalSlice, sec<2>>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                        else {
                            using transition = segue<RadialCCW, sec<1>>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                    break;

                    case is::DisplayOption::GAME_OPTION_RESTART: // restart level (when player uses restart option)
                        m_gameScene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;

                        // reinitialize all variables excepting the current level and the game progress
                        m_gameScene->getGameSystem().initData(false);
                        {
                            using transition = segue<RadialCCW, sec<1>>;
                            using action = transition::to<GameActivity>;
                            getController().replace<action>(m_gameScene->getGameSystem());
                        }
                    break;

                    default:
                        is::showLog("Error : Scene not found !");
                        std::terminate();
                    break;
                }
                m_changeActivity = true;
            }
        }
    }

    virtual void onLeave()  {;}

    virtual void onExit()   {;}

    virtual void onEnter()  {;}

    virtual void onResume() {;}

    virtual void onDraw(sf::RenderTexture& surface)
    {
        m_gameScene->drawScreen();
    }

    virtual void onEnd() {;}

    ~GameActivity() {;}
};

#endif // GAMEACTIVITY_H_INCLUDED

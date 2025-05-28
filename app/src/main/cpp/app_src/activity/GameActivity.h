#ifndef GAMEACTIVITY_H_INCLUDED
#define GAMEACTIVITY_H_INCLUDED

#include "../scenes/GameIntro/GameIntro.h"
#include "../scenes/GameMenu/GameMenu.h"
#include "../scenes/GameLevel/GameLevel.h"
#include "../scenes/GameOver/GameOver.h"

// example
// #include "../scenes/YourScene/YourScene.h"

////////////////////////////////////////////////////////////
/// Allows to manage the different scenes of the game
////////////////////////////////////////////////////////////
class GameActivity
{
private:
    std::shared_ptr<is::GameDisplay> m_gameScene;

public:
    bool m_changeActivity = false;
    GameActivity(is::GameSystemExtended &gameSysExt)
    {
        m_gameScene = nullptr;
////////////////////////////////////////////////////////////

        // Allows to choose the scene that will be launched
        switch (gameSysExt.m_launchOption)
        {
        case is::DisplayOption::INTRO:
            m_gameScene = std::make_shared<GameIntro>(gameSysExt);
        break;
        case is::DisplayOption::MAIN_MENU:
            m_gameScene = std::make_shared<GameMenu>(gameSysExt);
        break;
        case is::DisplayOption::GAME_LEVEL:
        case is::DisplayOption::RESTART_LEVEL:
        case is::DisplayOption::GAME_OPTION_RESTART:
			m_gameScene = std::make_shared<GameLevel>(gameSysExt);
        break;
        case is::DisplayOption::GAME_OVER:
        case is::DisplayOption::GAME_END_SCREEN:
            m_gameScene = std::make_shared<GameOver>(gameSysExt);
        break;

        // example
        // case is::DisplayOption::YOUR_SCENE:
        // m_gameScene = std::make_shared<YourScene>(gameSysExt);
        // break;

        default:
			is::showLog("ERROR: Scene not found !", true);
		break;
        }
        m_gameScene->loadResources();
    }

    virtual void onUpdate()
    {
        if (m_gameScene->getIsRunning()) m_gameScene->step();
        else
        {
            if (!m_changeActivity) m_changeActivity = true;
        }
    }

    virtual void onDraw() {m_gameScene->drawScreen();}

    virtual ~GameActivity() {}
};

#endif // GAMEACTIVITY_H_INCLUDED

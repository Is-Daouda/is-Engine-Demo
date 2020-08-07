#include "GameLevel.h"

GameLevel::GameLevel(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, is::GameSystemExtended &gameSysExt):
    GameDisplay(window, view, surface, gameSysExt, sf::Color(36, 146, 255)),
    CURRENT_LEVEL(gameSysExt.m_currentLevel)
{
    // redefine view parameter
    m_viewX = (m_viewW / 2.f);
    m_viewY = (m_viewH / 2.f);
    m_sceneWidth = 0;
    m_sceneHeight = 0;
    srand(time(NULL));
}

void GameLevel::SDMmanageSceneEvents()
{
    // Manage game controls (keyboard & Virtual Game Pad)
    auto gameKey = static_cast<is::GameKeyData*>(SDMgetObject("GameKeyData"));
    gameKey->step(DELTA_TIME);

    // allows activated use of buttons
    if (!m_keyBackPressed) gameKey->m_keyPausePressed = false;

    // allows activated use of buttons
    if (!m_gameSysExt.keyIsPressed(is::GameConfig::KEY_LEFT) &&
        !m_gameSysExt.keyIsPressed(is::GameConfig::KEY_RIGHT) &&
        !m_gameSysExt.keyIsPressed(is::GameConfig::KEY_A)
        && !m_gameSysExt.isPressed())
        m_gameSysExt.m_keyIsPressed = false;

    // even loop
    while (m_window.pollEvent(m_event))
    {
        controlEventFocusClosing();
        if (m_event.type == sf::Event::KeyReleased)
        {
            if (m_event.key.code == is::GameConfig::KEY_CANCEL)
            {
                m_keyBackPressed = true;
            }
        }
    }

    // if the window is not activated suspend the game
    if (!m_windowIsActive && m_isPlaying)
    {
        static_cast<GameController*>(SDMgetObject("GameController"))->stopSounds(true);
        m_isPlaying = false;
    }

    if (m_sceneEnd) m_isRunning = false;
}

void GameLevel::SDMmanageSceneMsgAnswers()
{
    // when user closes message box execute this instruction
    // "m_waitTime" allow to disable clicks on objects during a moment when user closes message box
    auto gameKey = static_cast<is::GameKeyData*>(SDMgetObject("GameKeyData"));
    switch (m_optionIndex)
    {
    case is::DisplayOption::GAME_OPTION_RESTART:
        if (m_msgAnswer == MsgAnswer::YES)
        {
            m_gameSysExt.m_launchOption = is::DisplayOption::GAME_OPTION_RESTART;
            m_waitTime = 10;
            m_sceneEnd = true;
        }
        else m_waitTime = 10;
        gameKey->m_keyPausePressed = true;
    break;

    case is::DisplayOption::QUIT_GAME:
        if (m_msgAnswer == MsgAnswer::YES)
        {
            m_gameSysExt.m_launchOption = is::DisplayOption::MAIN_MENU;
            m_sceneEnd = true;
        }
        else m_waitTime = 10;
        gameKey->m_keyPausePressed = true;
    break;
    }
}

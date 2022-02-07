#include "GameOverController.h"
#include "../../language/GameLanguage.h"

GameOverController::GameOverController(is::GameDisplay *scene) :
    m_scene(scene),
    m_sceneIndex(m_scene->getGameSystem().m_launchOption)
{
    // Here the text is displayed depending on the screen
    float txtX(320.f), txtY((m_sceneIndex == is::DisplayOption::GAME_OVER) ? 110.f : 32.f);
    is::createText(scene->GRMgetFont("font_msg"), m_txtTitle,
                    ((m_sceneIndex == is::DisplayOption::GAME_OVER) ?
                         is::lang::game_over[m_scene->getGameSystem().m_gameLanguage] :
                         is::lang::end_msg_congrat[m_scene->getGameSystem().m_gameLanguage]),
                    txtX, txtY, sf::Color::White, 56);
    is::centerSFMLObj(m_txtTitle);

    // message (only for Game End Screen)
    is::createText(m_scene->getFontMsg(), m_txtEndMsg, is::lang::end_msg_sentences[m_scene->getGameSystem().m_gameLanguage],
                    32.f, txtY + 70.f, sf::Color::White, 22);
    is::createText(m_scene->getFontMsg(), m_txtGetEditor, is::lang::end_msg_lvl_editor[m_scene->getGameSystem().m_gameLanguage],
                    320.f, txtY + 326.f, sf::Color::Yellow, true, 22);

    // create sprites
    float xPos(320.f), yPos((m_sceneIndex == is::DisplayOption::GAME_OVER) ? 250.f : 360.f);
    is::createSprite(m_scene->GRMgetTexture("option_pad"), m_sprPad1, sf::IntRect(0, 0, 160, 32), sf::Vector2f(xPos, yPos), sf::Vector2f(80.f, 16.f));
    is::createSprite(m_scene->GRMgetTexture("option_pad"), m_sprPad2, sf::IntRect(0, 0, 160, 32), sf::Vector2f(xPos, yPos + 50.f), sf::Vector2f(80.f, 16.f));

    float btSelectX(yPos + ((m_sceneIndex == is::DisplayOption::GAME_OVER) ? 0.f : 50.f));
    is::createSprite(m_scene->GRMgetTexture("option_pad"), m_scene->getSprButtonSelect(), sf::IntRect(160, 0, 160, 32),sf::Vector2f(xPos, btSelectX), sf::Vector2f(80.f, 16.f));

    // create texts
    is::createText(m_scene->getFontSystem(), m_txtRestartGame, is::lang::pad_restart_game[m_scene->getGameSystem().m_gameLanguage], 0.f, 0.f);
    is::createText(m_scene->getFontSystem(), m_txtQuitGame, is::lang::pad_quit_game[m_scene->getGameSystem().m_gameLanguage], 0.f, 0.f);
    is::centerSFMLObj(m_txtRestartGame);
    is::centerSFMLObj(m_txtQuitGame);
    is::setSFMLObjX_Y(m_txtRestartGame, is::getSFMLObjX(m_sprPad1), is::getSFMLObjY(m_sprPad1));
    is::setSFMLObjX_Y(m_txtQuitGame, is::getSFMLObjX(m_sprPad2), is::getSFMLObjY(m_sprPad2));

    // Choose the option quit by default for the Game End Screen
    if (m_sceneIndex == is::DisplayOption::GAME_END_SCREEN) m_scene->setOptionIndex(QUIT);
    else m_scene->GSMplayMusic("game_over"); // play music
}

void GameOverController::step(float const &DELTA_TIME)
{
    bool mouseInCollison(false);

    // check collision with sprite
    if ((m_scene->mouseCollision(m_sprPad1) && m_sceneIndex == is::DisplayOption::GAME_OVER) ||
        m_scene->mouseCollision(m_sprPad2)) mouseInCollison = true;

    if (!m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_UP) &&
        !m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_DOWN) &&
        !m_scene->getGameSystem().isPressed())
            m_scene->getGameSystem().m_keyIsPressed = false;

    if (!m_scene->m_isClosed)
    {
        // change option with mouse (touch on Android)
        if (m_scene->mouseCollision(m_sprPad1) && m_scene->getOptionIndex() != CONTINUE &&
            m_sceneIndex == is::DisplayOption::GAME_OVER) m_scene->setOptionIndex(CONTINUE, true);
        else if (m_scene->mouseCollision(m_sprPad2) && m_scene->getOptionIndex() != QUIT) m_scene->setOptionIndex(QUIT, true);

        // avoid the long pressing button effect
        if (!mouseInCollison && m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE)) m_scene->getGameSystem().m_keyIsPressed = true;

        // change option with keyboard (only for PC)
        // only for Game Over Screen
        if (!m_scene->getGameSystem().m_keyIsPressed && !mouseInCollison && m_sceneIndex == is::DisplayOption::GAME_OVER)
        {
            if (m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_UP)) m_scene->setOptionIndex(-1, false);
            else if (m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_DOWN)) m_scene->setOptionIndex(1, false);
            if (m_scene->getOptionIndex() < CONTINUE) m_scene->setOptionIndex(QUIT);
            if (m_scene->getOptionIndex() > QUIT) m_scene->setOptionIndex(CONTINUE);
        }

        // open editor link
        if (m_scene->mouseCollision(m_txtGetEditor) && m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE) &&
            m_sceneIndex == is::DisplayOption::GAME_END_SCREEN && m_imageScale < 1.05f)
        {
            m_imageScale = 1.4f;
            is::openURL("www.github.com/Is-Daouda/is-Engine-Level-Editor", is::OpenURLAction::Http);
        }

        // validate option
        if ((m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::KEYBOARD) ||
             (m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE) && mouseInCollison)) &&
            (m_scene->getWaitTime() == 0 && !m_scene->getGameSystem().m_keyIsPressed))
        {
            m_scene->GSMplaySound("select_option"); // We play this sound
            m_scene->setSprButtonSelectScale(1.6f);

            switch (m_scene->getOptionIndex())
            {
            case CONTINUE:
                m_scene->getGameSystem().initData(false); // clear game play date (life, checkpont, score, ...)
                m_scene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;
                m_scene->getGameSystem().saveData(is::GameConfig::GAME_DATA_FILE); // save data
            break;

            case QUIT:
                m_scene->getGameSystem().m_launchOption = is::DisplayOption::MAIN_MENU;
            break;
            }
            m_scene->m_isClosed = true;
        }
    }

    // change text color
    m_scene->setTextAnimation(m_txtRestartGame, m_sprPad1, CONTINUE);
    m_scene->setTextAnimation(m_txtQuitGame, m_sprPad2, QUIT);

    // pad animation
    is::scaleAnimation(DELTA_TIME, m_scene->getSprButtonSelectScale(), m_scene->getSprButtonSelect());

    // text animation
    is::scaleAnimation(DELTA_TIME, m_imageScale, m_txtGetEditor);

    if (m_scene->m_isClosed) m_scene->setIsRunning(false);
}

void GameOverController::draw(is::Render &surface)
{
    surface.draw(m_txtTitle);
    if (m_sceneIndex == is::DisplayOption::GAME_OVER)
    {
        if (m_scene->getOptionIndex() != CONTINUE) surface.draw(m_sprPad1);
        surface.draw(m_txtRestartGame);
    }
    else
    {
        surface.draw(m_txtEndMsg);
        surface.draw(m_txtGetEditor);
    }
    if (m_scene->getOptionIndex() != QUIT) surface.draw(m_sprPad2);
    surface.draw(m_scene->getSprButtonSelect());
    surface.draw(m_txtQuitGame);
}

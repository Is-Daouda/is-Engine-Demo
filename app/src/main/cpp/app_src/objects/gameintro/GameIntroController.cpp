#include "GameIntroController.h"

GameIntroController::GameIntroController(sf::Font &font, sf::Texture &texLogo, sf::Texture &texPad, is::GameDisplay *scene):
    is::MainObject(),
    Step(),
    m_scene(scene),
    m_alphaRec(255),
    m_blind(false),
    m_isOnPad(false),
    m_openLanguageMenu(false),
    m_playSnd(false)
{
    m_imageScale = 0.f;
    m_imageAlpha = 0;

    // transition rectangle
    is::createRectangle(m_recTransition, sf::Vector2f(m_scene->getViewW() + 10.f, m_scene->getViewH() + 10.f), sf::Color(0, 0, 0, m_alphaRec));
    is::setSFMLObjX_Y(m_recTransition, 320.f, 240.f);
    is::createRectangle(m_recChooseLanguage, sf::Vector2f(650.f, 190.f), sf::Color(0, 0, 0), 320.f, 240.f, true);
    is::setSFMLObjScale(m_recChooseLanguage, m_imageScale);

    if (m_scene->getGameSystem().m_firstLaunch)
    {
        is::createWText(font, m_txtChooseLanguage, L"Choose Language", 0.f, 0.f, sf::Color(255, 255, 255), 48);
        is::centerSFMLObj(m_txtChooseLanguage);
        is::setSFMLObjX_Y(m_txtChooseLanguage, m_scene->getViewX(), m_scene->getViewY() - 90.f);

        // create sprites
        float btX(230.f), btY(205.f);
        is::createSprite(texPad, m_scene->getSprButtonSelect(), sf::IntRect(192, 0, 192, 48), sf::Vector2f(btX, btY), sf::Vector2f(96.f, 24.f));
        is::createSprite(texPad, m_sprPadFr, sf::IntRect(192, 0, 192, 48), sf::Vector2f(btX, btY + 66.f), sf::Vector2f(96.f, 24.f));
        is::createText(m_scene->getFontSystem(), m_txtLangEng, is::lang::pad_game_language[is::lang::GameLanguage::ENGLISH],
                       is::getSFMLObjX(m_scene->getSprButtonSelect()), is::getSFMLObjY(m_scene->getSprButtonSelect()) - 6.f,
                       sf::Color(255, 255, 255), true, 25);
        is::createText(m_scene->getFontSystem(), m_txtLangFr, is::lang::pad_game_language[is::lang::GameLanguage::FRANCAIS],
                       is::getSFMLObjX(m_sprPadFr), is::getSFMLObjY(m_sprPadFr) - 6.f, sf::Color(255, 255, 255), true, 25);
        m_openLanguageMenu = true;
    }
    is::createSprite(texLogo, m_sprParent, sf::IntRect(0, 0, 256, 128), sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    WITH (2)
    {
        is::createSprite(texLogo, m_sprGear[_I], sf::IntRect(0, 128, 22, 22), sf::Vector2f(0.f, 0.f), sf::Vector2f(11.f, 11.f));
        is::createSprite(texLogo, m_sprHandGear[_I], sf::IntRect(21, 128, 70, 70), sf::Vector2f(0.f, 0.f), sf::Vector2f(35.f, 35.f));
    }
    is::setSFMLObjX_Y(m_sprGear[0], m_scene->getViewX() - 0.5f, m_scene->getViewY() - 75.5f);
    is::setSFMLObjX_Y(m_sprGear[1], m_scene->getViewX(), m_scene->getViewY() + 38.5f);
    is::setSFMLObjX_Y(m_sprHandGear[0], m_scene->getViewX() - 265.5f, m_scene->getViewY() - 46.f);
    is::setSFMLObjX_Y(m_sprHandGear[1], m_scene->getViewX() + 265.f, m_scene->getViewY() - 46.f);
    is::centerSFMLObj(m_sprParent);
    is::setSFMLObjX_Y(m_sprParent, m_scene->getViewX(), m_scene->getViewY());
}

void GameIntroController::step(float const &DELTA_TIME)
{
    // intro step
    switch(m_step)
    {
        case 0 :
            if (m_openLanguageMenu)
            {
                bool mouseInCollison(false);
                is::increaseVar(DELTA_TIME, m_imageScale, 0.08f, 1.f, 1.f);

                if (!m_scene->getGameSystem().isPressed()) m_scene->getGameSystem().m_keyIsPressed = false;

                if (m_scene->mouseCollision(m_scene->getSprButtonSelect())) mouseInCollison = true;
                else if (m_scene->mouseCollision(m_sprPadFr)) mouseInCollison = true;
                else m_isOnPad = false;

                if (!mouseInCollison && m_scene->getGameSystem().isPressed()) m_scene->getGameSystem().m_keyIsPressed = true;
                auto tempFnct = [this](int index)
                {
                    m_scene->setOptionIndex(index);
                    m_isOnPad = true;
                };

                if (!m_isOnPad)
                {
                    if (m_scene->mouseCollision(m_scene->getSprButtonSelect())) tempFnct(is::lang::GameLanguage::ENGLISH);
                    else if (m_scene->mouseCollision(m_sprPadFr)) tempFnct(is::lang::GameLanguage::FRANCAIS);
                }

                if ((m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_VALIDATION_KEYBOARD) ||
                    (m_scene->getGameSystem().isPressed() && mouseInCollison)) &&
                    (m_scene->getWaitTime() == 0 && !m_scene->getGameSystem().m_keyIsPressed))
                {
                    m_scene->GSMplaySound("select_option"); // We play this sound
                    m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                    m_scene->getGameSystem().m_gameLanguage = m_scene->getOptionIndex();
                    m_scene->getGameSystem().m_firstLaunch = false;
                    m_scene->getGameSystem().saveConfig(is::GameConfig::CONFIG_FILE);
                    m_openLanguageMenu = false;
                }
            }
            else
            {
                is::decreaseVar(DELTA_TIME, m_imageScale, 0.09f, 0.f, 0.f);

                if (!m_blind)
                {
                    if (m_imageAlpha > 90 && !m_playSnd)
                    {
                        m_scene->GSMplaySound("logo_sound");
                        m_playSnd = true;
                    }
                    if (m_imageAlpha < 250) m_imageAlpha += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                    else
                    {
                        m_imageAlpha = 0;
                        m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                        is::setSFMLObjAlpha(m_sprParent, m_imageAlpha);
                        is::setSFMLObjTexRec(m_sprParent, 256, 0, 256, 128);
                        m_blind = true;
                    }
                }
                else
                {
                    if (m_imageAlpha < 240) m_imageAlpha += static_cast<int>((10.f * is::VALUE_CONVERSION) * DELTA_TIME);
                    else m_imageAlpha = 255;
                    is::setSFMLObjAlpha(m_sprParent, m_imageAlpha);
                }

                if (m_alphaRec > 5) m_alphaRec -= static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                else
                {
                    m_alphaRec = 0;
                    if (m_time < 40) m_time += is::getMSecond(DELTA_TIME);
                    else m_step++;
                }
            }
            is::setSFMLObjScale(m_recChooseLanguage, m_imageScale);
        break;

        case 1:
            if (m_alphaRec < 250) m_alphaRec += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
            else
            {
                if (m_time < 50)
                {
                    m_alphaRec = 255;
                    m_time += is::getMSecond(DELTA_TIME);
                    is::setSFMLObjFillColor(m_recChooseLanguage, sf::Color(0, 128, 255));
                    is::setSFMLObjTexRec(m_sprParent, 91, 128, 600, 194);
                    is::centerSFMLObj(m_sprParent);
                }
                else m_step++;
            }
        break;

        case 2:
            is::increaseVar(DELTA_TIME, m_imageScale, 0.06f, 1.2f, 1.2f);
            is::setSFMLObjScaleX_Y(m_recChooseLanguage, 1.2, m_imageScale);
            if (m_alphaRec > 5) m_alphaRec -= static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
            else
            {
                m_alphaRec = 0;
                if (m_time < is::SECOND) m_time += is::getMSecond(DELTA_TIME);
                else m_step++;
            }
        break;

        case 3:
            if (m_alphaRec < 250) m_alphaRec += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
            else
            {
                m_scene->setIsRunning(false);  // quit the scene main loop
                m_scene->getGameSystem().m_launchOption = is::DisplayOption::MAIN_MENU;  // go to main menu scene
                m_step++;
            }
        break;
    }
    is::setSFMLObjRotate(m_sprGear[0], 5.f * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprGear[1], -13.f * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprHandGear[0], 6.3f * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprHandGear[1], -6.3f * is::VALUE_CONVERSION * DELTA_TIME);
    if (m_step < 3) is::setSFMLObjFillColor(m_recTransition, sf::Color(255, 255, 255, m_alphaRec));
    else is::setSFMLObjFillColor(m_recTransition, sf::Color(0, 0, 0, m_alphaRec));
}

void GameIntroController::draw(sf::RenderTexture &surface)
{
    if (m_imageScale > 0.f) surface.draw(m_recChooseLanguage);
    if (m_openLanguageMenu)
    {
        surface.draw(m_scene->getSprButtonSelect());
        surface.draw(m_sprPadFr);
        surface.draw(m_txtChooseLanguage);
        surface.draw(m_txtLangEng);
        surface.draw(m_txtLangFr);
    }
    else
    {
        surface.draw(m_sprParent);
        if (m_step > 1)
        {
            WITH(2)
            {
                surface.draw(m_sprGear[_I]);
                surface.draw(m_sprHandGear[_I]);
            }
        }
        if (m_step < 3) surface.draw(m_recTransition);
    }
}

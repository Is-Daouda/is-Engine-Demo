#include "GameIntroController.h"

GameIntroController::GameIntroController(is::GameDisplay *scene):
    is::MainObject(),
    Step(),
    m_scene(scene),
    m_transitionEffect(scene),
    m_optionIndex(0),
    m_blind(false),
    m_isOnPad(false),
    m_openLanguageMenu(false),
    m_playSnd(false)
{
    m_imageScale = 0.f;
    m_imageAlpha = 0;

    // get texture
    auto &texLogo = m_scene->GRMgetTexture("logo");
    auto &texLogoBg = m_scene->GRMgetTexture("logo_bg");
    auto &texPad = m_scene->GRMgetTexture("language_pad");

    // transition rectangle
    is::createRectangle(m_recChooseLanguage, sf::Vector2f(650.f, 190.f), sf::Color(0, 0, 0), 320.f, 240.f, true);
    is::setSFMLObjScale(m_recChooseLanguage, m_imageScale);

    if (m_scene->getGameSystem().m_firstLaunch)
    {
        is::createText(m_scene->GRMgetFont("font_title"), m_txtChooseLanguage, "Choose Language", m_scene->getViewX(), m_scene->getViewY() - 64.f, sf::Color::White, true, 48);
        is::createSprite(texPad, m_sprButtonSelect, sf::IntRect(0, 0, 90, 96), sf::Vector2f(m_scene->getViewX() - 64.f, m_scene->getViewY() + 28.f), sf::Vector2f(45.f, 48.f));
        is::createSprite(texPad, m_sprPadFr, sf::IntRect(90, 0, 90, 96), sf::Vector2f(m_scene->getViewX() + 64.f, m_scene->getViewY() + 28.f), sf::Vector2f(45.f, 48.f));
        m_openLanguageMenu = true;
    }

    // create sprite
    is::createSprite(texLogo, m_sprLogo, sf::IntRect(0, 0, 256, 128), sf::Vector2f(0.f, 0.f), sf::Vector2f(128.f, 64.f));
    is::createSprite(texLogoBg, m_sprLogoBg, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    WITH (2)
    {
        is::createSprite(texLogo, m_sprGear[_I], sf::IntRect(0, 128, 22, 22), sf::Vector2f(0.f, 0.f), sf::Vector2f(11.f, 11.f));
        is::createSprite(texLogo, m_sprHandGear[_I], sf::IntRect(21, 128, 70, 70), sf::Vector2f(0.f, 0.f), sf::Vector2f(35.f, 35.f));
    }
    is::setSFMLObjX_Y(m_sprGear[0], m_scene->getViewX()
#if defined(IS_ENGINE_SDL_2)
                       + 0.85f
#else
                       - 1.5f
#endif
                       , m_scene->getViewY() - 33.5f);
    is::setSFMLObjX_Y(m_sprGear[1], m_scene->getViewX()
#if defined(IS_ENGINE_SDL_2)
                      + 1.f
#endif
                      , m_scene->getViewY() + 80.5f);
    is::setSFMLObjX_Y(m_sprHandGear[0], m_scene->getViewX()
#if defined(IS_ENGINE_SDL_2)
                      - 265.5f
#else
                      - 262.5f
#endif
                      , m_scene->getViewY() - 4.f);
    is::setSFMLObjX_Y(m_sprHandGear[1], m_scene->getViewX()
#if defined(IS_ENGINE_SDL_2)
                      + 265.f
#else
                      + 258.f
#endif
                      , m_scene->getViewY() - 4.f);
    m_sprLogo.setPosition(m_scene->getViewX(), m_scene->getViewY());
}

void GameIntroController::step(float const &DELTA_TIME)
{
    // DELTA TIME
    float const SPEED(1.2f);
    float const ROTATION_SPEED(7.f);
    float const TRANSITION_SPEED(static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME));

    // intro animation step
    switch(m_step)
    {
    case 0:
        if (m_openLanguageMenu)
        {
            bool mouseInCollison(false);
            is::increaseVar(DELTA_TIME, m_imageScale, 0.08f, 1.f, 1.f);

            if (!m_scene->getGameSystem().isPressed()) m_scene->getGameSystem().m_keyIsPressed = false;

            if (m_scene->mouseCollision(m_sprButtonSelect)) mouseInCollison = true;
            else if (m_scene->mouseCollision(m_sprPadFr)) mouseInCollison = true;
            else m_isOnPad = false;
            if (!mouseInCollison && m_scene->getGameSystem().isPressed()) m_scene->getGameSystem().m_keyIsPressed = true;
            auto tempFnct = [this](int index)
            {
                m_optionIndex = index;
                m_isOnPad = true;
            };

            if (!m_isOnPad)
            {
                if (m_scene->mouseCollision(m_sprButtonSelect)) tempFnct(is::lang::GameLanguage::ENGLISH);
                else if (m_scene->mouseCollision(m_sprPadFr)) tempFnct(is::lang::GameLanguage::FRANCAIS);
            }

            if ((m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_VALIDATION_KEYBOARD) ||
                (m_scene->getGameSystem().isPressed(is::GameSystem::MOUSE) && mouseInCollison)) &&
                !m_scene->getGameSystem().m_keyIsPressed)
            {
                m_scene->GSMplaySound("select_option");
                m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                m_scene->getGameSystem().m_gameLanguage = m_optionIndex;
                m_scene->getGameSystem().m_firstLaunch = false;
                m_scene->getGameSystem().saveConfig(is::GameConfig::CONFIG_FILE);
                m_openLanguageMenu = false;
            }
        }
        else
        {
            is::decreaseVar(DELTA_TIME, m_imageScale, 0.08f, 0.f, 0.f);
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
                    m_sprLogo.setTextureRect(sf::IntRect(256, 0, 256, 128));
                    m_sprLogo.setColor(sf::Color(255, 255, 255, m_imageAlpha));
                    m_blind = true;
                }
            }
            else
            {
                if (m_imageAlpha < 240) m_imageAlpha += static_cast<int>((10.f * is::VALUE_CONVERSION) * DELTA_TIME);
                else m_imageAlpha = 255;
                m_sprLogo.setColor(sf::Color(255, 255, 255, m_imageAlpha));
            }

            // transition effect start
            if (m_transitionEffect.getImageAlpha() > 5) m_transitionEffect.setImageAlpha(m_transitionEffect.getImageAlpha() - TRANSITION_SPEED);
            else
            {
                m_transitionEffect.setImageAlpha(0);
                if (m_time < 50) m_time += static_cast<int>(SPEED * is::VALUE_CONVERSION * DELTA_TIME);
                else m_step++;
            }
        }
        is::setSFMLObjScaleX_Y(m_recChooseLanguage, 1.f, m_imageScale);
    break;

    case 1:
        // transition effect end
        if (m_transitionEffect.getImageAlpha() < 250) m_transitionEffect.setImageAlpha(m_transitionEffect.getImageAlpha() + TRANSITION_SPEED);
        else
        {
            if (m_time < 55)
            {
                m_transitionEffect.setImageAlpha(255);
                is::setSFMLObjFillColor(m_recChooseLanguage, sf::Color(0, 128, 255));
                m_time += static_cast<int>(SPEED * is::VALUE_CONVERSION * DELTA_TIME);
                is::setSFMLObjTexRec(m_sprLogo, 91, 128, 600, 282);
                is::centerSFMLObj(m_sprLogo);
            }
            else
            {
                #if defined(IS_ENGINE_SDL_2)
                is::setSFMLObjX(m_sprLogo, m_scene->getViewX() - 4.f);
                is::setSFMLObjY(m_recChooseLanguage, is::getSFMLObjY(m_recChooseLanguage) - 56.f);
                #endif // defined
                m_step++;
            }
        }
    break;
    case 2:
        is::increaseVar(DELTA_TIME, m_imageScale, 0.06f, 1.6f, 1.6f);
        is::setSFMLObjScaleX_Y(m_recChooseLanguage, 1.2, m_imageScale);
        if (m_scene->getGameSystem().isPressed(is::GameSystem::ALL_BUTTONS)) m_step++;

        // transition effect start
        if (m_transitionEffect.getImageAlpha() > 5) m_transitionEffect.setImageAlpha(m_transitionEffect.getImageAlpha() - TRANSITION_SPEED);
        else
        {
            m_transitionEffect.setImageAlpha(0);
            if (m_time < 95) m_time += static_cast<int>(SPEED * is::VALUE_CONVERSION * DELTA_TIME);
            else m_step++;
        }
    break;
    case 3:
        // transition effect end
        if (m_transitionEffect.getImageAlpha() < 250) m_transitionEffect.setImageAlpha(m_transitionEffect.getImageAlpha() + TRANSITION_SPEED);
        else
        {
            m_scene->setIsRunning(false);  // quit the main render loop
            m_scene->getGameSystem().m_launchOption = is::DisplayOption::MAIN_MENU;  // go to main menu
            m_step++;
        }
    break;
    }
    is::setSFMLObjRotate(m_sprGear[0], ROTATION_SPEED + 2.f * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprGear[1], -(ROTATION_SPEED + 4.f) * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprHandGear[0], ROTATION_SPEED * is::VALUE_CONVERSION * DELTA_TIME);
    is::setSFMLObjRotate(m_sprHandGear[1], -ROTATION_SPEED * is::VALUE_CONVERSION * DELTA_TIME);
    if (m_step <= 2) is::setSFMLObjFillColor(m_transitionEffect.getRecTransition(), sf::Color(255, 255, 255, m_transitionEffect.getImageAlpha()));
    else is::setSFMLObjFillColor(m_transitionEffect.getRecTransition(), sf::Color(0, 0, 0, m_transitionEffect.getImageAlpha()));
}

void GameIntroController::draw(is::Render &surface)
{
    if (m_imageScale > 0.f) is::draw(surface, m_recChooseLanguage);
    if (m_openLanguageMenu)
    {
        is::draw(surface, m_sprButtonSelect);
        is::draw(surface, m_sprPadFr);
        is::draw(surface, m_txtChooseLanguage);
    }
    else
    {
        if (m_step < 2) is::draw(surface, m_sprLogoBg);
        is::draw(surface, m_sprLogo);
        if (m_step > 1)
        {
            WITH(2)
            {
                is::draw(surface, m_sprGear[_I]);
                is::draw(surface, m_sprHandGear[_I]);
            }
        }
        is::draw(surface, m_transitionEffect.getRecTransition());
    }
}

#include "MenuController.h"

MenuController::MenuController(is::GameDisplay *scene):
    m_pageName(PAGE_MAIN_MENU),
    m_scene(scene),
    m_gameOptionIndex(0),
    m_sprButtonPemuteScale(1.f),
    m_timeClick(0.f),
    m_isClicked(false),
    m_padCfgClicked(false),
    m_saveFileOpen(false)
{
    m_strName = "MenuController";

    for (unsigned int i(0); i < is::arraySize(m_txtOptionScale); i++) m_txtOptionScale[i] = 1.f;

    // load configuration file
    if (m_scene->getGameSystem().fileExist(is::GameConfig::CONFIG_FILE)) m_scene->getGameSystem().loadConfig(is::GameConfig::CONFIG_FILE);

    float distance(5.f), widgetSpace(55.f), xPos(147.f), yPos(274.f);

    // create sprites
    is::createSprite(m_scene->GRMgetTexture("main_menu_pad"), m_sprPad1, sf::IntRect(0, 0, 192, 48), sf::Vector2f(xPos, yPos + distance), sf::Vector2f(96.f, 24.f));
    is::createSprite(m_scene->GRMgetTexture("main_menu_pad"), m_sprPad2, sf::IntRect(0, 0, 192, 48), sf::Vector2f(xPos, yPos + widgetSpace + distance), sf::Vector2f(96.f, 24.f));
    is::createSprite(m_scene->GRMgetTexture("main_menu_pad"), m_sprPad3, sf::IntRect(0, 0, 192, 48), sf::Vector2f(xPos, yPos + widgetSpace * 2.f + distance), sf::Vector2f(96.f, 24.f));
    is::createSprite(m_scene->GRMgetTexture("main_menu_pad"), m_scene->getSprButtonSelect(), sf::IntRect(192, 0, 192, 48), sf::Vector2f(xPos, yPos + distance), sf::Vector2f(96.f, 24.f));
    float btYSize(0.9f);
    is::setSFMLObjScaleX_Y(m_sprPad1, 1.f, btYSize);
    is::setSFMLObjScaleX_Y(m_sprPad2, 1.f, btYSize);
    is::setSFMLObjScaleX_Y(m_sprPad3, 1.f, btYSize);

    // create text for main menu
    float const TXT_Y_ON_BT(8.f);
    int const _PAD_TXT_SIZE(22);
    is::createText(m_scene->getFontSystem(), m_txtStartGame, is::lang::pad_new_game[m_scene->getGameSystem().m_gameLanguage],
                   is::getSFMLObjX(m_sprPad1), is::getSFMLObjY(m_sprPad1) - TXT_Y_ON_BT, sf::Color::Blue, true, _PAD_TXT_SIZE);
    is::createText(m_scene->getFontSystem(), m_txtContinue, is::lang::pad_continue_game[m_scene->getGameSystem().m_gameLanguage],
                   is::getSFMLObjX(m_sprPad2), is::getSFMLObjY(m_sprPad2) - TXT_Y_ON_BT, true, _PAD_TXT_SIZE);
    is::createText(m_scene->getFontSystem(), m_txtPadConfig, is::lang::pad_controller[m_scene->getGameSystem().m_gameLanguage],
                   is::getSFMLObjX(m_sprPad3), is::getSFMLObjY(m_sprPad3) - TXT_Y_ON_BT, sf::Color(0, 0, 0), _PAD_TXT_SIZE);
    is::centerSFMLObj(m_txtPadConfig);

    // create text for game option
    int const _SIZE(26);
    float const _X_OPT(m_scene->getViewX() - 55.f), _Y_OPT(m_scene->getViewY() + 10.f), _OPT_SPACE(44.f);
    is::createText(m_scene->getFontSystem(), m_txtOptionValue[0], is::lang::opt_enable_sound[m_scene->getGameSystem().m_gameLanguage],
                   _X_OPT, _Y_OPT, sf::Color(sf::Color(0, 0, 0)), _SIZE);
    is::createText(m_scene->getFontSystem(), m_txtOptionValue[1], is::lang::opt_enable_music[m_scene->getGameSystem().m_gameLanguage],
                   _X_OPT, _Y_OPT + _OPT_SPACE, sf::Color(sf::Color(0, 0, 0)), _SIZE);
    is::createText(m_scene->getFontSystem(), m_txtOptionValue[2], is::lang::opt_enable_vibrate[m_scene->getGameSystem().m_gameLanguage],
                   _X_OPT, _Y_OPT + _OPT_SPACE * 2.f, sf::Color(sf::Color(0, 0, 0)), _SIZE);
    is::createText(m_scene->getFontSystem(), m_txtOptionValue[3], is::lang::opt_game_lang[m_scene->getGameSystem().m_gameLanguage],
                   _X_OPT, _Y_OPT + _OPT_SPACE * 3.f, sf::Color(sf::Color(0, 0, 0)), _SIZE);

    // load game data
    if (m_scene->getGameSystem().fileExist(is::GameConfig::GAME_DATA_FILE))
    {
        m_scene->getGameSystem().loadData(is::GameConfig::GAME_DATA_FILE);
        if (m_scene->getGameSystem().m_gameProgression > m_scene->getGameSystem().m_levelNumber) m_scene->getGameSystem().m_gameProgression = m_scene->getGameSystem().m_levelNumber;
        m_saveFileOpen = true;
    }

    // load configuration data
    m_scene->getGameSystem().loadPadConfig(is::GameConfig::GAME_PAD_FILE);

    float _xPos(150.f);
    is::createSprite(m_scene->GRMgetTexture("option_pad"), m_sprPermuteAB, sf::IntRect(160, 0, 160, 32), sf::Vector2f(0.f, 0.f), sf::Vector2f(80.f, 16.f));
    is::setSFMLObjX_Y(m_sprPermuteAB, m_scene->getViewX() - _xPos - 20.f, m_scene->getViewY() - 80.f);
    is::createText(m_scene->getFontSystem(), m_txtPermuteAB, is::lang::msg_permute_AB[m_scene->getGameSystem().m_gameLanguage] + getABPlace(), 0.f, 0.f, sf::Color(255, 255, 255));
    is::centerSFMLObj(m_txtPermuteAB);
    is::setSFMLObjX_Y(m_txtPermuteAB, is::getSFMLObjX(m_sprPermuteAB), is::getSFMLObjY(m_sprPermuteAB) - TXT_Y_ON_BT + 1.f);

    is::createText(m_scene->getFontSystem(), m_txtMovePad, is::lang::msg_how_move_control[m_scene->getGameSystem().m_gameLanguage], 0.f, 0.f, sf::Color(255, 255, 255, 80));
    is::centerSFMLObj(m_txtMovePad);
    is::setSFMLObjX_Y(m_txtMovePad, m_scene->getViewX(), m_scene->getViewY());

    is::createText(m_scene->getFontSystem(), m_txtSetAlpha, "Transparence", m_scene->getViewX() + _xPos, m_scene->getViewY() - 85.f, true, 20);
    is::setSFMLObjFillColor(m_txtSetAlpha, sf::Color::White);

    float const _DIST(65.f);
    is::createSprite(m_scene->GRMgetTexture("tools_pad"), m_sprAddAlpha, sf::IntRect(32, 0, 32, 32),
                     sf::Vector2f(is::getSFMLObjX(m_txtSetAlpha) + _DIST, is::getSFMLObjY(m_txtSetAlpha) - 10.f), sf::Vector2f(16.f, 16.f));
    is::createSprite(m_scene->GRMgetTexture("tools_pad"), m_sprReduceAlpha, sf::IntRect(96, 0, 32, 32),
                     sf::Vector2f(is::getSFMLObjX(m_txtSetAlpha) - _DIST - 32.f, is::getSFMLObjY(m_txtSetAlpha) - 10.f), sf::Vector2f(16.f, 16.f));
    is::createSprite(m_scene->GRMgetTexture("game_pad"), m_sprJoystick[0], sf::IntRect(0, 0, 134, 134), sf::Vector2f(0.f, 0.f), sf::Vector2f(67.f, 67.f));
    is::createSprite(m_scene->GRMgetTexture("game_pad"), m_sprJoystick[1], sf::IntRect(134, ((!m_scene->getGameSystem().m_permutePadAB) ? 0 : 67), 144, 67), sf::Vector2f(0.f, 0.f), sf::Vector2f(72.f, 37.f));
    is::createRectangle(m_recLine, sf::Vector2f(650.f, 1.5f), sf::Color::White, m_scene->getViewX(), m_scene->getViewY() - 34.f, true);
    is::createRectangle(m_recSelectPad, sf::Vector2f(134.f, 134.f), sf::Color::Transparent, 0.f, 0.f, true);
    is::setSFMLObjOutlineColor(m_recSelectPad, 1.f, sf::Color::Red);

    is::createRectangle(m_recCfgBg, sf::Vector2f(m_scene->getViewW() + 10.f, m_scene->getViewH() + 10.f), sf::Color(0, 0, 0, 230), m_scene->getViewX(), m_scene->getViewY(), true);
}

void MenuController::step(float const &DELTA_TIME)
{
    // m_isClose allow to deactivate scene object
    if (!m_scene->m_isClose)
    {
        // check with collision with sprite
        auto cancelBt = m_scene->SDMgetObject("CancelButton");
        if (m_scene->mouseCollision(cancelBt->getSprite())) cancelBt->setImageAlpha(255);

        m_padCfgClicked = false;

        // allows activated use of buttons
        if (!m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_UP) &&
            !m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_DOWN) &&
            !m_scene->getGameSystem().isPressed())
                m_scene->getGameSystem().m_keyIsPressed = false;

        // allow to know is mouse is in collision with sprite
        bool mouseInCollisonPad(false);
        bool mouseInCollisonOption(false);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      MAIN MENU
//////////////////////////////////////////////////////////////////////////////////////////////////////
        if (m_pageName != PAGE_PAD_CONFIG)
        {
            if (m_scene->mouseCollision(m_sprPad1) || m_scene->mouseCollision(m_sprPad2) ||
                m_scene->mouseCollision(m_sprPad3)) mouseInCollisonPad = true;

            // check collision with option buttons
            for (unsigned int i(0); i < is::arraySize(m_txtOptionValue); i++)
            {
                if (m_scene->mouseCollision(m_txtOptionValue[i])) mouseInCollisonOption = true;
            }

            // change option with mouse (touch on Android)
            if (m_scene->mouseCollision(m_sprPad1) && m_scene->getOptionIndex() != OP_START_GAME)  m_scene->setOptionIndex(OP_START_GAME, true, 1.4f);
            if (m_scene->mouseCollision(m_sprPad2) && m_scene->getOptionIndex() != OP_CONTINUE)    m_scene->setOptionIndex(OP_CONTINUE, true, 1.4f);
            if (m_scene->mouseCollision(m_sprPad3) && m_scene->getOptionIndex() != OP_CONFIG_PAD)  m_scene->setOptionIndex(OP_CONFIG_PAD, true, 1.4f);

            // avoid the long pressing button effect
            if (!mouseInCollisonPad && !mouseInCollisonOption && m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE))
                m_scene->getGameSystem().m_keyIsPressed = true;

            // change option with keyboard (only for PC)
            if (!m_scene->getGameSystem().m_keyIsPressed && !mouseInCollisonPad)
            {
                if (m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_UP)) m_scene->setOptionIndex(-1, false, 1.4f);
                else if (m_scene->getGameSystem().keyIsPressed(is::GameConfig::KEY_DOWN)) m_scene->setOptionIndex(1, false, 1.4f);
                if (m_scene->getOptionIndex() < 0) m_scene->setOptionIndex(OP_CONFIG_PAD);
                if (m_scene->getOptionIndex() > OP_CONFIG_PAD) m_scene->setOptionIndex(0);
            }

            // validate menu option
            if ((m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::KEYBOARD) ||
                (m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE) && mouseInCollisonPad)) &&
                (m_scene->getWaitTime() == 0 && !m_scene->getGameSystem().m_keyIsPressed))
            {
                auto playSelectSnd = [this]()
                {
                    m_scene->GSMplaySound("select_option"); // We play this sound
                    m_scene->setSprButtonSelectScale(1.4f);
                    m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                };

                switch (m_scene->getOptionIndex())
                {
                case OP_START_GAME:
                    if (!m_scene->getGameSystem().fileExist(is::GameConfig::GAME_DATA_FILE))
                    {
                        playSelectSnd();
                        m_scene->getGameSystem().saveData(is::GameConfig::GAME_DATA_FILE);
                        m_scene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;
                        m_scene->m_isClose = true;
                    }
                    else
                    {
                        m_scene->showMessageBox(is::lang::msg_erase_data[m_scene->getGameSystem().m_gameLanguage]);
                    }
                break;

                // load current level saved
                case OP_CONTINUE:
                    if (m_scene->getGameSystem().fileExist(is::GameConfig::GAME_DATA_FILE))
                    {
                        m_scene->GSMplaySound("select_option"); // We play this sound
                        m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                        m_scene->getGameSystem().m_currentLevel = m_scene->getGameSystem().m_gameProgression;
                        m_scene->getGameSystem().m_launchOption = is::DisplayOption::GAME_LEVEL;
                        m_scene->m_isClose = true;
                    }
                break;

                // show game pad configuration option
                case OP_CONFIG_PAD:
                    #if !defined(__ANDROID__)
                    m_scene->showMessageBox(is::lang::msg_controller_opt_error[m_scene->getGameSystem().m_gameLanguage], false);
                    #else
                    playSelectSnd();
                    m_scene->getGameSystem().m_keyIsPressed = true;
                    m_pageName = PAGE_PAD_CONFIG;
                    #endif // defined
                break;
                }
                m_scene->setKeyBackPressed(false);
            }
            if (m_pageName != PAGE_PAD_CONFIG)
            {
                // Quit game
                if (m_scene->getKeyBackPressed())
                {
                    m_scene->showMessageBox(is::lang::msg_quit_game[m_scene->getGameSystem().m_gameLanguage]);
                    m_closeApplication = true;
                    m_scene->setKeyBackPressed(false);
                }
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      GAME OPTION
//////////////////////////////////////////////////////////////////////////////////////////////////////

            // mouse hover effect
            auto setTxtOpionScale = [this](short int m_txtOptionIndex)
            {
                //////////////////////////////////////////////////////////
                #if !defined(__ANDROID__)
                m_scene->GSMplaySound("change_option"); // We play this sound
                #endif
                //////////////////////////////////////////////////////////
                m_txtOptionScale[m_txtOptionIndex] = 1.2f;
                m_gameOptionIndex = m_txtOptionIndex;
            };

            if (m_scene->mouseCollision(m_txtOptionValue[OP_MENU_SND]) && m_gameOptionIndex != OP_MENU_SND)
            {
                setTxtOpionScale(OP_MENU_SND);
            }
            else if (m_scene->mouseCollision(m_txtOptionValue[OP_MENU_MSC]) && m_gameOptionIndex != OP_MENU_MSC)
            {
                setTxtOpionScale(OP_MENU_MSC);
            }
            else if (m_scene->mouseCollision(m_txtOptionValue[OP_MENU_VIBRATE]) && m_gameOptionIndex != OP_MENU_VIBRATE)
            {
                setTxtOpionScale(OP_MENU_VIBRATE);
            }
            else if (m_scene->mouseCollision(m_txtOptionValue[OP_MENU_LANG]) && m_gameOptionIndex != OP_MENU_LANG)
            {
                setTxtOpionScale(OP_MENU_LANG);
            }

            if (m_scene->getGameSystem().isPressed() && mouseInCollisonOption && m_scene->getWaitTime() == 0 && !m_scene->getGameSystem().m_keyIsPressed)
            {
                // validate option
                auto setKeyToTrue = [this](short int m_txtOptionIndex)
                {
                    m_txtOptionScale[m_txtOptionIndex] = 1.4f;
                    m_scene->getGameSystem().m_keyIsPressed = true;
                    m_scene->getGameSystem().saveConfig(is::GameConfig::CONFIG_FILE);
                };

                switch (m_gameOptionIndex)
                {
                    // activate / deactivate sound
                    case OP_MENU_SND:
                        if (!m_scene->getGameSystem().m_enableSound)
                        {
                            m_scene->getGameSystem().m_enableSound = true;
                            m_scene->GSMplaySound("select_option"); // We play this sound
                        }
                        else m_scene->getGameSystem().m_enableSound = false;
                        setKeyToTrue(OP_MENU_SND);
                    break;

                    // activate / deactivate music
                    case OP_MENU_MSC:
                        if (!m_scene->getGameSystem().m_enableMusic)
                        {
                            m_scene->getGameSystem().m_enableMusic = true;
                            m_scene->GSMplaySound("select_option"); // We play this sound
                        }
                        else m_scene->getGameSystem().m_enableMusic = false;
                        setKeyToTrue(OP_MENU_MSC);
                    break;

                    case OP_MENU_VIBRATE:
                        if (!m_scene->getGameSystem().m_enableVibrate)
                        {
                            m_scene->getGameSystem().m_enableVibrate = true;
                            m_scene->GSMplaySound("select_option"); // We play this sound
                            m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                        }
                        else m_scene->getGameSystem().m_enableVibrate = false;
                        setKeyToTrue(OP_MENU_VIBRATE);
                    break;

                    // change game language
                    case OP_MENU_LANG:
                        // With each click we change the language index
                        m_scene->getGameSystem().m_gameLanguage++;

                        // Here the enumeration FRANCAIS represents the value of the index of the last language
                        // If the variable is greater than this value we return to the first language
                        // because there are more languages
                        if (m_scene->getGameSystem().m_gameLanguage > is::lang::GameLanguage::FRANCAIS) m_scene->getGameSystem().m_gameLanguage = is::lang::GameLanguage::ENGLISH;
                        m_scene->GSMplaySound("select_option"); // We play this sound

                        m_txtStartGame.setString(is::lang::pad_new_game[m_scene->getGameSystem().m_gameLanguage]);
                        m_txtContinue.setString(is::lang::pad_continue_game[m_scene->getGameSystem().m_gameLanguage]);
                        m_txtPadConfig.setString(is::lang::pad_controller[m_scene->getGameSystem().m_gameLanguage]);
                        m_txtPermuteAB.setString(is::lang::msg_permute_AB[m_scene->getGameSystem().m_gameLanguage] + getABPlace());
                        m_txtMovePad.setString(is::lang::msg_how_move_control[m_scene->getGameSystem().m_gameLanguage]);
                        is::centerSFMLObj(m_txtStartGame);
                        is::centerSFMLObj(m_txtContinue);
                        is::centerSFMLObj(m_txtPadConfig);
                        is::centerSFMLObj(m_txtPermuteAB);
                        is::centerSFMLObj(m_txtMovePad);

                        setKeyToTrue(OP_MENU_LANG);
                    break;
                }
            }

            // update game option
            if (m_scene->getGameSystem().m_enableSound) m_txtOptionValue[OP_MENU_SND].setString(is::lang::opt_enable_sound[m_scene->getGameSystem().m_gameLanguage]);
            else m_txtOptionValue[OP_MENU_SND].setString(is::lang::opt_disable_sound[m_scene->getGameSystem().m_gameLanguage]);
            if (m_scene->getGameSystem().m_enableMusic) m_txtOptionValue[OP_MENU_MSC].setString(is::lang::opt_enable_music[m_scene->getGameSystem().m_gameLanguage]);
            else m_txtOptionValue[OP_MENU_MSC].setString(is::lang::opt_disable_music[m_scene->getGameSystem().m_gameLanguage]);
            if (m_scene->getGameSystem().m_enableVibrate) m_txtOptionValue[OP_MENU_VIBRATE].setString(is::lang::opt_enable_vibrate[m_scene->getGameSystem().m_gameLanguage]);
            else m_txtOptionValue[OP_MENU_VIBRATE].setString(is::lang::opt_disable_vibrate[m_scene->getGameSystem().m_gameLanguage]);
            m_txtOptionValue[OP_MENU_LANG].setString(is::lang::opt_game_lang[m_scene->getGameSystem().m_gameLanguage]);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      GAME KEY CONFIG (ONLY FOR ANDROID)
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else
        {
            auto setKeyPos = [this](int index, float &xVar, float &yVar)
            {
                sf::Vector2f pos;
                if (m_scene->mouseCollision(m_sprJoystick[index], pos) && m_scene->getGameSystem().isPressed())
                {
                    m_padCfgClicked = true;
                    bool haveError(false), canMove(true);
                    auto checkPadDistance = [this, &pos, &canMove](int _index)
                    {
                        if (is::pointDistance(pos.x, pos.y,
                                              is::getSFMLObjX(m_sprJoystick[_index]),
                                              is::getSFMLObjY(m_sprJoystick[_index])) < 160.f) canMove = false;
                    };
                    if (index == 0) checkPadDistance(1); else checkPadDistance(0);

                    if (canMove)
                    {
                        float _x(((pos.x > m_scene->getViewX()) ? 1.f : -1.f) * is::pointDistance(pos.x, pos.y, m_scene->getViewX(), pos.y));
                        float _y(((pos.y > m_scene->getViewY()) ? 1.f : -1.f) * is::pointDistance(pos.x, pos.y, pos.x, m_scene->getViewY()));

                        float _xSize(67.f), _ySize(67.f);
                        if (index == 1)
                        {
                            _xSize = 72.f;
                            _ySize = 37.f;
                        }

                        if (pos.x < _xSize)
                        {
                            _x = _xSize;
                            haveError = true;
                        }
                        else if (pos.x > m_scene->getViewX() + m_scene->getViewW() / 2.f - _xSize)
                        {
                            haveError = true;
                            _x = m_scene->getViewX() + m_scene->getViewW() / 2.f - _xSize;
                        }
                        else xVar = _x;

                        if (pos.y < m_scene->getViewY() + ((index == 0) ? 32.f : 4.f))
                        {
                            _y = _ySize + 32.f;
                            haveError = true;
                        }
                        else if (pos.y > m_scene->getViewY() + m_scene->getViewH() / 2.f - _ySize)
                        {
                            _y = m_scene->getViewY() + m_scene->getViewH() / 2.f - _ySize;
                            haveError = true;
                        }
                        else yVar = _y;
                    } else haveError = true;
                    is::setSFMLObjOutlineColor(m_recSelectPad, ((haveError) ? sf::Color(255, 0, 0) : sf::Color(0, 255, 0)));
                    is::setSFMLObjSize(m_recSelectPad, is::getSFMLObjWidth(m_sprJoystick[index]), is::getSFMLObjHeight(m_sprJoystick[index]));
                    is::centerSFMLObj(m_recSelectPad);
                    is::setSFMLObjX_Y(m_recSelectPad, is::getSFMLObjX(m_sprJoystick[index]), is::getSFMLObjY(m_sprJoystick[index]));
                }
            };

            setKeyPos(0, m_scene->getGameSystem().m_padDirXPos, m_scene->getGameSystem().m_padDirYPos);
            setKeyPos(1, m_scene->getGameSystem().m_padActionXPos, m_scene->getGameSystem().m_padActionYPos);
            is::setSFMLObjX_Y(m_sprJoystick[0],
                              m_scene->getViewX() + m_scene->getGameSystem().m_padDirXPos,
                              m_scene->getViewY() + m_scene->getGameSystem().m_padDirYPos);
            is::setSFMLObjX_Y(m_sprJoystick[1],
                              m_scene->getViewX() + m_scene->getGameSystem().m_padActionXPos,
                              m_scene->getViewY() + m_scene->getGameSystem().m_padActionYPos);

            // This avoids the repetition of certain actions when we touch the buttons
            if (m_timeClick > 10.f &&
                (m_scene->mouseCollision(m_sprPermuteAB) || m_scene->mouseCollision(m_sprAddAlpha) || m_scene->mouseCollision(m_sprReduceAlpha)) &&
                            m_scene->getGameSystem().isPressed())
                m_timeClick = 15.f;
            else is::decreaseVar(DELTA_TIME, m_timeClick, 1.f, 0.f, 0.f);

            if (m_scene->mouseCollision(m_sprAddAlpha) && m_scene->getGameSystem().isPressed())
            {
                if (static_cast<int>(m_timeClick) == 0) m_scene->GSMplaySound("select_option"); // We play this sound
                m_timeClick = 15.f;
                if (m_scene->getGameSystem().m_padAlpha < 250) m_scene->getGameSystem().m_padAlpha += ((is::VALUE_CONVERSION * 2.f) * DELTA_TIME);
                is::setSFMLObjTexRec(m_sprAddAlpha, 64, 0, 32, 32);
            }
            else is::setSFMLObjTexRec(m_sprAddAlpha, 32, 0, 32, 32);

            if (m_scene->mouseCollision(m_sprReduceAlpha) && m_scene->getGameSystem().isPressed())
            {
                if (static_cast<int>(m_timeClick) == 0) m_scene->GSMplaySound("select_option"); // We play this sound
                m_timeClick = 15.f;
                if (m_scene->getGameSystem().m_padAlpha > 20) m_scene->getGameSystem().m_padAlpha -= ((is::VALUE_CONVERSION * 2.f) * DELTA_TIME);
                is::setSFMLObjTexRec(m_sprReduceAlpha, 128, 0, 32, 32);
            }
            else is::setSFMLObjTexRec(m_sprReduceAlpha, 96, 0, 32, 32);

            if (m_scene->mouseCollision(m_sprPermuteAB) && m_scene->getGameSystem().isPressed() && static_cast<int>(m_timeClick) == 0)
            {
                m_scene->GSMplaySound("select_option"); // We play this sound
                m_timeClick = 15.f;
                m_scene->getGameSystem().m_permutePadAB = !m_scene->getGameSystem().m_permutePadAB;
                m_sprButtonPemuteScale = 1.3f;
                is::setSFMLObjTexRec(m_sprJoystick[1], 134, ((!m_scene->getGameSystem().m_permutePadAB) ? 0 : 67), 144, 67);
                m_txtPermuteAB.setString(is::lang::msg_permute_AB[m_scene->getGameSystem().m_gameLanguage] + getABPlace());
                is::centerSFMLObj(m_txtPermuteAB);
            }

            for (int i(0); i < 2; i++) is::setSFMLObjAlpha(m_sprJoystick[i], m_scene->getGameSystem().m_padAlpha);

            if (!mouseInCollisonOption && m_scene->getGameSystem().isPressed()) m_scene->getGameSystem().m_keyIsPressed = true;

            // hide game pad configuration
            if ((m_scene->getKeyBackPressed() || (m_scene->getGameSystem().isPressed() &&
                                                  static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"))->isOn())) &&
                m_scene->getWaitTime() == 0)
            {
                m_scene->getGameSystem().savePadConfig(is::GameConfig::GAME_PAD_FILE);
                m_scene->GSMplaySound("cancel"); // We play this sound
                m_scene->getGameSystem().useVibrate(m_scene->getVibrateTimeDuration());
                m_pageName = PAGE_MAIN_MENU;
                m_scene->setKeyBackPressed(false);
            }
        }

        // change the color of the texts according to the chosen option
        m_scene->setTextAnimation(m_txtStartGame, m_sprPad1, OP_START_GAME);
        m_scene->setTextAnimation(m_txtContinue,  m_sprPad2, OP_CONTINUE);
        m_scene->setTextAnimation(m_txtPadConfig, m_sprPad3, OP_CONFIG_PAD);
    }

    // game option texts animation
    for (unsigned int i(0); i < is::arraySize(m_txtOptionValue); i++)
    {
        m_scene->setTextAnimation(m_txtOptionValue[i], m_gameOptionIndex, i);
        is::scaleAnimation(DELTA_TIME, m_txtOptionScale[i], m_txtOptionValue[i]);
    }

    // PAD animation
    is::scaleAnimation(DELTA_TIME, m_scene->getSprButtonSelectScale(), m_scene->getSprButtonSelect(), m_sprPad1.getScale().x);
    is::scaleAnimation(DELTA_TIME, m_scene->getSprButtonSelectScale(), m_sprPermuteAB, 1);

    if (m_scene->m_isClose)
    {
        m_scene->setSceneStart(false);
        m_scene->setIsRunning(false);
    }
}

void MenuController::draw(is::Render &surface)
{
    static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"))->setVisible(m_pageName == PAGE_PAD_CONFIG);

    // draw button
    if (m_scene->getOptionIndex() != OP_START_GAME) surface.draw(m_sprPad1);
    if (m_scene->getOptionIndex() != OP_CONTINUE) surface.draw(m_sprPad2);
    if (m_scene->getOptionIndex() != OP_CONFIG_PAD) surface.draw(m_sprPad3);
    surface.draw(m_scene->getSprButtonSelect());
    surface.draw(m_txtStartGame);
    surface.draw(m_txtContinue);
    surface.draw(m_txtPadConfig);
    for (unsigned int i(0); i < is::arraySize(m_txtOptionValue); i++) surface.draw(m_txtOptionValue[i]);

    // show the game pad configuration page
    if (m_pageName == PAGE_PAD_CONFIG)
    {
        surface.draw(m_recCfgBg);
        surface.draw(m_txtMovePad);
        surface.draw(m_sprPermuteAB);
        surface.draw(m_txtPermuteAB);
        surface.draw(m_txtSetAlpha);
        surface.draw(m_sprAddAlpha);
        surface.draw(m_sprReduceAlpha);
        for (int i(0); i < 2; i++) surface.draw(m_sprJoystick[i]);
        if (m_padCfgClicked) surface.draw(m_recSelectPad);
        surface.draw(m_recLine);
    }
}

#include "GameMenu.h"

GameMenu::GameMenu(is::GameSystemExtended &gameSysExt):
    GameDisplay(gameSysExt, sf::Color::White) {}

void GameMenu::loadResources()
{
    GameDisplay::loadParentResources();
    m_gameSysExt.initData(true);

    // load textures
    GRMaddTexture("main_menu_pad", is::GameConfig::GUI_DIR + "main_menu_pad.png");
    GRMaddTexture("option_pad", is::GameConfig::GUI_DIR + "option_pad.png");
    GRMaddTexture("tools_pad", is::GameConfig::GUI_DIR + "tools_pad.png");
    GRMaddTexture("game_pad", is::GameConfig::GUI_DIR + "game_pad.png");
    auto &bg = GRMaddTexture("menu_background", is::GameConfig::SPRITES_DIR + "menu_background.png");

    SDMaddSceneObject(std::make_shared<is::Background>(bg, 0.f, 0.f, this, 0.f, 0.f, false, false));
    SDMaddSceneObject(std::make_shared<MenuController>(this));
    SDMaddSceneObject(std::make_shared<CancelButton>(this));
}

void GameMenu::SDMmanageSceneEvents()
{
    // even loop
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        controlEventFocusClosing(event);
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == is::GameConfig::KEY_CANCEL)
            {
                m_keyBackPressed = true;
            }
        }
    }
}

void GameMenu::SDMmanageSceneMsgAnswers()
{
    // when user closes message box in update function execute this instruction
    // "m_waitTime" allow to disable clicks on objects during a moment when user closes message box
    auto menuCtrl = static_cast<MenuController*>(SDMgetObject("MenuController"));
    if (menuCtrl->m_closeApplication) // quit game
    {
        if (m_msgAnswer == MsgAnswer::YES)
        {
            m_window.close();
            m_isRunning = false;
        }
        else
        {
            m_waitTime = 20;
            menuCtrl->m_closeApplication = false;
        }
    }
    else if (menuCtrl->m_pageName == MenuController::MenuPage::PAGE_MAIN_MENU)
    {
        if (m_optionIndex == MenuController::GameMenu::OP_START_GAME)
        {
            if (m_msgAnswer == MsgAnswer::YES)
            {
                m_gameSysExt.initProgress();
                m_gameSysExt.saveData(is::GameConfig::GAME_DATA_FILE);
                m_gameSysExt.m_launchOption = is::DisplayOption::GAME_LEVEL;
                m_isClosed = true;
            }
            else m_waitTime = 20;
        }
        else m_waitTime = 20;
    }
}

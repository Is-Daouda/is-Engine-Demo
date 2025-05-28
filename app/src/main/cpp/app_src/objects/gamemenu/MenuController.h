#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../widgets/CancelButton.h"
#include "../../language/GameLanguage.h"

class MenuController : public is::MainObject
{
public:
    bool m_closeApplication = false;
    enum MenuPage
    {
        PAGE_MAIN_MENU,
        PAGE_PAD_CONFIG
    };
    MenuPage m_pageName;

    enum GameMenu
    {
        OP_START_GAME,
        OP_CONTINUE,
        OP_CONFIG_PAD
    };
    enum GameOption
    {
        OP_MENU_SND,
        OP_MENU_MSC,
        OP_MENU_VIBRATE,
        OP_MENU_LANG
    };

    MenuController(is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void draw(is::Render &surface);

private:
    is::GameDisplay *m_scene;

    std::wstring getABPlace() const
    {
        std::wstring strAnd = L"and";
        if (m_scene->getGameSystem().m_gameLanguage == is::lang::GameLanguage::FRANCAIS) strAnd = L"et";
        return ((!m_scene->getGameSystem().m_permutePadAB) ? L"A " + strAnd + L" B" : L"B " + strAnd + L" A");
    }

    sf::Sprite m_sprPad1, m_sprPad2, m_sprPad3;
    sf::Sprite m_sprJoystick[2], m_sprPermuteAB, m_sprAddAlpha, m_sprReduceAlpha;

    sf::Text m_txtStartGame, m_txtContinue, m_txtPadConfig;
    sf::Text m_txtSetAlpha, m_txtOptionValue[5], m_txtPermuteAB, m_txtMovePad;

    sf::RectangleShape m_recLine, m_recSelectPad, m_recCfgBg;
    int m_gameOptionIndex;
    float m_sprButtonPemuteScale, m_timeClick;
    float m_txtOptionScale[6];
    bool m_isClicked, m_padCfgClicked;
    bool m_saveFileOpen;
};

#endif // MAINMENU_H_INCLUDED

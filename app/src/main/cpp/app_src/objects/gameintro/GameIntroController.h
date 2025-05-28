#ifndef GAMEINTROCONTROLLER_H_INCLUDED
#define GAMEINTROCONTROLLER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Step.h"
#include "../../../isEngine/system/graphic/TransitionEffect.h"

class GameIntroController : public is::MainObject, public is::Step
{
public:
    GameIntroController(is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void draw(is::Render &surface);

private:
    is::GameDisplay *m_scene;
    is::TransitionEffect m_transitionEffect;
    sf::Sprite m_sprLogo, m_sprLogoBg, m_sprPadFr, m_sprButtonSelect, m_sprHandGear[2], m_sprGear[2];
    sf::Text m_txtChooseLanguage, m_txtLangEng, m_txtLangFr;
    sf::RectangleShape m_recTransition, m_recChooseLanguage;

    int m_optionIndex;
    bool m_blind, m_isOnPad, m_openLanguageMenu, m_playSnd;
};

#endif // GAMEINTROCONTROLLER_H_INCLUDED

#ifndef GAMEINTROCONTROLLER_H_INCLUDED
#define GAMEINTROCONTROLLER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Step.h"

class GameIntroController : public is::MainObject, public is::Step
{
public:
    GameIntroController(sf::Font &font, sf::Texture &texLogo, sf::Texture &texPad, is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

private:
    is::GameDisplay *m_scene;
    sf::Sprite m_sprPadFr, m_sprHandGear[2], m_sprGear[2];
    sf::Text m_txtChooseLanguage, m_txtLangEng, m_txtLangFr;
    sf::RectangleShape m_recTransition, m_recChooseLanguage;

    int m_alphaRec;
    bool m_blind, m_isOnPad, m_openLanguageMenu, m_playSnd;
};

#endif // GAMEINTROCONTROLLER_H_INCLUDED

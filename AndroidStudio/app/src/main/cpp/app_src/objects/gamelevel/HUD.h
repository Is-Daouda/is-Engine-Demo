#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "../widgets/GameDialog.h"
#include "Player.h"

////////////////////////////////////////////////////////////
/// Displays game information (number of Bonuses, life, ...)
////////////////////////////////////////////////////////////
class HUD : public is::MainObject
{
public:
    HUD(is::GameDisplay *scene);

    void loadResources(sf::Font const &fnt);
    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

private:
    is::GameDisplay *m_scene;
    int m_score;
    float m_timeScoreCount;

    sf::Text m_txtHealth, m_txtBonus, m_txtLives, m_txtLevelTime, m_txtScore;
};

#endif // HUD_H_INCLUDED

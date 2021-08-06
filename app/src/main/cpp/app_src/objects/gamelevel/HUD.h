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
    void step(float const &DELTA_TIME);
    void draw(is::Render &surface);
    void setShowTime(bool val) {m_showTime = val;}

private:
    is::GameDisplay *m_scene;
    int m_score;
    float m_timeScoreCount;
    bool m_showTime;

    sf::Text m_txtWorld, m_txtBonus, m_txtLevelTime, m_txtScore;
};

#endif // HUD_H_INCLUDED

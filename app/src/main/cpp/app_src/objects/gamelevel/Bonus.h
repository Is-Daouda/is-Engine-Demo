#ifndef BONUS_H_INCLUDED
#define BONUS_H_INCLUDED

#include "../../../isEngine/system/entity/parents/Step.h"
#include "../../../isEngine/system/entity/parents/Type.h"
#include "../../../isEngine/system/display/GameDisplay.h"
#include "../widgets/GameDialog.h"
#include "Player.h"
#include "GroundDetection.h"
#include "Effect.h"

class Bonus : public is::MainObject, public is::Step, public is::ScorePoint, public GroundDetection, public is::Type
{
public:
    /// types of bonus
    enum BonusType
    {
        COIN,
        MUSHROOM,
        FIRE_FLOWER,
        STARMAN,
        MUSHROOM_1UP
    };

    Bonus(sf::Texture &tex, BonusType type, float x, float y, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);
    void jump()
    {
        if (static_cast<int>(m_timeJump) == 0) m_timeJump = 20.f;
    }

private:
    is::GameDisplay *m_scene;
    bool m_touchPlayer, m_starting;
    float m_timeJump;
};

#endif // BONUS_H_INCLUDED

#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/ScorePoint.h"
#include "GameController.h"

class Effect : public is::MainObject, public is::Type, is::ScorePoint
{
public:
    /// types of bonus
    enum EffectType
    {
        ONE_UP,
        SCORE_100,
        SCORE_200,
        SCORE_800,
        SCORE_1000,
        SCORE_2000,
        SCORE_5000,
        BLOCK_DEBRIS,
        COIN_FLY,
    };
    Effect(sf::Texture &tex, EffectType type, float x, float y, is::GameDisplay *scene, float speed = 0.f, float Hspeed = 0.f);
    void step(float const &DELTA_TIME);

private:
    is::GameDisplay *m_scene;
};

#endif // EFFECT_H_INCLUDED

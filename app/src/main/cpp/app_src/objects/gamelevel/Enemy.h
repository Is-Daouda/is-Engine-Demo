#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "../../../isEngine/system/entity/MainObject.h"
#include "../../../isEngine/system/entity/parents/Type.h"
#include "../widgets/GameDialog.h"
#include "Player.h"
#include "FireBall.h"
#include "GroundDetection.h"
#include "Effect.h"

class Enemy : public is::MainObject, public GroundDetection, public is::Type
{
public:
    enum EnemyType
    {
        LITTLE_GOOMBA,
        KOOPA_TROOPA,
        PIRANA_PLANT
    };

    Enemy(EnemyType type, float x, float y, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    //void draw(is::Render &surface);
    void setIsBeated(bool val) {m_isBeated = val;}
    bool getIsBeated() const {return m_isBeated;}

private:
    is::GameDisplay *m_scene;
    float m_awakeTime, m_timeCancelCollision, m_timeHurt;
    bool m_isBeated, m_isTouched, m_isKicked;
};

#endif // ENEMY_H_INCLUDED

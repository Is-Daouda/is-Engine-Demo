#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Visibilty.h"
#include "../../../isEngine/system/entity/parents/Type.h"

class Block : public is::MainObject, public is::Type
{
public:
    static float m_globalframe; // allows to standardize the animation of the blocks

    /// types of blocks
    enum BlockType
    {
        BLOCK_INVISIBLE,
        BLOCK_DESTRUCTIBLE,
        BLOCK_MUSHROOM,
        BLOCK_COIN,
        BLOCK_MULTI_COIN,
        BLOCK_STARMAN,
        BLOCK_MUSHROOM_1UP
    };

    Block(BlockType type, float x, float y, is::GameDisplay *scene, float frame = 0.f);

    void step(float const& DELTA_TIME);
    //void draw(is::Render &surface);
    void setCreateBonus(bool val) {m_createBonus = val;}
    void reduceCoins()
    {
        if (m_countCoins > 0) --m_countCoins;
        m_time = 10.f;
    }

    bool getCreateBonus() const   {return m_createBonus;}

private:
    is::GameDisplay *m_scene;
    float m_sprOrigin;
    bool m_textureExiste, m_createBonus;
    int m_countCoins;
};

#endif // BLOC_H_INCLUDED

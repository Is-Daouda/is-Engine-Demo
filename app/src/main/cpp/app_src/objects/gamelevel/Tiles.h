#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Type.h"

class Tiles : public is::MainObject, public is::Type
{
public:
    Tiles(int index, float x, float y, is::GameDisplay *scene):
        MainObject(x, y),
        Type(index)
{
    m_strName = "Tiles";

    // We create the sprite of the object.
    // On SDL this sprite will be blit (Useful for drawing a group of different objects whose
    // sprites have the same textures. This makes the game more optimized).
    scene->createSprite("tileset", *this, sf::IntRect(0, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(0.f, 0.f));
    is::setFrame(m_sprParent, index, 6, 32);

    m_depth = 10;
}
};

#endif // TILES_H_INCLUDED

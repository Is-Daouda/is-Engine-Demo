#include "Block.h"

float Block::m_globalframe = 0.f;

Block::Block(BlockType type, float x, float y, is::GameDisplay *scene, float frame):
    MainObject(x, y),
    Type(type),
    m_scene(scene),
    m_sprOrigin(16.f),
    m_textureExiste((m_type == BlockType::BLOCK_INVISIBLE) ? false : true),
    m_createBonus(false),
    m_countCoins(5) // Number of coins
{
    m_visible = (type != BLOCK_MUSHROOM_1UP), // If it's a Block that gives the mushroom we make it invisible at the start
    m_strName = "Block";
    m_isActive = true;
    m_frame = frame;
    if (m_textureExiste)
    {
        // We create the sprite of the object.
        // On SDL this sprite will be blit (Useful for drawing a group of different objects whose
        // sprites have the same textures. This makes the game more optimized).
        scene->createSprite("tileset", *this, sf::IntRect(0, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(0.f, 0.f));
    }

    // define collision mask
    setRectangleMask(32, 32);
}

void Block::step(float const& DELTA_TIME)
{
    if (m_isActive)
    {
        switch (m_type)
        {
            case BLOCK_MUSHROOM:
            case BLOCK_COIN:
            case BLOCK_MUSHROOM_1UP:
                if (!m_createBonus) m_frame = Block::m_globalframe; else m_frame = 41.f;
            break;

            case BLOCK_STARMAN:
                if (!m_createBonus) m_frame = 2.f; else m_frame = 41.f;
            break;

            case BLOCK_MULTI_COIN:
                if (m_countCoins == 0)
                {
                    m_frame = 41.f;
                    m_createBonus = true;
                }
                else
                {
                    m_frame = 2.f;
                    m_createBonus = false;
                }
                if (m_time > 0.f) m_time -= is::getMSecond(DELTA_TIME); else m_time = 0.f; // allow to avoid loop
            break;
        }

        if (m_yOffset < 0.f) m_yOffset += 2.f * is::VALUE_CONVERSION * DELTA_TIME;
        else m_yOffset = 0.f;

        // update sprite and collision mask
        is::setFrame(m_sprParent, m_frame, 6, 32);
        updateSprite();
        updateCollisionMask();
    }
}

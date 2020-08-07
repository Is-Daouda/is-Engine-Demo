#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"

class Block : public is::MainObject
{
public:
    /// types of blocks
    enum BlockType
    {
        BLOCK_NORMAL,
        BLOCK_TRANSPARENT,
        BLOCK_MOVE_HORIZ,
        BLOCK_MOVE_VERTI
    };

    Block(sf::Texture *tex, BlockType type, float x, float y);

    void step(float const& DELTA_TIME);
    void setMoveHorizontal(bool val);
    void setMoveVertical(bool val);
    void setChangeDir(bool val);
    void draw(sf::RenderTexture &surface);

    bool getChangeDir() const;
    bool getMoveHorizontal() const;
    bool getMoveVertical() const;
    BlockType getType() const;

private:
    float m_sprOrigin;
    BlockType m_type;
    bool m_textureExiste, m_changeDir, m_moveHorizontal, m_moveVertical;
};

#endif // BLOC_H_INCLUDED

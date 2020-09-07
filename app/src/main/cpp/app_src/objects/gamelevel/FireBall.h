#ifndef FIREBALL_H_INCLUDED
#define FIREBALL_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "GameController.h"

////////////////////////////////////////////////////////////
/// Represents the player's weapon in the game
////////////////////////////////////////////////////////////
class FireBall : public is::MainObject
{
public:
    /// Return the instance number
    static int instanceNumber;

    FireBall(sf::Texture &tex, float x, float y, float hSpeed, is::GameDisplay *scene);
    ~FireBall() {instanceNumber--;}

    void step(float const &DELTA_TIME);
    void createExplosion()    {m_createExplosion = true;}

    bool getCreateExplosion() {return m_createExplosion;}

private:
    is::GameDisplay *m_scene;
    bool m_createExplosion;
};

#endif // FIREBALL_H_INCLUDED

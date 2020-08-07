#ifndef ELECTROSHOCK_H_INCLUDED
#define ELECTROSHOCK_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "GameController.h"

////////////////////////////////////////////////////////////
/// Represents the player's weapon in the game
////////////////////////////////////////////////////////////
class ElectroShock : public is::MainObject
{
public:
    ElectroShock(float x, float y, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

private:
    is::GameDisplay *m_scene;
    sf::CircleShape m_circleElectro;
};

#endif // ELECTROSHOCK_H_INCLUDED

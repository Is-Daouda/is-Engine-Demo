#ifndef SPECIALOBJECT_H_INCLUDED
#define SPECIALOBJECT_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Type.h"
#include "Player.h"
#include "GameController.h"

////////////////////////////////////////////////////////////
/// Allows you to perform special actions in the levels:
/// - Control the Camera (Here prevent it from moving)
/// - Manage the passage in the tunnels
/// - ...
////////////////////////////////////////////////////////////
class SpecialObject : public is::MainObject, is::Type
{
public:
    enum SpecialObjectType
    {
        VIEW_CONTROLLER,
        ENTER_TUNNEL_A,
        TUNNEL_EXIT_A,
        ENTER_TUNNEL_B,
        TUNNEL_EXIT_B,
    };

    SpecialObject(SpecialObjectType type, float x, float y, is::GameDisplay *scene);
    void step(float const &DELTA_TIME);

private:
    is::GameDisplay *m_scene;
};

#endif // SPECIALOBJECT_H_INCLUDED

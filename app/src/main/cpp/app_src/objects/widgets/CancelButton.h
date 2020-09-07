#ifndef CANCELBUTTON_H_INCLUDED
#define CANCELBUTTON_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/parents/Visibilty.h"

class CancelButton : public is::MainObject, public is::Visibility
{
public:
    CancelButton(sf::Texture &tex, is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);
    bool isOn() const;

private:
    is::GameDisplay *m_scene;
};

#endif // CANCELBUTTON_H_INCLUDED

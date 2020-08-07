#ifndef LEVELTITLE_H_INCLUDED
#define LEVELTITLE_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../widgets/GameDialog.h"
#include "GameController.h"

class LevelTitle : public is::MainObject
{
public:
    LevelTitle(sf::Font &fnt, float x, float y, std::string levelTitle, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

private:
    is::GameDisplay *m_scene;
    sf::Text m_txtLevelTitle;
};

#endif // LEVELTITLE_H_INCLUDED

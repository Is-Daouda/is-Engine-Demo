#include "LevelTitle.h"

LevelTitle::LevelTitle(sf::Font &fnt, float x, float y, std::string levelTitle, is::GameDisplay *scene):
    MainObject(x, y),
    m_scene(scene)
{
    is::createText(fnt, m_txtLevelTitle, levelTitle, x, y, sf::Color::Black, true, 32);
}

void LevelTitle::step(float const &DELTA_TIME)
{
    if (static_cast<GameController*>(m_scene->SDMgetObject("GameController"))->canActivate())
    {
        if (m_imageAlpha > 12) m_imageAlpha -= static_cast<int>((5.f * is::VALUE_CONVERSION) * DELTA_TIME);
        else m_destroy = true;
        is::setSFMLObjAlpha2(m_txtLevelTitle, m_imageAlpha);
    }
}

void LevelTitle::draw(sf::RenderTexture &surface)
{
    surface.draw(m_txtLevelTitle);
}

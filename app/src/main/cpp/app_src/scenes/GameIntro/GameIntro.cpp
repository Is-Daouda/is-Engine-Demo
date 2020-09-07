#include "GameIntro.h"

GameIntro::GameIntro(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, is::GameSystemExtended &gameSysExt):
    GameDisplay(window, view, surface, gameSysExt, sf::Color::White) {}

void GameIntro::loadResources()
{
    GameDisplay::loadParentResources();

    m_gameSysExt.loadConfig(is::GameConfig::CONFIG_FILE);

    // load textures
    is::loadSFMLObjResource(m_texLogo, is::GameConfig::SPRITES_DIR + "logo.png");
    is::loadSFMLObjResource(m_texPad, is::GameConfig::GUI_DIR + "main_menu_pad.png");
    is::loadSFMLObjResource(m_fontTitle, is::GameConfig::FONT_DIR + "space_ranger_halftone_italic_qz_30.otf");

    // load sound
    GSMaddSound("logo_sound", is::GameConfig::SFX_DIR + "logo_sound.ogg");

    SDMaddSceneObject(std::make_shared<GameIntroController>(m_fontTitle, m_texLogo, m_texPad, this));
}

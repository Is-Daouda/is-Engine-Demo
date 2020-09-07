#include "GameOver.h"

GameOver::GameOver(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, is::GameSystemExtended &gameSystemExtended):
    GameDisplay(window, view, surface, gameSystemExtended, sf::Color::Black) {}

void GameOver::loadResources()
{
    m_gameSysExt.loadConfig(is::GameConfig::CONFIG_FILE);

    GameDisplay::loadParentResources();

    // load resources
    is::loadSFMLObjResource(m_texPad, is::GameConfig::GUI_DIR + "option_pad.png");
    is::loadSFMLObjResource(m_fontTitle, is::GameConfig::FONT_DIR + "space_ranger_halftone_italic_qz_30.otf");

    // load music
    GSMaddMusic("game_over", is::GameConfig::MUSIC_DIR + "game_over.ogg");

    SDMaddSceneObject(std::make_shared<GameOverController>(m_fontTitle, m_texPad, this));
}

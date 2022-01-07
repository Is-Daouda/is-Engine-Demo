#include "GameOver.h"

GameOver::GameOver(is::GameSystemExtended &gameSystemExtended):
    GameDisplay(gameSystemExtended, sf::Color::Black) {}

void GameOver::loadResources()
{
    m_gameSysExt.loadConfig(is::GameConfig::CONFIG_FILE);

    GameDisplay::loadParentResources();

    // load resources
    GRMaddTexture("option_pad", is::GameConfig::GUI_DIR + "option_pad.png");

    // load music
    GSMaddMusic("game_over", is::GameConfig::MUSIC_DIR + "game_over.ogg");

    SDMaddSceneObject(std::make_shared<GameOverController>(this));
}

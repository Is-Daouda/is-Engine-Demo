#include "GameIntro.h"

GameIntro::GameIntro(is::GameSystemExtended &gameSysExt):
    GameDisplay(gameSysExt, sf::Color::White) {}

void GameIntro::loadResources()
{
    GameDisplay::loadParentResources();
    m_gameSysExt.loadConfig(is::GameConfig::CONFIG_FILE);

    // load textures
    GRMaddTexture("logo", is::GameConfig::GUI_DIR + "logo.png");
    GRMaddTexture("logo_bg", is::GameConfig::GUI_DIR + "logo_bg.png");
    GRMaddTexture("language_pad", is::GameConfig::GUI_DIR + "language_pad.png");

    // load sound
    GSMaddSound("logo_sound", is::GameConfig::SFX_DIR + "logo_sound.wav");

    SDMaddSceneObject(std::make_shared<GameIntroController>(this));
}

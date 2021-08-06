#include "GameLevel.h"

void GameLevel::loadResources()
{
    GameDisplay::loadParentResources();

    GRMaddTexture("tools_pad", is::GameConfig::GUI_DIR + "tools_pad.png");
    GRMaddTexture("ico_button", is::GameConfig::GUI_DIR + "ico_button.png");

    auto gameKey = std::make_shared<is::GameKeyData>(this);
    gameKey->loadResources();
    gameKey->setDepth(-9);

    // We add the object to the SDM container but we order it to display it only
    // We also give it a name that will allow us to identify it in the container
    // It will be updated in the SDMmanageSceneEvents() method of GameLevel Class
    SDMaddSceneObject(gameKey, false, true, "GameKeyData");

    // load sounds with GSM
    GSMaddSound("1_up", is::GameConfig::SFX_DIR + "1_up.wav");
    GSMaddSound("break_block", is::GameConfig::SFX_DIR + "break_block.wav");
    GSMaddSound("bump", is::GameConfig::SFX_DIR + "bump.wav");
    GSMaddSound("coin", is::GameConfig::SFX_DIR + "coin.wav");
    GSMaddSound("fireball", is::GameConfig::SFX_DIR + "fireball.wav");
    GSMaddSound("flagpole", is::GameConfig::SFX_DIR + "flagpole.wav");
    GSMaddSound("jump_small", is::GameConfig::SFX_DIR + "jump_small.wav");
    GSMaddSound("jump_super", is::GameConfig::SFX_DIR + "jump_super.wav");
    GSMaddSound("kick", is::GameConfig::SFX_DIR + "kick.wav");
    GSMaddSound("mario_die", is::GameConfig::SFX_DIR + "mario_die.wav");
    GSMaddSound("pause", is::GameConfig::SFX_DIR + "pause.wav");
    GSMaddSound("pipe", is::GameConfig::SFX_DIR + "pipe.wav");
    GSMaddSound("powerup", is::GameConfig::SFX_DIR + "powerup.wav");
    GSMaddSound("powerup_appears", is::GameConfig::SFX_DIR + "powerup_appears.wav");
    GSMaddSound("stomp", is::GameConfig::SFX_DIR + "stomp.wav");
    GSMaddSound("warning", is::GameConfig::SFX_DIR + "warning.wav");
    GSMaddSound("stage_clear", is::GameConfig::SFX_DIR + "stage_clear.wav");
    GSMaddSound("score_count", is::GameConfig::SFX_DIR + "score_count.wav");
    GSMsetSoundLoop("score_count", true);

    // GUI resources
    auto &texPad = GRMaddTexture("option_pad", is::GameConfig::GUI_DIR + "option_pad.png");
    auto &texDialog = GRMaddTexture("dialog_box", is::GameConfig::GUI_DIR + "dialog_box.png");
    is::createSprite(texPad, m_sprButtonSelect, sf::IntRect(160, 0, 160, 32), sf::Vector2f(0.f, 0.f) , sf::Vector2f(80.f, 16.f));

    // tiles
    GRMaddTexture("tileset", is::GameConfig::TILES_DIR + "tileset.png");

    // sprites
    GRMaddTexture("player", is::GameConfig::SPRITES_DIR + "player.png");
    GRMaddTexture("bonus",  is::GameConfig::SPRITES_DIR + "bonus.png");
    GRMaddTexture("enemy",  is::GameConfig::SPRITES_DIR + "enemy.png");
    GRMaddTexture("fire",   is::GameConfig::SPRITES_DIR + "fire.png");
    GRMaddTexture("effect", is::GameConfig::SPRITES_DIR + "effect.png");

    // creation of the object which manages the level
    // but avoid the SDM drawing it
    auto gameCtrl = std::make_shared<GameController>(CURRENT_LEVEL, this);
    SDMaddSceneObject(gameCtrl, true, false);

    auto gameDialog = std::make_shared<is::GameDialog>(texDialog, getFontSystem(), this);
    gameDialog->setDepth(-8);

    // We add the GameDialog object in the SDM container but the SDM will not update it.
    // The GameController object will take care of updating it in its step()
    SDMaddSceneObject(gameDialog, false, true);

    auto player = std::make_shared<Player>(gameCtrl->m_timeUp, this);
    SDMaddSceneObject(player); // We add the object in the SDM container

    auto gameHud = std::make_shared<HUD>(this);
    SDMaddSceneObject(gameHud); // We add the object in the SDM container

    SDMaddSceneObject(std::make_shared<CancelButton>(this)); // We add the object in the SDM container

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      LEVEL INTERPRETER
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // chose the corresponding level map array
    short const *mapIndex = is::level::getLevelMap(CURRENT_LEVEL);
    if (mapIndex == 0) is::showLog("Level Map not found !", true);

    // allow to browse the map array
    int dataIndex(0);

    // variable to position the objects in level
    float i(0.f), j(0.f);

    // allow to build tile map
    std::vector<short> backTileValue;

    // this variables allow to store tile id
    // signification of 999 = empty tile
    short backTileNumber(999);

    /*
     * How does the level interpreter work?
     * ------------------------------------------------------------------------------------------
     * It works like the way you write in a book i.e you write from right to left and when you reach
     * the end you come back to the line and so on. Except that here writing is replaced by the
     * creation or positioning of level objects.
     *
     * Here's how the mechanism works?
     * -----------------------------------------------------------------------------------------
     * i, j represents the position of the virtual cursor (represents the hand that writes).
     * For each value of mapIndex[dataIndex] we execute a specific instruction(s)
     * The value 1000 represents an empty case (space),
     * the 1001 allows to move the cursor to the right,
     * the 1002 puts it back on the line,
     * 1003 represents the end of the level and the other values allows to manipulate the
     * objects of the levels.
     */
    do
    {
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      BLOCK
//////////////////////////////////////////////////////////////////////////////////////////////////////
        if (mapIndex[dataIndex] == 0) // block mask
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_INVISIBLE, 32.f * j, 32.f * i, this), false, true);
        else if (mapIndex[dataIndex] == 1) // block world 1-1
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_DESTRUCTIBLE, 32.f * j, 32.f * i, this, 2.f), false, true);
        else if (mapIndex[dataIndex] == 2) // block underground
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_DESTRUCTIBLE, 32.f * j, 32.f * i, this, 3.f), false, true);
        else if (mapIndex[dataIndex] == 3) // block coin
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_COIN, 32.f * j, 32.f * i, this), false, true);
        else if (mapIndex[dataIndex] == 4) // block with multiple coins
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_MULTI_COIN, 32.f * j, 32.f * i, this), false, true);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      BONUS
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] == 100) // Coin
            SDMaddSceneObject(std::make_shared<Bonus>(Bonus::BonusType::COIN, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 101) // Mushroom / Fire Flower
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_MUSHROOM, 32.f * j, 32.f * i, this), false, true);
        else if (mapIndex[dataIndex] == 102) // Life
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_MUSHROOM_1UP, 32.f * j, 32.f * i, this), false, true);
        else if (mapIndex[dataIndex] == 103) // Starman
            SDMaddSceneObject(std::make_shared<Block>(Block::BlockType::BLOCK_STARMAN, 32.f * j, 32.f * i, this), false, true);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      ENEMY
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] == 200)
            SDMaddSceneObject(std::make_shared<Enemy>(Enemy::EnemyType::LITTLE_GOOMBA, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 201)
            SDMaddSceneObject(std::make_shared<Enemy>(Enemy::EnemyType::KOOPA_TROOPA, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 202)
            SDMaddSceneObject(std::make_shared<Enemy>(Enemy::EnemyType::PIRANA_PLANT, 32.f * j, 32.f * i, this));

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      PLAYER
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] == 300) // player
        {
            if (!m_gameSysExt.m_checkPoint) player->setPosition(32.f * j, (32.f * i) - 16.f);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      GAMEPLAY OBJECT
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] == 400) // limit the movement of the view
            SDMaddSceneObject(std::make_shared<SpecialObject>(SpecialObject::VIEW_CONTROLLER, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 401) // finish level
            SDMaddSceneObject(std::make_shared<FinishObject>(32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 402) // the entrance of the first tunnel
            SDMaddSceneObject(std::make_shared<SpecialObject>(SpecialObject::ENTER_TUNNEL_A, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 403) // exit of the first tunnel
            SDMaddSceneObject(std::make_shared<SpecialObject>(SpecialObject::TUNNEL_EXIT_A, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 404) // the entrance of the second tunnel
            SDMaddSceneObject(std::make_shared<SpecialObject>(SpecialObject::ENTER_TUNNEL_B, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 405) // exit of the second tunnel
            SDMaddSceneObject(std::make_shared<SpecialObject>(SpecialObject::TUNNEL_EXIT_B, 32.f * j, 32.f * i, this));
        else if (mapIndex[dataIndex] == 406) // check point
        {
            // use the position of the game controller as the position of the checkpoint
            gameCtrl->setPosition(32.f * j, 32.f * i);
            if (m_gameSysExt.m_checkPoint) player->setPosition(gameCtrl->getX(), gameCtrl->getY() - 16.f);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      TILES
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] >= 500 && mapIndex[dataIndex] < 700)
        {
            backTileNumber = (mapIndex[dataIndex] - 500);
        }
        else if (mapIndex[dataIndex] == 1000) // empty tile (empty space)
        {
            // empty tile
            backTileNumber = 999;
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CURSOR MOVEMENT
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (mapIndex[dataIndex] == 1001) // move the cursor
        {
            // filling the tile number array
            backTileValue.push_back(backTileNumber);

            // empty tile
            backTileNumber = 999;
            j++;
        }
        else if (mapIndex[dataIndex] == 1002) // back to the line of cursor
        {
            if (m_sceneWidth == 0) m_sceneWidth = static_cast<unsigned int>(j); // define the level width
            j = 0.f;
            i++;
            backTileNumber = 999; // empty tile
            m_sceneHeight = static_cast<unsigned int>(i); // define the level height in pixel
        }
        else // error
        {
            is::showLog("\nUnknown value  : " + is::numToStr(mapIndex[dataIndex]) + "\n");
            ///std::terminate();
        }

        // allow to now the line of cursor
        dataIndex++;
    }
    while (mapIndex[dataIndex] != 1003); // read values in array (1003 = end of tile map)

    // create the tile map with vector array
    for (unsigned int i = 0; i < m_sceneWidth; ++i)
    {
        for (unsigned int j = 0; j < m_sceneHeight; ++j)
        {
            // we get the current tile number
            if (backTileValue[i + j * m_sceneWidth] != 999) // is not an empty space
            {
                SDMaddSceneObject(std::make_shared<Tiles>(backTileValue[i + j * m_sceneWidth], 32.f * i, 32.f * j, this), false, true);
            }
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CREATION OF THE LEVEL
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // set view position in relation to player
    player->setStartPosition(player->getX(), player->getY());
    m_viewX = player->getX();
    m_viewY = 0.f;

    if (m_viewX < m_viewW / 2) m_viewX = m_viewW / 2;
    if (m_viewX > static_cast<float>(m_sceneWidth) * 32.f - m_viewW / 2)
        m_viewX = static_cast<float>(m_sceneWidth) * 32.f - m_viewW / 2;

    if (m_viewY < m_viewH / 2) m_viewY = m_viewH / 2;
    if (m_viewY > static_cast<float>(m_sceneHeight) * 32.f - m_viewH / 2)
        m_viewY = static_cast<float>(m_sceneHeight) * 32.f - m_viewH / 2;

    // We add the object in the SDM container, but avoid SDM updating it
    SDMaddSceneObject(std::make_shared<PauseOption>(this), false);

    // set HUD position
    gameHud->setPosition(m_viewX, m_viewY);

    // load level music
    GSMaddMusic("world_1", is::GameConfig::MUSIC_DIR + "world_1.ogg");
    GSMsetMusicLoop("world_1", true);
    GSMaddMusic("world_1_hurry_up", is::GameConfig::MUSIC_DIR + "world_1_hurry_up.ogg");
    GSMsetMusicLoop("world_1_hurry_up", true);
    GSMaddMusic("underground", is::GameConfig::MUSIC_DIR + "underground.ogg");
    GSMsetMusicLoop("underground", true);
    GSMaddMusic("starman", is::GameConfig::MUSIC_DIR + "starman.ogg");

    // Add Start Transition object
    SDMaddSceneObject(std::make_shared<StartTransition>(this));

    // level information
    is::showLog("\nLevel           : " + is::numToStr(CURRENT_LEVEL + 1) +
                "\nInstance Number : " + is::numToStr(is::MainObject::instanceNumber) +
                "\nLevel Width     : " + is::numToStr(m_sceneWidth  * 32) + " (" + is::numToStr(m_sceneWidth) + ")" +
                "\nLevel Height    : " + is::numToStr(m_sceneHeight * 32) + " (" +is::numToStr(m_sceneHeight) + ")" +
                "\nLevel Time      : " + is::numToStr(gameCtrl->m_gameTime.getMinute()) + "min : "
                                       + is::numToStr(gameCtrl->m_gameTime.getSecond()) + "s" +
                "\n");
}

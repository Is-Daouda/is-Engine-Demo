#include "GameController.h"

GameController::GameController(int const &CURRENT_LEVEL, is::GameDisplay *scene):
    m_timeUp(false),
    m_restartTime(0),
    m_scene(scene),
    CURRENT_LEVEL(CURRENT_LEVEL),
    m_countLevelTime(true),
    m_timeCancelCollision(0)
{
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SET LEVEL TIME
//////////////////////////////////////////////////////////////////////////////////////////////////////
    m_gameTime.setTimeValue(2, 59, 59);
}

void GameController::step(float const &DELTA_TIME)
{
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SPECIAL STEP
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // We call the player object which is in the SDM container
    auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));

    // if the player is KO we stop the level
    if (player->getIsKO())
    {
        m_scene->getGameSystem().m_launchOption = is::DisplayOption::RESTART_LEVEL; // restart level
        m_scene->setSceneEnd(true);
    }

    // when player finish the level
    if (!m_scene->getSceneEnd())
    {
        if (auto finishObject = static_cast<FinishObject*>(m_scene->SDMgetObject("FinishObject")); finishObject->getStep() == 1)
        {
            //////////////////////////////////////////////////////////////////////
            m_scene->getGameSystem().m_currentLevel++; // allow to access the next level

            // increase game progress
            if (m_scene->getGameSystem().m_currentLevel > m_scene->getGameSystem().m_gameProgression && m_scene->getGameSystem().m_gameProgression < m_scene->getGameSystem().m_levelNumber)
            {
                m_scene->getGameSystem().m_gameProgression++;
            }
            m_scene->getGameSystem().saveData(is::GameConfig::GAME_DATA_FILE); // save data
            //////////////////////////////////////////////////////////////////////

            m_scene->getGameSystem().m_launchOption = is::DisplayOption::NEXT_LEVEL; // go to the next level
            m_scene->setSceneEnd(true);
            finishObject->addStep();
        }
    }

    // allows to show dialog box to help user
    levelSpecialAction();

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      VIEW
//////////////////////////////////////////////////////////////////////////////////////////////////////
    // move the view if player is active
    if (!player->getIsKO())
    {
        if (player->getX() > m_scene->getViewW() / 2.f && player->getX() < static_cast<float>(m_scene->getSceneWidth() * 32) - m_scene->getViewW() / 2.f)
            m_scene->setViewX(player->getX());

        if (player->getY() > static_cast<float>(m_scene->getSceneHeight() * 32) - m_scene->getViewH() / 2.f)
            m_scene->setViewY(static_cast<float>(m_scene->getSceneHeight() * 32) - m_scene->getViewH() / 2.f);
        else if (player->getY() > m_scene->getViewH() / 2.f && player->getY() < static_cast<float>(m_scene->getSceneHeight() * 32) - m_scene->getViewH() / 2.f)
            m_scene->setViewY(player->getY());
        else if (player->getY() < m_scene->getViewH() / 2.f) m_scene->setViewY(m_scene->getViewH() / 2.f);
    }

    // update scene view
    m_scene->setView();

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      LEVEL CHRONOMETER
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_countLevelTime)
    {
        if (m_gameTime.getTimeValue() != 0)
        {
            // level global time
            m_scene->getGameSystem().m_levelTime += is::getMSecond(DELTA_TIME);
            m_gameTime.step(DELTA_TIME, is::VALUE_CONVERSION, is::VALUE_TIME);
        }
        else m_timeUp = true;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      IN GAME
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_scene->getIsPlaying())
    {
        // check if mouse is in collision with sprite
        auto cancelBt = static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"));
        if (m_scene->mouseCollision(cancelBt->getSprite()))
        {
            cancelBt->setAlpha(255);
            m_scene->getGameSystem().m_keyIsPressed = true;
        }
        pauseGame();

        // Apply a 3D effect on the background movement
        m_scene->SDMgetObject("Background")->setX(m_scene->getViewX() * 0.98f - 390.f);

        // block update and collision test
        blocCollision(DELTA_TIME);

        auto gameDialog = static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"));
        gameDialog->step(DELTA_TIME);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      MENU PAUSE
//////////////////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        if (m_scene->getSceneStart() && !m_scene->getSceneEnd())
        {
            auto gameKey = static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"));

            // mini option pause
            auto pauseOption = static_cast<PauseOption*>(m_scene->SDMgetObject("PauseOption"));
            pauseOption->step(DELTA_TIME);

            // function to resume play and replay paused sounds
            auto continueGame = [this, gameKey]()
            {
                stopSounds(false);
                m_scene->GSMplaySound("cancel"); // We play this sound
                static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"))->setAlpha(0);
                m_scene->setIsPlaying(true);
                m_scene->getGameSystem().m_keyIsPressed = true;
                gameKey->m_keyPausePressed = true;
            };

            // if back key is pressed continue the game
            if (m_scene->getKeyBackPressed() && !gameKey->m_keyPausePressed)
            {
                continueGame();
                m_scene->setKeyBackPressed(false);
            }

            // validate option
            if (((m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::KEYBOARD) && !gameKey->m_keyPausePressed) ||
                (m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE) && pauseOption->m_mouseInCollison)) &&
                m_scene->getWaitTime() == 0 && !m_scene->getGameSystem().m_keyIsPressed && !m_scene->getSceneEnd())
            {
                switch (m_scene->getOptionIndex())
                {
                    case is::DisplayOption::RESUME_GAME:
                        continueGame();
                    break;

                    case is::DisplayOption::GAME_OPTION_RESTART:
                        m_scene->showMessageBox(is::lang::msg_pause_restart[m_scene->getGameSystem().m_gameLanguage], true);
                    break;

                    case is::DisplayOption::QUIT_GAME:
                        m_scene->showMessageBox(is::lang::msg_pause_quit[m_scene->getGameSystem().m_gameLanguage], true);
                    break;
                }
            }
        }
    }
}

void GameController::blocCollision(float const &DELTA_TIME)
{
    // We call the object which is in the SDM container
    auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));

    short const yPrecision(6);
    short const xPrecision(10);
    bool rightCollision(true);
    bool cancelRightCollision(false);
    bool canFollowBloc(true);

    // If this counter is greater than 0, it cancels all the collisions of the player at
    // the top in order to make the contacts with the blocks softer
    if (m_timeCancelCollision > 0.f) m_timeCancelCollision -= is::getMSecond(DELTA_TIME);
    else m_timeCancelCollision = 0.f;

    // this variables allow to store block temporally
    Block* blockGroundId = nullptr;
    Block* blockId = nullptr;

    // block loop
    for (std::list<std::shared_ptr<is::MainObject>>::iterator it = m_scene->m_SDMsceneObjects.begin();
        it != m_scene->m_SDMsceneObjects.end(); ++it)
    {
        if (is::instanceExist(*it))
        {
            // This allows to know if the object browsed is a Block
            if ((*it)->getName() == "Block")
            {
                auto currentBlock = static_cast<Block*>(it->get());

                if (currentBlock->getIsActive())
                {
                    // moving blocks
                    if (currentBlock->getType() == Block::BlockType::BLOCK_MOVE_HORIZ ||
                        currentBlock->getType() == Block::BlockType::BLOCK_MOVE_VERTI)
                    {
                        // We check if the moving block touches the Limiter. to change its trajectory
                        for (std::list<std::shared_ptr<is::MainObject>>::iterator _it = m_scene->m_SDMsceneObjects.begin();
                            _it != m_scene->m_SDMsceneObjects.end(); ++_it)
                        {
                            if (is::instanceExist((*_it)))
                            {
                                // This allows to know if the object browsed is a Limiter
                                if ((*_it)->getName() == "Limiter")
                                {
                                    if (currentBlock->placeMetting(0 , 0, (*_it)))
                                    {
                                        if (!currentBlock->getChangeDir())
                                        {
                                            currentBlock->setSpeed(1.f);
                                            currentBlock->setChangeDir(true);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // check collision at bottom
                    if (static_cast<int>(m_timeCancelCollision) == 0)
                    {
                        if (player->getY() + static_cast<float>(player->getMaskH()) < currentBlock->getY() + (static_cast<float>(yPrecision) + 4.f) &&
                            player->getY() + static_cast<float>(player->getMaskH()) > currentBlock->getY() - 2.f)
                        {
                            if (player->placeMetting(0, yPrecision + 10, currentBlock) && player->getVspAcc() > -0.99f)
                            {
                                blockGroundId = currentBlock; // store the block to use it after
                            }
                        }
                    }

                    // execute this instruction only for this block
                    if (currentBlock->getType() == Block::BlockType::BLOCK_NORMAL)
                    {
                        // check collision at right
                        if (player->placeMetting(xPrecision, 0, currentBlock))
                        {
                            if (player->getHsp() > 0.f)
                            {
                                blockId = currentBlock;
                                rightCollision = true;
                            }
                            if (is::instanceExist(blockGroundId))
                            {
                                if (blockGroundId->getMoveHorizontal() && std::abs(blockGroundId->getSpeed()) > 0.f) canFollowBloc = false;
                            }
                        }
                        else if (player->placeMetting(-xPrecision, 0, currentBlock)) // check collision at left
                        {
                            if (player->getHsp() < 0.f)
                            {
                                blockId = currentBlock;
                                rightCollision = false;
                            }
                            if (is::instanceExist(blockGroundId))
                            {
                                if (blockGroundId->getMoveHorizontal() && std::abs(blockGroundId->getSpeed()) > 0.f) canFollowBloc = false;
                            }
                        }
                        else if (player->placeMetting(0, -yPrecision, currentBlock) && player->getVspAcc() < 0.f) // check top collision
                        {
                            player->setY(currentBlock->getY() + 5.f + static_cast<float>(currentBlock->getMaskH()));
                            player->setIsJumping(false);
                        }
                    }
                }
                currentBlock->step(DELTA_TIME);
            }
        }
    }

    // apply collision on the player
    if (player->getIsActive())
    {
        // check if it has collision
        if (is::instanceExist(blockGroundId) && static_cast<int>(m_timeCancelCollision) == 0) player->setOnGround(true);
        else player->setOnGround(false);

        // cancel horizontal collision if this condition is true
        if (cancelRightCollision) blockId = nullptr;

        // This prevents the player from moving when he touches a block on the left or right side
        player->setCanMove(blockId == nullptr);

        // is on ground
        if (player->getOnGround())
        {
            // if player touch the ground after jumping
            if (player->getVspAcc() > 0.f)
            {
                player->setVspAcc(0.f);
            }

            // if player is on horizontal moving bloc
            if (canFollowBloc)
            {
                if (blockGroundId->getMoveHorizontal())
                {
                    player->setX(player->getX() + (blockGroundId->getSpeed() * is::VALUE_CONVERSION) * DELTA_TIME);
                }
                if (blockGroundId->getMoveVertical())
                {
                    player->setY(player->getY() + (blockGroundId->getSpeed() * is::VALUE_CONVERSION) * DELTA_TIME);
                }
            }

            // if player is on vertical moving bloc
            if (is::instanceExist(blockGroundId))
            {
                player->setY(blockGroundId->getY() - static_cast<float>(player->getMaskH()));
            }
        }

        // horizontal collision with blocs
        if (is::instanceExist(blockId))
        {
            if (!rightCollision) // put player to the right of the block
            {
                player->setX(blockId->getX() + static_cast<float>(blockId->getMaskW()));
                player->setHsp(0.f);
            }
            else  // put player to the left of the block
            {
                player->setX(blockId->getX() - static_cast<float>(player->getMaskW()));
                player->setHsp(0.f);
            }
        }
    }
}

void GameController::pauseGame()
{
    // We call the object which is in the SDM container
    auto gameKey = static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"));
    auto finishObject = static_cast<FinishObject*>(m_scene->SDMgetObject("FinishObject"));
    auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));
    auto cancelBt = static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"));

    if (((((m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::KEYBOARD) && !m_scene->getGameSystem().m_keyIsPressed) ||
           m_scene->getKeyBackPressed()) && !gameKey->m_keyPausePressed) ||
         (m_scene->getGameSystem().isPressed(is::GameSystem::ValidationButton::MOUSE) && cancelBt->isOn())) &&
        (m_scene->getSceneStart() && !m_scene->getSceneEnd() && finishObject->getStep() == 0 && player->getHealth() != 0))
    {
        m_scene->GSMplaySound("select_option"); // We play this sound
        if (!m_scene->getKeyBackPressed()) m_scene->getGameSystem().useVibrate(40);
        stopSounds(true);
        m_scene->setIsPlaying(false);
        gameKey->m_keyPausePressed = true;
        m_scene->getGameSystem().m_keyIsPressed = true;
        m_scene->setKeyBackPressed(false);
    }
}

void GameController::levelSpecialAction()
{
    // We call the object which is in the SDM container
    auto gameDialog = static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"));
    if (CURRENT_LEVEL == is::level::LevelId::LEVEL_1)
    {
        if (gameDialog->getDialogIndex() == is::GameDialog::DIALOG_NONE)
        {
            // the dialog is displayed after 1 seconds compared to the time of the level when the scene start
            if (m_gameTime.compareTime(2, 59)) gameDialog->setDialog(is::GameDialog::DIALOG_PLAYER_MOVE);
        }
    }
    if (CURRENT_LEVEL == is::level::LevelId::LEVEL_3)
    {
        if (gameDialog->getDialogIndex() == is::GameDialog::DIALOG_NONE)
        {
            // the dialog is displayed after 1 seconds compared to the time of the level when the scene start
            if (m_gameTime.compareTime(2, 59)) gameDialog->setDialog(is::GameDialog::DIALOG_PLAYER_ATTACK);
        }
    }
}

void GameController::stopSounds(bool val)
{
    if (m_scene->getGameSystem().m_enableSound)
    {
        if (val)
        {
            m_scene->GSMpauseSound("lose");
            m_scene->GSMpauseSound("finish");
        }
        else
        {
            if (is::getSFMLSndState(m_scene->GSMgetSound("lose"), sf::Sound::Paused))   m_scene->GSMpauseSound("lose");
            if (is::getSFMLSndState(m_scene->GSMgetSound("finish"), sf::Sound::Paused)) m_scene->GSMpauseSound("finish");
        }
        static_cast<Player*>(m_scene->SDMgetObject("Player"))->stopAllSounds(val);
    }
    if (m_scene->getGameSystem().m_enableMusic)
    {
        if (val)
        {
            if (is::getSFMLSndState(m_scene->GSMgetMusic("level_music"), sf::Sound::Playing)) m_scene->GSMgetMusic("level_music")->pause();
        }
        else
        {
            if (is::getSFMLSndState(m_scene->GSMgetMusic("level_music"), sf::Sound::Paused)) m_scene->GSMgetMusic("level_music")->play();
        }
    }
}

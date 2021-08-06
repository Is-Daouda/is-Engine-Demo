#include "GameController.h"

GameController::GameController(int const &CURRENT_LEVEL, is::GameDisplay *scene):
    m_scene(scene),
    CURRENT_LEVEL(CURRENT_LEVEL),
    m_countLevelTime(false),
    m_launchTransition(true),
    m_timeCancelCollision(0)
{
    m_strName = "GameController";

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SET LEVEL TIME
//////////////////////////////////////////////////////////////////////////////////////////////////////
    m_gameTime.setTimeValue(1, 30, 0);
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
        // restart level if the player has enough health
        if (m_scene->getGameSystem().m_currentLives > 0) m_scene->getGameSystem().m_launchOption = is::DisplayOption::RESTART_LEVEL;
        else m_scene->getGameSystem().m_launchOption = is::DisplayOption::GAME_OVER;
        m_scene->setSceneEnd(true);
    }

    // the check point is activated when player reaches the required position
    if (player->getX() > m_x && !m_scene->getSceneEnd()) m_scene->getGameSystem().m_checkPoint = true;

    // when player finish the level
    if (!m_scene->getSceneEnd())
    {
        if (auto finishObject = static_cast<FinishObject*>(m_scene->SDMgetObject("FinishObject")); finishObject != nullptr)
        {
            if (finishObject->getStep() == 5)
            {
                //////////////////////////////////////////////////////////////////////
                m_scene->getGameSystem().m_currentLevel++; // allow to access the next level
                m_scene->getGameSystem().m_checkPoint = false;

                // increase game progress
                if (m_scene->getGameSystem().m_currentLevel > m_scene->getGameSystem().m_gameProgression &&
                    m_scene->getGameSystem().m_gameProgression < m_scene->getGameSystem().m_levelNumber)
                {
                    m_scene->getGameSystem().m_gameProgression++;
                }
                m_scene->getGameSystem().saveData(is::GameConfig::GAME_DATA_FILE); // save data
                //////////////////////////////////////////////////////////////////////

                // Normally we go to the next level. But as there is only one level for the
                // moment so we go to the game over scene.
                // the NEXT_LEVEL enum allows to go to another level but on condition that
                // this level is implemented in GameLevel loadResources() function towards line 80.
                m_scene->getGameSystem().m_launchOption = is::DisplayOption::GAME_END_SCREEN; // go to the game over scene
                m_scene->setSceneEnd(true);
                finishObject->addStep();
            }
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
        if (m_moveView && player->getSpriteX() > m_scene->getViewX() &&
            player->getSpriteX() < static_cast<float>(m_scene->getSceneWidth() * 32) - m_scene->getViewW() / 2.f)
            m_scene->setViewVarX(player->getSpriteX());
    }

    // update scene view
    m_scene->setView();

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      IN GAME
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_scene->getIsPlaying())
    {
        auto gameDialog = static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"));
        gameDialog->step(DELTA_TIME);

        auto cancelBt = static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"));
        // check if mouse is in collision with sprite
        if (m_scene->mouseCollision(cancelBt->getSprite()))
        {
            cancelBt->setImageAlpha(255);
            m_scene->getGameSystem().m_keyIsPressed = true;
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      LEVEL CHRONOMETER
//////////////////////////////////////////////////////////////////////////////////////////////////////
        // The time does not count down when an RPG type message is displayed
        if (m_countLevelTime && !gameDialog->showDialog())
        {
            if (m_gameTime.getTimeValue() != 0)
            {
                if (!m_playWarningSnd)
                {
                    if (m_gameTime.compareTime(0, 30, 0))
                    {
                        m_scene->GSMplaySound("warning");
                        m_scene->GSMstopMusic("world_1");
                        m_playWarningSnd = true;
                    }
                }
                else
                {
                    if (is::checkSFMLSndState(m_scene->GSMgetSound("warning"), is::SFMLSndStatus::Stopped) &&
                        is::checkSFMLSndState(m_scene->GSMgetMusic("world_1_hurry_up"), is::SFMLSndStatus::Stopped) &&
                        is::checkSFMLSndState(m_scene->GSMgetMusic("underground"), is::SFMLSndStatus::Stopped) &&
                        is::checkSFMLSndState(m_scene->GSMgetMusic("starman"), is::SFMLSndStatus::Stopped) &&
                        player->getIsActive())
                        m_scene->GSMplayMusic("world_1_hurry_up");
                }

                // level global time
                m_gameTime.step(DELTA_TIME);
            }
            else m_timeUp = true;
        }
        if (!gameDialog->showDialog()) pauseGame();

        // block update and collision test
        blockCollision(DELTA_TIME);
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
                static_cast<CancelButton*>(m_scene->SDMgetObject("CancelButton"))->setImageAlpha(0);
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

void GameController::blockCollision(float const &DELTA_TIME)
{
    // We call the object which is in the SDM container
    auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));

    short const yPrecision(6);
    short const xPrecision(10);
    bool rightCollision(true);
    bool avoidHorizontalCollision(false);

    // If this counter is greater than 0, it cancels all the collisions of the player at
    // the top in order to make the contacts with the blocks softer
    if (m_timeCancelCollision > 0.f) m_timeCancelCollision -= is::getMSecond(DELTA_TIME);
    else m_timeCancelCollision = 0.f;

    // this variables allow to store block temporally
    Block* blockGroundId = nullptr;
    Block* blockId = nullptr;

    // block animation, allows all block objects to have the same image
    Block::m_globalframe += 0.1f * is::VALUE_CONVERSION * DELTA_TIME; // image animation speed
    if (Block::m_globalframe > 40.5f) Block::m_globalframe = 38.f;
    if (Block::m_globalframe < 38.f) Block::m_globalframe = 38.f;

    // block loop
    for (std::list<std::shared_ptr<is::MainObject>>::iterator it = m_scene->m_SDMsceneObjects.begin();
        it != m_scene->m_SDMsceneObjects.end(); ++it)
    {
        if (is::instanceExist(*it))
        {
            if (m_scene->inViewRec(it->get(), false))
            {
                // This allows to know if the object browsed is a Block
                if ((*it)->getName() == "Block")
                {
                    auto currentBlock = static_cast<Block*>(it->get());
                    // Mario can use the object gives a 1UP mushroom if and only it is below him,
                    // otherwise it is rendered unusable
                    // When Mario receives the Bonus, everything is no longer active
                    if (currentBlock->getType() == Block::BLOCK_MUSHROOM_1UP && !currentBlock->getCreateBonus())
                    {
                        bool use(is::isBetween(player->getX(), currentBlock->getX(), currentBlock->getX() + currentBlock->getMaskW()) &&
                                player->getY() + ((player->getHealth() == 2) ? 0.f : 24.f) > currentBlock->getY() + currentBlock->getMaskW() - 2.f);
                        currentBlock->setIsActive(use);
                    }

                    if (currentBlock->getIsActive())
                    {
                        // check collision at bottom with Player
                        if (static_cast<int>(m_timeCancelCollision) == 0)
                        {
                            if (player->getY() + static_cast<float>(player->getMaskH()) < currentBlock->getY() + (static_cast<float>(yPrecision) + 4.f) &&
                                player->getY() + static_cast<float>(player->getMaskH()) > currentBlock->getY() - 2.f)
                            {
                                if (player->placeMetting(0, yPrecision + 10, currentBlock) && player->getVspAcc() > -0.99f)
                                {
                                    if (player->getVspAcc() > 0.5f) avoidHorizontalCollision = true;
                                    blockGroundId = currentBlock; // store the block to use it after
                                }
                            }
                        }

                        // check top collision with player
                        // here the collision takes over Mario's size
                        if (player->placeMetting(0, -yPrecision, currentBlock))
                        {
                            if (player->getVspAcc() < 0.f)
                            {
                                if (!currentBlock->getCreateBonus()) currentBlock->setYOffset(-10.f);

                                // if Mario is big he can destroy the blocks
                                if (currentBlock->getType() == Block::BLOCK_DESTRUCTIBLE)
                                {
                                    if (player->getHealth() == 2)
                                    {
                                        m_scene->GSMplaySound("break_block");
                                        currentBlock->setDestroyed();
                                    }
                                    else m_scene->GSMplaySound("bump");
                                }
                                else // blocks that give Bonuses
                                {
                                    if (!currentBlock->getCreateBonus() && static_cast<int>(currentBlock->getTime()) == 0)
                                    {
                                        float _X(currentBlock->getX()), _Y(currentBlock->getY());
                                        std::string sndName("powerup_appears");
                                        switch(currentBlock->getType())
                                        {
                                            case Block::BLOCK_COIN:
                                                sndName = "coin";
                                                addBonus();
                                                m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::COIN_FLY, _X + 4.f, _Y - 16.f, m_scene));
                                            break;
                                            case Block::BLOCK_MULTI_COIN:
                                                sndName = "coin";
                                                addBonus();
                                                currentBlock->reduceCoins();
                                                m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::COIN_FLY, _X + 4.f, _Y - 16.f, m_scene));
                                            break;
                                            case Block::BLOCK_MUSHROOM:
                                                if (player->getHealth() == 1)
                                                {
                                                    m_scene->SDMaddSceneObject(std::make_shared<Bonus>(Bonus::MUSHROOM, _X, _Y, m_scene));
                                                }
                                                else m_scene->SDMaddSceneObject(std::make_shared<Bonus>(Bonus::FIRE_FLOWER, _X, _Y, m_scene));
                                            break;
                                            case Block::BLOCK_MUSHROOM_1UP:
                                                m_scene->SDMaddSceneObject(std::make_shared<Bonus>(Bonus::MUSHROOM_1UP, _X, _Y, m_scene));
                                                currentBlock->setVisible(true);
                                            break;
                                            case Block::BLOCK_STARMAN:
                                                m_scene->SDMaddSceneObject(std::make_shared<Bonus>(Bonus::STARMAN, _X, _Y, m_scene));
                                            break;
                                        }
                                        m_scene->GSMplaySound(sndName);
                                        currentBlock->setCreateBonus(true);
                                    }
                                    else m_scene->GSMplaySound("bump");
                                }
                                player->setY(currentBlock->getY() + static_cast<float>(currentBlock->getMaskH()) +
                                            ((player->getHealth() == 1) ? -(player->getMaskH() / 2.f) : 0.f));
                                player->setVspAcc(1.f);
                                player->setIsJumping(false);
                            }
                            avoidHorizontalCollision = true;
                        }

                        // side collision with Player
                        if (!avoidHorizontalCollision)
                        {
                            if (player->placeMetting(xPrecision, 0, currentBlock)) // check collision at right
                            {
                                if (player->getHsp() > 0.f)
                                {
                                    blockId = currentBlock;
                                    rightCollision = true;
                                }
                            }
                            else if (player->placeMetting(-xPrecision, 0, currentBlock)) // check collision at left
                            {
                                if (player->getHsp() < 0.f)
                                {
                                    blockId = currentBlock;
                                    rightCollision = false;
                                }
                            }
                        }

                        // Check collision with others objects (Bonus, Enemy)
                        for (std::list<std::shared_ptr<is::MainObject>>::iterator _it = m_scene->m_SDMsceneObjects.begin();
                            _it != m_scene->m_SDMsceneObjects.end(); ++_it)
                        {
                            if (is::instanceExist((*_it)))
                            {
                                // Check collision on the sides (only for Enemy and Bonus)
                                auto sideCollision = [this, currentBlock](is::MainObject *obj, bool playBumpSnd = false)
                                {
                                    if (std::abs(obj->getHsp()) > 0.f && obj->getY() + 16.f > currentBlock->getY())
                                    {
                                        if (obj->placeMetting(1 , 0, currentBlock) && obj->getX() < currentBlock->getX() + 16.f && obj->getHsp() > 0.f)
                                        {
                                            obj->setHsp(-std::abs(obj->getHsp()));
                                            if (playBumpSnd) m_scene->GSMplaySound("bump");
                                        }
                                        if (obj->placeMetting(-1 , 0, currentBlock) && obj->getX() > currentBlock->getX() + 16.f && obj->getHsp() < 0.f)
                                        {
                                            obj->setHsp(std::abs(obj->getHsp()));
                                            if (playBumpSnd) m_scene->GSMplaySound("bump");
                                        }
                                    }
                                };

                                // Each type of object with its collision
                                if ((*_it)->getName() == "FireBall")
                                {
                                    auto obj = static_cast<FireBall*>(_it->get());
                                    if (!obj->getCreateExplosion())
                                    {
                                        if (obj->placeMetting(0 , 2, currentBlock) && obj->getVsp() > 0.f &&
                                            obj->getY() + 16.f < currentBlock->getY())
                                        {
                                            obj->setTime(0.f);
                                            obj->setY(currentBlock->getY() - 20.f);
                                        }
                                        if (std::abs(obj->getHsp()) > 0.f && obj->getY() + 16.f > currentBlock->getY())
                                        {
                                            if ((obj->placeMetting(1 , 0, currentBlock) && obj->getX() < currentBlock->getX() + 12.f) ||
                                                (obj->placeMetting(-1 , 0, currentBlock) && obj->getX() > currentBlock->getX() + 12.f))
                                            {
                                                m_scene->GSMplaySound("bump");
                                                obj->createExplosion();
                                            }
                                        }
                                    }
                                }
                                else if ((*_it)->getName() == "Enemy")
                                {
                                    auto obj = static_cast<Enemy*>(_it->get());
                                    if (!obj->getIsBeated())
                                    {
                                        // If Mario hits the block below and the enemy is above then he dies
                                        if (currentBlock->getYOffset() < 0.f && obj->placeMetting(0 , 2, currentBlock))
                                        {
                                            m_scene->SDMaddSceneObject(std::make_shared<Effect>(((obj->getType() == Enemy::KOOPA_TROOPA) ? Effect::SCORE_200 : Effect::SCORE_100),
                                                                                                  obj->getSpriteX(), obj->getY(), m_scene));
                                            obj->setIsBeated(true);
                                        }

                                        if (obj->placeMetting(0 , 2, currentBlock) && obj->getVsp() > 0.f)
                                        {
                                            obj->setOnGround(true);
                                            obj->setTime(0.f);
                                            obj->setY(currentBlock->getY() - obj->getMaskH());
                                        }
                                        sideCollision(_it->get(), obj->getType() == Enemy::KOOPA_TROOPA);
                                    }
                                }
                                else if ((*_it)->getName() == "Bonus")
                                {
                                    auto obj = static_cast<Bonus*>(_it->get());

                                    // If Mario hits the block below and the enemy is above then he dies
                                    if (currentBlock->getYOffset() < 0.f && obj->placeMetting(0 , 2, currentBlock))
                                    {
                                        obj->setHsp(std::abs(obj->getHsp()) * ((player->getSpriteX() > obj->getSpriteX()) ? -1.f : 1.f));
                                        obj->jump();
                                    }

                                    if (obj->placeMetting(0 , 2, currentBlock) && obj->getVsp() > 0.f && obj->getY() < currentBlock->getY())
                                    {
                                        obj->setOnGround(true);
                                        obj->setTime(0.f);
                                        obj->setY(currentBlock->getY() - obj->getMaskH());
                                    }
                                    if (obj->getY() < currentBlock->getY()) sideCollision(_it->get());
                                }
                            }
                        }
                    }
                    currentBlock->step(DELTA_TIME);
                    if (currentBlock->isDestroyed()) // destroy block at the end to avoid error
                    {
                        float _X(currentBlock->getX()), _Y(currentBlock->getY() + 16.f);
                        m_scene->getGameSystem().m_currentScore += 50;

                        // create blocks debris
                        m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::BLOCK_DEBRIS, _X, _Y, m_scene, 4.f, 4.f));
                        m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::BLOCK_DEBRIS, _X, _Y, m_scene, 4.f, -4.f));
                        m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::BLOCK_DEBRIS, _X, _Y, m_scene, 1.5f, 1.5f));
                        m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::BLOCK_DEBRIS, _X, _Y, m_scene, 1.5f, -1.5f));

                        // Here the object is deleted manually because its update is not managed by the SDM
                        it->reset();
                    }
                }
            }
        }
    }

    // apply collision on the player
    if (player->getIsActive())
    {
        // check if it has collision
        if (is::instanceExist(blockGroundId) && static_cast<int>(m_timeCancelCollision) == 0) player->setOnGround(true);
        else player->setOnGround(false);

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

void GameController::addBonus()
{
    m_scene->getGameSystem().m_currentBonus++;

    // add life if bonus > 99
    if (m_scene->getGameSystem().m_currentBonus > 99)
    {
        m_scene->GSMplaySound("1_up"); // We play this sound
        m_scene->getGameSystem().useVibrate(60);
        m_scene->getGameSystem().m_currentLives++;
        m_scene->getGameSystem().m_currentBonus = 0;
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
        m_scene->GSMplaySound("pause"); // We play this sound
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
    if (CURRENT_LEVEL == is::level::LevelId::LEVEL_1 && !m_scene->getGameSystem().m_checkPoint)
    {
        // We call the object which is in the SDM container
        auto gameDialog = static_cast<is::GameDialog*>(m_scene->SDMgetObject("GameDialog"));

        if (gameDialog->getDialogIndex() == is::GameDialog::DIALOG_NONE)
        {
            // the dialog is displayed after 1 seconds compared to the time of the level when the scene start
            if (m_gameTime.compareTime(1, 29)) gameDialog->setDialog(is::GameDialog::DIALOG_PLAYER_MOVE);
        }
    }
}

void GameController::stopSounds(bool val)
{
    if (m_scene->getGameSystem().m_enableSound)
    {
        if (val)
        {
            m_scene->GSMpauseSound("mario_die");
            m_scene->GSMpauseSound("flagpole");
            m_scene->GSMpauseSound("warning");
            m_scene->GSMpauseSound("stage_clear");
            m_scene->GSMpauseSound("score_count");
        }
        else
        {
            if (is::checkSFMLSndState(m_scene->GSMgetSound("mario_die"), is::SFMLSndStatus::Paused))   m_scene->GSMplaySound("mario_die");
            if (is::checkSFMLSndState(m_scene->GSMgetSound("flagpole"), is::SFMLSndStatus::Paused)) m_scene->GSMplaySound("flagpole");
            if (is::checkSFMLSndState(m_scene->GSMgetSound("warning"), is::SFMLSndStatus::Paused)) m_scene->GSMplaySound("warning");
            if (is::checkSFMLSndState(m_scene->GSMgetSound("stage_clear"), is::SFMLSndStatus::Paused)) m_scene->GSMplaySound("stage_clear");
            if (is::checkSFMLSndState(m_scene->GSMgetSound("score_count"), is::SFMLSndStatus::Paused)) m_scene->GSMplaySound("score_count");
        }
    }
    if (m_scene->getGameSystem().m_enableMusic)
    {
        if (val)
        {
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("world_1"), is::SFMLSndStatus::Playing)) m_scene->GSMpauseMusic("world_1");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("world_1_hurry_up"), is::SFMLSndStatus::Playing)) m_scene->GSMpauseMusic("world_1_hurry_up");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("underground"), is::SFMLSndStatus::Playing)) m_scene->GSMpauseMusic("underground");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("starman"), is::SFMLSndStatus::Playing)) m_scene->GSMpauseMusic("starman");
        }
        else
        {
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("world_1"), is::SFMLSndStatus::Paused)) m_scene->GSMplayMusic("world_1");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("world_1_hurry_up"), is::SFMLSndStatus::Paused)) m_scene->GSMplayMusic("world_1_hurry_up");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("underground"), is::SFMLSndStatus::Paused)) m_scene->GSMplayMusic("underground");
            if (is::checkSFMLSndState(m_scene->GSMgetMusic("starman"), is::SFMLSndStatus::Paused)) m_scene->GSMplayMusic("starman");
        }
    }
}

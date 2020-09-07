#include "Player.h"

Player::Player(sf::Texture &texPlayer, sf::Texture &texFire, bool &timeUp, is::GameDisplay *scene):
    MainObject(),
    Visibility(),
    Health(1, 2),
    GroundDetection(),
    m_scene(scene),
    m_texFire(texFire),
    m_timeUp(timeUp),
    m_vAcc(0.f),
    HSP_ACC(0.15f),
    HSP_MAX(2.5f),
    m_hspAcc(HSP_ACC),
    m_hspLim(HSP_MAX),
    VSP_LIM(9.f),
    VSP_ACC(0.8f),
    VSP_MAX(7.5f),
    VSP_FALL_ACC(0.385f),
    m_starFrame(0.f),
    m_starFrameIndex(0.f),
    m_timePlayerInvicibility(0),
    m_timeStarInvicibility(0),
    m_timeFreezPlayer(0),
    m_canMove(true),
    m_goDown(false),
    m_stopAnimation(false),
    m_moveRight(true),
    m_isJumping(false),
    m_playerIsKO(false),
    m_isBraking(false),
    m_haveFireBall(false)
{
    m_strName = "Player";
    m_depth = -1;

    // define collision mask
    setRectangleMask(24, 48);
    m_xOffset = 12.f;
    m_yOffset = 24.f;
    m_isActive = true;
    is::createSprite(texPlayer, m_sprParent, sf::IntRect(0, 0, 24, 48), sf::Vector2f(m_x, m_y), sf::Vector2f(m_xOffset, m_yOffset));
}

void Player::step(float const &DELTA_TIME)
{
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      PLAYER IS ACTIVE
//////////////////////////////////////////////////////////////////////////////////////////////////////

        // counters
        int const M_SECONDS = is::getMSecond(DELTA_TIME);
        float const SUB_IMAGE = 9.f; // sub image number

        if (m_isActive)
        {
            if (m_time > 0) m_time -= M_SECONDS; else m_time = 0;

            // reduce player freez time
            if (m_timeFreezPlayer > 0) m_timeFreezPlayer -= M_SECONDS;
            else m_timeFreezPlayer = 0;

            // reduce player star invincibility time
            if (m_timeStarInvicibility > 0) m_timeStarInvicibility -= M_SECONDS;
            else m_timeStarInvicibility = 0;

            if (m_timeFreezPlayer == 0)
            {
                // reduce player invincibility time
                if (m_timePlayerInvicibility > 0) m_timePlayerInvicibility -= M_SECONDS;
                else m_timePlayerInvicibility = 0;
            }

            // stop the starman music at 2 seconds before the end and play level music
            if (m_timeStarInvicibility < is::SECOND * 2)
            {
                if (m_timeStarInvicibility != 0)
                {
                    auto msc = m_scene->GSMgetMusic("starman");
                    if (is::getSFMLSndState(msc, sf::Sound::Playing))
                    {
                        msc->stop();
                        m_scene->GSMplayMusic("world_1");
                    }
                }
            }

            // Starman animation
            if (m_timeStarInvicibility > 0)
            {
                m_starFrame += 0.2f * is::VALUE_CONVERSION * DELTA_TIME; // image speed
                if (m_starFrame > 1.f)
                {
                    m_starFrameIndex += SUB_IMAGE;
                    m_starFrame = 0.f;
                }
                if (m_starFrameIndex > SUB_IMAGE * 3.f) m_starFrameIndex = 0.f;
            }
            else
            {
                m_starFrameIndex = 0.f;
                m_starFrame = 0.f;
            }

            if (m_timeFreezPlayer == 0)
            {
                // use big Mario frame when health greater than 1
                m_frameStart = ((m_health > 1) ? (((!m_haveFireBall) ? 5.f : 6.f) * SUB_IMAGE) : 0.f) +
                               (((!m_haveFireBall || m_health != 2) && m_timeStarInvicibility > 0) ? SUB_IMAGE : 0.f);
                m_frameEnd   = ((m_health > 1) ? (((!m_haveFireBall) ? 6.f : 7.f) * SUB_IMAGE - 1.f) : SUB_IMAGE) +
                               (((!m_haveFireBall || m_health != 2) && m_timeStarInvicibility > 0) ? SUB_IMAGE : 0.f);

                // We call the GameKeyData object from the SDM container
                auto gameKey = static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"));

                // Take a test to find out if attack key is pressed
                if (gameKey->m_keyBPressed && !gameKey->m_keyBUsed)
                {
                    // create object once
                    if (FireBall::instanceNumber < 2 && m_haveFireBall)
                    {
                        m_scene->GSMplaySound("fireball"); // We play this sound

                        // Add the object to the SDM container
                        m_scene->SDMaddSceneObject(std::make_shared<FireBall>(m_texFire, getSpriteX() + (8.f * m_imageXscale),
                                                                              getSpriteY() - 8.f, 8.f * m_imageXscale, m_scene));
                    }
                    gameKey->m_keyBUsed = true;
                }

                m_hspLim = ((gameKey->is::GameKeyData::m_keyBPressed) ? HSP_MAX * 2.f : HSP_MAX);

                // We set this variable to false to avoid that the player is always lowered when the DOWN key is released
                m_isDuking = false;

                // horizontal move
                m_hspAcc = HSP_ACC + ((m_isBraking) ? 0.1f : 0.f);
                if (gameKey->m_keyRightPressed)
                {
                    if (m_hsp < m_hspLim) m_hsp += (m_hspAcc * is::VALUE_CONVERSION) * DELTA_TIME;
                    m_isBraking = ((m_hsp < 0.f) ? true : false);
                    m_moveRight = true;
                }
                else if (gameKey->m_keyLeftPressed)
                {
                    if (m_hsp > -m_hspLim) m_hsp -= (m_hspAcc * is::VALUE_CONVERSION) * DELTA_TIME;
                    m_isBraking = ((m_hsp > 0.f) ? true : false);
                    m_moveRight = false;
                }
                else if (gameKey->m_keyDownPressed)
                {
                    // only big Mario can duking
                    if (m_health > 1 && m_onGround) m_isDuking = true;
                }
                else // friction
                {
                    m_isBraking = false;

                    if (m_moveRight)
                    {
                        if (m_hsp > 0.5f) m_hsp -= 0.25f; else m_hsp = 0.f;
                    }
                    else
                    {
                        if (m_hsp < -0.5f) m_hsp += 0.25f; else m_hsp = 0.f;
                    }
                }

                if (m_vAcc < 0.f) m_time = 0;

                // if player is on ground
                if (m_onGround)
                {
                    // make a jump
                    if (gameKey->m_keyAPressed && !gameKey->m_keyAUsed)
                    {
                        std::string sndName((m_health != 2) ? "jump_small" : "jump_super");
                        m_scene->GSMplaySound(sndName); // We play this sound
                        makeJump(VSP_MAX, gameKey->m_keyAUsed);
                    }
                }
                else // in air (gravity effect)
                {
                    // if player is jumping
                    if (m_isJumping)
                    {
                        // propulsion speed
                        if (m_vsp < m_vAcc) m_vAcc -= (VSP_ACC * is::VALUE_CONVERSION) * DELTA_TIME;
                        else m_isJumping = false; // cancel the player's propulsion
                    }
                    else
                    {
                        if (m_vAcc < VSP_LIM) m_vAcc += (VSP_FALL_ACC * is::VALUE_CONVERSION) * DELTA_TIME;
                        else m_vAcc = VSP_LIM;
                    }
                }

                // if the player outs at the bottom of the level then he loses
                if (m_y > m_scene->getViewY() + (m_scene->getViewH() / 2.f)) playerHurt(false);

                // player will lose if the chronometer reaches 00min : 00s : 00
                if (m_timeUp) playerHurt(false);

                // avoid the player to out at the left of the level
                if (m_x < m_scene->getViewX() - (m_scene->getViewW() / 2.f) && m_hsp < 0.f)
                {
                    m_x = m_xPrevious;
                    m_hsp = 0.f;
                }

                // avoid the player to out at the right of the level
                if (m_x > static_cast<float>(m_scene->getSceneWidth() * 32) - 32.f)
                {
                    m_hsp = 0.f;
                    m_x = static_cast<float>(m_scene->getSceneWidth() * 32) - 32.f;
                }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      ANIMATION
//////////////////////////////////////////////////////////////////////////////////////////////////////
                if (m_goDown)
                {
                    if (!m_stopAnimation) m_frame += 0.22f * is::VALUE_CONVERSION * DELTA_TIME; // image speed
                    setFrameLimit(m_frameStart + 6.f, m_frameStart + 7.5f);
                }
                else if (m_onGround)
                {
                    if (m_isDuking) m_frame = m_frameEnd;               // if player is duking
                    else if (m_isBraking) m_frame = m_frameStart + 4.f; // if player is braking
                    else if (std::abs(m_hsp) > 0.5f)                    // if player is moving
                    {
                        m_frame += (((m_hspLim > HSP_MAX) ? 0.4f : 0.22f) * is::VALUE_CONVERSION) * DELTA_TIME; // image speed
                        setFrameLimit(m_frameStart + 1.f, m_frameStart + 3.5f);
                    }
                    else m_frame = m_frameStart; // animation stand up
                }
                else // animations in the air
                {
                    if (m_vAcc < 0.f) m_frame = m_frameStart + 5.f;
                }

                // update object position
                m_xPrevious = m_x;
                m_yPrevious = m_y;
                if (m_canMove) m_x += (m_hsp * is::VALUE_CONVERSION) * DELTA_TIME;
                m_y += (m_vAcc * is::VALUE_CONVERSION) * DELTA_TIME;
            }
            else
            {
                // Animation of Mario shrinking or enlarging when he gets the mushroom or hits an enemy
                if (!m_haveFireBall)
                {
                    if (m_timeFreezPlayer % 3 == 0) m_frame += ((m_frame > 34.f) ? -SUB_IMAGE * 5.f : SUB_IMAGE * 5.f);
                    if (m_timeFreezPlayer % 6 == 0) m_frame += ((m_frame > 34.f) ? -SUB_IMAGE * 5.f : SUB_IMAGE * 5.f);
                    if (static_cast<int>(m_frame) == SUB_IMAGE - 1.f) m_frame = 0.f;
                }
            }

            // update collision mask (position, size, ...)
            updateCollisionMask();

            // change mask height in relation to Mario size
            if (m_health != 2 || m_isDuking) m_aabb.m_top += 24;
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      IF PLAYER IS NOT ACTIVE
//////////////////////////////////////////////////////////////////////////////////////////////////////
        else
        {
            // animation : stand up
            if (m_health > 0)
            {
                m_frame = ((m_health == 1) ? 35.f : 0.f);
            }
            else // when player is KO
            {
                gameCtrl->setCountLevelTime(false); // stop level chronometer
                m_frame = SUB_IMAGE - 1.f;

                // Get the player out of the level when he loses
                if (m_y < (static_cast<float>(m_scene->getSceneHeight() * 32) + 24.f))
                {
                    m_time += M_SECONDS;
                    m_vsp = (3.3f * is::VALUE_CONVERSION) * DELTA_TIME;
                    if (m_time > 30.f) m_y += m_vsp;
                    else m_y -= m_vsp;
                }
                else
                {
                    if (!m_playerIsKO)
                    {
                        if (!is::getSFMLSndState(m_scene->GSMgetSound("mario_die"), sf::Sound::Playing))
                            m_playerIsKO = true;
                    }
                }
            }
        }

        // change the sprite face
        if (m_hsp > 0.f) m_imageXscale = 1.f;
        if (m_hsp < 0.f) m_imageXscale = -1.f;

        // update sprite
        is::setFrame(m_sprParent, m_frame + m_starFrameIndex, static_cast<int>(SUB_IMAGE), 24, 48, 24, 48);
        updateSprite();
    }
}

bool Player::placeMetting(int x, int y, Block* other)
{
    is::Rectangle testRec = this->getMask();

    if (x > 0) testRec.m_left += 20;
    if (x < 0) testRec.m_right -= 20;

    testRec.m_left += x;
    testRec.m_right += x;
    testRec.m_top += y;
    testRec.m_bottom += y;

    testRec.m_left += 2;
    testRec.m_right -= 2;

    bool isCollision = false;

    is::Rectangle otherRectangle = other->getMask();

    if (is::collisionTest(testRec, otherRectangle))
    {
        isCollision = true;
    }
    return isCollision;
}

bool Player::getIsKO() const
{
    return m_playerIsKO;
}

bool Player::playerIsVulnerable() const
{
    return (m_timePlayerInvicibility == 0 && getIsActive());
}

bool Player::getIsJumping() const
{
    return m_isJumping;
}

float Player::getVspAcc() const
{
    return m_vAcc;
}

float Player::getMaxHsp() const
{
    return m_hspLim;
}

float Player::getHspAcc() const
{
    return HSP_ACC;
}

void Player::draw(sf::RenderTexture &surface)
{
    if (m_visible)
    {
        if (m_timePlayerInvicibility % 2 == 0) surface.draw(m_sprParent);
    }
}

void Player::setIsJumping(bool val)
{
    m_isJumping = val;
}

void Player::setCanMove(bool val)
{
    m_canMove = val;
}

void Player::setIsKO(bool val)
{
    m_playerIsKO = val;
}

void Player::setVspAcc(float val)
{
    // if the value of the propulsion speed is positive then cancel the jump
    if (val > 0.f && m_isJumping)
    {
        if (m_time > 5) m_time = 4;
        m_isJumping = false;
    }
    m_vAcc = val;
}

void Player::setOnGround(bool val)
{
    if (static_cast<int>(m_time) == 0) m_onGround = val;
}

void Player::makeJump(float vspLim, bool &_keyState)
{
    m_isJumping = true;
    m_vAcc = 0.f;
    m_vsp = -vspLim; // limit of the jump height
    m_time = 20;
    _keyState = true;
    m_onGround = false;
}

void Player::playerHurt(bool canContinue)
{
    if (canContinue)
    {
        m_haveFireBall = false;

        // If the number of health is greater than 0 he can continue the game otherwise he loses
        if (getHealth() > 1)
        {
            m_scene->GSMplaySound("pipe"); // We play this sound
            m_scene->getGameSystem().useVibrate(60);
            addHealth(-1);
            m_timeFreezPlayer = is::SECOND;
            m_timePlayerInvicibility = 60;
        }
        else
        {
            if (getIsActive())
            {
                m_time = 0.f;
                m_scene->GSMplaySound("mario_die"); // We play this sound
                m_scene->GSMgetMusic("world_1")->stop(); // We stop this sound
                m_scene->GSMgetMusic("underground")->stop(); // We stop this sound
                m_scene->getGameSystem().useVibrate(60);
                setHealth(0);
                setIsActive(false);
            }
        }
    }
    else
    {
        if (getIsActive())
        {
            m_time = 0.f;
            m_scene->GSMplaySound("mario_die"); // We play this sound
            m_scene->GSMgetMusic("world_1")->stop(); // We stop this sound
            m_scene->GSMgetMusic("underground")->stop(); // We stop this sound
            m_scene->GSMgetMusic("starman")->stop(); // We stop this sound
            m_scene->getGameSystem().useVibrate(60);
            setHealth(0);
            setIsActive(false);
        }
    }
}

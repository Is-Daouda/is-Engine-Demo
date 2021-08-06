#include "Bonus.h"

Bonus::Bonus(BonusType type, float x, float y, is::GameDisplay *scene):
    MainObject(x, y),
    Step(0),
    ScorePoint(100), // add score for coin
    GroundDetection(),
    Type(type),
    m_scene(scene),
    m_touchPlayer(false),
    m_starting(true),
    m_timeJump(0.f)
{
    m_xOffset = 16.f;
    m_yOffset = 16.f;
    setRectangleMask(32, 32);

    // We create the sprite of the object.
    // On SDL this sprite will be blit (Useful for drawing a group of different objects whose
    // sprites have the same textures. This makes the game more optimized).
    scene->createSprite("bonus", *this, sf::IntRect(32 * m_type, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));
    if (m_type == COIN) m_starting = false;
    else m_strName = "Bonus";
    m_depth = 1;
}

void Bonus::step(float const &DELTA_TIME)
{
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
        // We call the player object which is in the SDM container
        if (auto player = static_cast<Player*>(m_scene->SDMgetObject("Player")); player->getIsActive())
        {
            // If the player is at a certain distance from the Bonus then he can get it
            if (is::pointDistance(player->getSpriteX(), player->getSpriteY(), m_x + 16.f, m_y + 16.f) < 34.f &&
                !m_touchPlayer && !m_starting)
            {
                Effect::EffectType type = Effect::SCORE_1000;

                // The action changes according to the Bonus
                switch (m_type)
                {
                    case COIN:
                        m_scene->getGameSystem().m_currentScore += getScorePoint();
                        gameCtrl->addBonus();
                        m_scene->GSMplaySound("coin"); // We play this sound
                    break;

                    case MUSHROOM:
                        player->addHealth();
                        player->setTimeFreezPlayer(is::SECOND);
                        m_scene->GSMplaySound("powerup"); // We play this sound
                    break;

                    case FIRE_FLOWER:
                        if (!player->getHaveFireBall()) player->haveFireBall();

                        m_scene->GSMplaySound("powerup"); // We play this sound
                    break;

                    case STARMAN:
                        player->haveStarPower();
                        m_scene->GSMplaySound("powerup"); // We play this sound
                        m_scene->GSMstopMusic("world_1");
                        m_scene->GSMstopMusic("world_1_hurry_up");
                        m_scene->GSMplayMusic("starman");
                    break;

                    case MUSHROOM_1UP:
                        m_scene->getGameSystem().m_currentLives++;
                        m_scene->GSMplaySound("1_up"); // We play this sound
                        type = Effect::ONE_UP;
                    break;
                }
                if (m_type != COIN) m_scene->SDMaddSceneObject(std::make_shared<Effect>(type, m_x, m_y, m_scene));
                m_touchPlayer = true;
            }
        }

        // Allow to manage the movement and gravity of the object
        auto physic = [this, &DELTA_TIME]()
        {
            if (m_starting)
            {
                m_timeJump = 0.f;
                if (m_y > m_yStart - 32.f) m_y -= 1.f * is::VALUE_CONVERSION * DELTA_TIME;
                else
                {
                    m_hsp = 2.f;
                    m_starting = false;
                }
            }
            else
            {
                if (m_type != FIRE_FLOWER)
                {
                    m_time += is::getMSecond(DELTA_TIME);
                    m_x += m_hsp * is::VALUE_CONVERSION * DELTA_TIME;

                    // bounce the object
                    if (m_type == STARMAN)
                    {
                        if (m_time > 20.f) is::increaseVar(DELTA_TIME, m_vsp, 1.f, 4.f, 4.f); // go down
                        else is::decreaseVar(DELTA_TIME, m_vsp, 0.8f, -3.5f, -3.5f); // go up
                    }
                    else // normal physic horizontal movement and gravity effect
                    {
                        if (m_timeJump > 0.f) m_timeJump -= is::getMSecond(DELTA_TIME); else m_timeJump = 0.f;
                        if (static_cast<int>(m_timeJump) == 0)
                        {
                            if (m_onGround) m_vsp = 0.f;
                            else is::increaseVar(DELTA_TIME, m_vsp, 0.5f, 3.5f, 3.5f);
                        }
                        else is::decreaseVar(DELTA_TIME, m_vsp, 1.f, -2.f, -2.f); // jump propulsion time
                        if (m_time > 20.f) m_onGround = false;
                    }
                    m_y += m_vsp * is::VALUE_CONVERSION * DELTA_TIME;
                    if (m_y > static_cast<float>(m_scene->getSceneHeight() * 32) + 24.f) m_destroy = true;
                }
            }
        };

        m_frame += 0.15f * is::VALUE_CONVERSION * DELTA_TIME;
        switch(m_type)
        {
            case COIN:
                setFrameLimit(6.f, 11.5f);
            break;

            case MUSHROOM:
                physic();
                m_frame = 0.f;
            break;

            case MUSHROOM_1UP:
                physic();
                m_frame = 1.f;
            break;

            case FIRE_FLOWER:
                physic();
                setFrameLimit(2.f, 5.5f);
            break;

            case STARMAN :
                physic();
                setFrameLimit(12.f, 17.5f);
            break;
        }

        if (m_touchPlayer)
        {
            m_destroy = true; // We destroy the object
        }
        is::setFrame(m_sprParent, m_frame, 6, 32);
        updateSprite();
        centerCollisionMask(m_x + m_xOffset, m_y + m_yOffset);
    }
}

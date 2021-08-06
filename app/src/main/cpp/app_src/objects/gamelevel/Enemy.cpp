#include "Enemy.h"

Enemy::Enemy(EnemyType type, float x, float y, is::GameDisplay *scene):
    MainObject(x, y),
    GroundDetection(),
    Type(type),
    m_scene(scene),
    m_awakeTime(0.f),
    m_timeCancelCollision(0.f),
    m_timeHurt(0.f),
    m_isBeated(false),
    m_isTouched(false),
    m_isKicked(false)
{
    m_strName = "Enemy";
    setRectangleMask(32, 32);
    m_xOffset = m_yOffset = 16.f;

    // We create the sprite of the object.
    // On SDL this sprite will be blit (Useful for drawing a group of different objects whose
    // sprites have the same textures. This makes the game more optimized).
    scene->createSprite("enemy", *this, sf::IntRect(0, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));

    switch (m_type)
    {
    case PIRANA_PLANT:
        m_depth = is::DepthObject::BIG_DEPTH + 1; // appears behind the tiles
        m_x += 16.f;
    break;

    default:
        m_hsp = -1.f;
    break;
    }
}

void Enemy::step(float const &DELTA_TIME)
{
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
        if (m_scene->inViewRec(this))
        {
            auto contactWithOtherObject = [this, &DELTA_TIME, gameCtrl]()
            {
                for (std::list<std::shared_ptr<is::MainObject>>::iterator it = m_scene->m_SDMsceneObjects.begin();
                    it != m_scene->m_SDMsceneObjects.end(); ++it)
                {
                    if (is::instanceExist(*it))
                    {
                        if (!m_isBeated)
                        {
                            if ((*it)->getName() == "Player")
                            {
                                auto player = static_cast<Player*>(it->get());

                                // Star power
                                if (player->getHaveStarPower() && placeMetting(0, 0, player))
                                {
                                    m_scene->SDMaddSceneObject(std::make_shared<Effect>(((m_type == KOOPA_TROOPA) ? Effect::SCORE_200 : Effect::SCORE_100),
                                                                                          getSpriteX(), m_y, m_scene));
                                    m_scene->GSMplaySound("stomp");
                                    m_isTouched = false;
                                    m_isBeated = true;
                                }

                                // collision with player (supports Mario size)
                                if (!m_isBeated)
                                {
                                    if (placeMetting(0, 0, player) && player->getIsActive() &&
                                        static_cast<int>(m_timeCancelCollision) == 0)
                                    {
                                        auto kooperKick = [this, player](bool jump)
                                        {
                                            if (m_isKicked)
                                            {
                                                m_hsp = 0.f;
                                                m_isKicked = false;
                                                m_scene->GSMplaySound("stomp");
                                            }
                                            else
                                            {
                                                m_isKicked = true;
                                                m_hsp = 6.3f * ((player->getSpriteX() > getSpriteX()) ? -1.f : 1.f);
                                                m_scene->GSMplaySound("kick");
                                            }
                                            m_awakeTime = 0.f;
                                            if (jump) player->makeJump(2.f, static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"))->m_keyAUsed);
                                        };
                                        // Mario can beat the enemy if he is on top
                                        if (player->getY() + (player->getMaskH() / ((player->getHealth() == 2) ? 1 : 2)) < m_y + 12.f && !player->getOnGround())
                                        {
                                            switch (m_type)
                                            {
                                                case LITTLE_GOOMBA:
                                                    player->makeJump(2.f, static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"))->m_keyAUsed);
                                                    m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::SCORE_100, getSpriteX(), m_y, m_scene));
                                                    m_scene->GSMplaySound("stomp");
                                                    m_isTouched = true;
                                                    m_isBeated = true;
                                                break;

                                                case KOOPA_TROOPA:
                                                    if (!m_isTouched)
                                                    {
                                                        m_isTouched = true;
                                                        m_hsp = 0.f;
                                                        m_scene->SDMaddSceneObject(std::make_shared<Effect>(Effect::SCORE_100, getSpriteX(), m_y, m_scene));
                                                        m_scene->GSMplaySound("stomp");
                                                        player->makeJump(2.f, static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"))->m_keyAUsed);
                                                    }
                                                    else
                                                    {
                                                        kooperKick(true);
                                                    }
                                                    m_timeCancelCollision = is::SECOND / 2.f;
                                                break;
                                            }
                                        }
                                        else // Otherwise Mario is hit
                                        {
                                            if (m_type == KOOPA_TROOPA && m_isTouched && !m_isKicked)
                                            {
                                                kooperKick(false);
                                                m_timeCancelCollision = is::SECOND / 2.f;
                                            }
                                            else
                                            {
                                                if (player->playerIsVulnerable()) player->playerHurt();
                                            }
                                        }
                                    }

                                    // prevent the plant from moving when Mario is on top of him
                                    if (m_type == PIRANA_PLANT)
                                    {
                                        if (is::isBetween(player->getSpriteX(), getSpriteX() - 32.f, getSpriteX() + 32.f) &&
                                            m_y > (m_yStart + 38.f) && m_time > 3.f)
                                            m_time -= is::getMSecond(DELTA_TIME);
                                    }
                                }
                            }

                            // If it hits other enemies when it's kicked
                            if (m_type == KOOPA_TROOPA)
                            {
                                if (m_isTouched && m_isKicked)
                                {
                                    if ((*it)->getName() == "Enemy" && it->get() != this)
                                    {
                                        auto obj = static_cast<Enemy*>(it->get());
                                        if (placeMetting(0, 0, obj) && !obj->getIsBeated())
                                        {
                                            m_scene->GSMplaySound("kick");
                                            obj->setIsBeated(true);
                                        }
                                    }
                                }
                            }

                            // collision with Fire Ball
                            if ((*it)->getName() == "FireBall")
                            {
                                auto obj = static_cast<FireBall*>(it->get());
                                if (placeMetting(0, 0, obj) && !obj->getCreateExplosion())
                                {
                                    m_scene->GSMplaySound("stomp");
                                    m_scene->SDMaddSceneObject(std::make_shared<Effect>(((m_type == KOOPA_TROOPA) ? Effect::SCORE_200 : Effect::SCORE_100),
                                                                                          getSpriteX(), m_y, m_scene));
                                    obj->createExplosion();
                                    m_isBeated = true;
                                    break;
                                }
                            }
                        }
                        else break;
                    }
                }
            };

            m_time += is::getMSecond(DELTA_TIME);
            if (m_timeCancelCollision > 0.f) m_timeCancelCollision -= is::getMSecond(DELTA_TIME);
            else m_timeCancelCollision = 0.f;
            m_frame += 0.15f * is::VALUE_CONVERSION * DELTA_TIME;

            // Allow to manage the movement and gravity of the object
            auto physic = [this, &DELTA_TIME]()
            {
                m_imageXscale = (m_hsp > 0) ? -1.f : 1.f;
                if (!m_isBeated)
                {
                    m_x += m_hsp * is::VALUE_CONVERSION * DELTA_TIME;
                    if (m_onGround) m_vsp = 0.f;
                    else is::increaseVar(DELTA_TIME, m_vsp, 0.5f, 3.5f, 3.5f);
                    if (m_time > 20.f) m_onGround = false;
                }
                else
                {
                    if (!m_isTouched)
                    {
                        m_imageYscale = -1.f;
                        m_timeHurt += is::getMSecond(DELTA_TIME);
                        if (m_timeHurt > 10.f) m_vsp = 4.f;
                        else m_vsp = -4.f;
                    }
                }
                m_y += m_vsp * is::VALUE_CONVERSION * DELTA_TIME;
                if (m_y > static_cast<float>(m_scene->getSceneHeight() * 32) + 24.f) m_destroy = true;
                if (m_time > 20.f) m_onGround = false;
            };

            switch (m_type)
            {
                case Enemy::EnemyType::LITTLE_GOOMBA:
                    physic();
                    contactWithOtherObject();
                    if (!m_isBeated) setFrameLimit(0.f, 1.5f);
                    else
                    {
                        m_frame = 0.f;
                        if (m_isTouched)
                        {
                            if (m_imageAlpha > 5) m_imageAlpha -= static_cast<int>(4.f * is::VALUE_CONVERSION * DELTA_TIME);
                            else m_destroy = true;
                            m_imageYscale = 0.1f;
                            m_yOffset = 30.f;
                        }
                    }
                break;

                case Enemy::EnemyType::KOOPA_TROOPA:
                    physic();
                    contactWithOtherObject();
                    if (m_isTouched)
                    {
                        if (!m_isKicked)
                        {
                            m_awakeTime += is::getMSecond(DELTA_TIME);
                            if (m_awakeTime > 4.f * is::SECOND)
                            {
                                m_awakeTime = 0.f;
                                m_hsp = is::choose(2, -1.f, 1.f);
                                m_isTouched = false;
                            }
                        }
                    }
                    else setFrameLimit(2.f, 3.5f);
                    if (m_isBeated || m_isTouched) m_frame = 4.f;
                break;

                case Enemy::EnemyType::PIRANA_PLANT:
                    contactWithOtherObject();

                    // allow to move pirana plant
                    if (!m_isBeated)
                    {
                        if (m_time > 6.f * is::SECOND)
                        {
                            m_speed = -1.f;
                            m_time = 0.f;
                        }
                        else if (m_time > 3.f * is::SECOND) m_speed = 1.f;
                        m_y += m_speed * is::VALUE_CONVERSION * DELTA_TIME;
                        if (m_y < m_yStart) m_y = m_yStart;
                        if (m_y > m_yStart + 40.f) m_y = m_yStart + 40.f;
                        setFrameLimit(5.f, 6.5f);
                    }
                    else m_frame = 5.f;
                break;

                default :
                break;
            }

            // update sprite
            is::setFrame(m_sprParent, m_frame, 4, 32);
            updateSprite();
            centerCollisionMask(m_x + m_xOffset, m_y + 16.f);
        }
    }
}

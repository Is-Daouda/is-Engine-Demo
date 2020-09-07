#include "FinishObject.h"

FinishObject::FinishObject(sf::Texture &tex, float x, float y, is::GameDisplay *scene):
    MainObject(x, y),
    Step(0),
    m_scene(scene),
    addScore(0)
{
    m_strName = "FinishObject";
    setRectangleMask(32, 32);
    is::createSprite(tex, m_sprCastleFlag, sf::IntRect(160, 224, 32, 32), sf::Vector2f(m_x + 224.f, m_y + 170.f), sf::Vector2f(0.f, 0.f));
    is::createSprite(tex, m_sprCastle, sf::IntRect(0, 224, 160, 160), sf::Vector2f(m_x + 160.f, m_y + 160.f), sf::Vector2f(0.f, 0.f));
    m_x += 13.f;
    is::createSprite(tex, m_sprParent, sf::IntRect(160, 256, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(0.f, 0.f));
}

void FinishObject::step(float const &DELTA_TIME)
{
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
        auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));
        switch (m_step)
        {
            case 0:
            // If the player touches the object then he has gained the level
            if (player->getX() > m_x + 8.f)
            {
                Effect::EffectType type = Effect::SCORE_5000;
                if (is::isBetween(player->getY(), m_yStart + 32.f, m_yStart + 96.f))        type = Effect::SCORE_2000;
                else if (is::isBetween(player->getY(), m_yStart + 96.f, m_yStart + 160.f))  type = Effect::SCORE_800;
                else if (is::isBetween(player->getY(), m_yStart + 160.f, m_yStart + 480.f)) type = Effect::SCORE_100;
                m_scene->SDMaddSceneObject(std::make_shared<Effect>(gameCtrl->m_texEffect, type,
                                                                    player->getSpriteX(), player->getSpriteY(), m_scene));
                m_scene->GSMplaySound("flagpole"); // We play this sound
                m_scene->GSMgetMusic("world_1")->stop();
                m_scene->GSMgetMusic("starman")->stop();
                static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"))->m_disableAllKey = true; // deactivate commands
                player->setHsp(0.f);
                player->setVspAcc(0.f);
                player->setGoDown(true);
                gameCtrl->setCountLevelTime(false);
                addStep(); // We go to the next step
            }
            break;

            case 1: // Lower the flag
                {
                    float _SPEED(2.6f * is::VALUE_CONVERSION * DELTA_TIME);
                    if (m_y < m_yStart + 242.f)
                    {
                        m_y += _SPEED;
                    }
                    else
                    {
                        player->setStopAnimation(true);
                        player->setImageXscale(-1.f);
                        player->setX(m_x + 24.f);
                        m_time += is::getMSecond(DELTA_TIME);
                        if (m_time > is::SECOND)
                        {
                            m_scene->GSMplaySound("stage_clear"); // We play this sound
                            player->setGoDown(false);
                            addStep();
                        }
                    }
                    player->setVspAcc(0.f);
                    if (player->getY() < m_y - 8.f) player->setY(m_y - 7.f);
                    else player->setY(player->getY() + _SPEED);
                }
            break;

            case 2: // Move Mario to the castle
                player->setHsp(3.f);
                if (player->getSpriteX() > m_xStart + 246.f)
                {
                    player->setVisible(false);
                    m_scene->GSMplaySound("score_count"); // We play this sound
                    addStep();
                }
            break;

            case 3: // Time bonus
                {
                    int t(gameCtrl->m_gameTime.getTimeValue());
                    if (t > 0)
                    {
                        m_time += is::getMSecond(DELTA_TIME);
                        if (m_time > 1.f)
                        {
                            addScore++;
                            if (addScore > 5)
                            {
                                m_scene->getGameSystem().m_currentScore += 50;
                                addScore = 0;
                            }
                            t -= 10;
                            if (t < 0) t = 0;
                        }
                        else m_time = 0.f;
                        gameCtrl->m_gameTime.setMSecond(t);
                    }
                    else
                    {
                        gameCtrl->m_gameTime.setTimeValue(0, 0, 0);
                        m_scene->GSMgetSound("score_count")->stop();
                        if (!is::getSFMLSndState(m_scene->GSMgetSound("stage_clear"), sf::Sound::Playing)) addStep();
                    }
                }
            break;

            case 4:  // Move the flag above the castle upwards and exit the level
                is::moveSFMLObjY(m_sprCastleFlag, -0.5f * is::VALUE_CONVERSION * DELTA_TIME);
                if (is::getSFMLObjY(m_sprCastleFlag) < m_yStart + 128.f) addStep();
            break;

            // case 5:
            // the implementation of this part is in the step() method of GameController
            // break;
        }
        updateSprite();
        updateCollisionMask();
        is::setSFMLObjProperties(m_sprCastleFlag, is::getSFMLObjX(m_sprCastleFlag), is::getSFMLObjY(m_sprCastleFlag));
    }
}

void FinishObject::draw(sf::RenderTexture &surface)
{
    // We draw the object only if it is in the field of vision of the scene view
    if (m_scene->inViewRec(this))
    {
        surface.draw(m_sprParent);
        surface.draw(m_sprCastleFlag);
        surface.draw(m_sprCastle);
    }
}

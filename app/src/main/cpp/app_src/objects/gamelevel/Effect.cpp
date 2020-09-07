#include "Effect.h"

Effect::Effect(sf::Texture &tex, EffectType type, float x, float y, is::GameDisplay *scene, float speed, float Hspeed):
    MainObject(x, y),
    Type(type),
    ScorePoint(),
    m_scene(scene)
{
    m_strName = "Effect";
    m_xOffset = m_yOffset = 16.f;
    m_frame   = m_type;
    m_speed   = speed;
    m_hsp     = Hspeed;

    is::createSprite(tex, m_sprParent, sf::IntRect(0, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));

    // Score to add
    switch (m_type)
    {
        case SCORE_100:
        case COIN_FLY:
            m_scorePoint = 100;
        break;
        case SCORE_200:  m_scorePoint = 200;  break;
        case SCORE_800:  m_scorePoint = 800;  break;
        case SCORE_1000: m_scorePoint = 1000; break;
        case SCORE_2000: m_scorePoint = 2000; break;
        case SCORE_5000: m_scorePoint = 5000; break;
        default : break;
    }
    m_scene->getGameSystem().m_currentScore += getScorePoint();
}

void Effect::step(float const &DELTA_TIME)
{
    if (static_cast<GameController*>(m_scene->SDMgetObject("GameController"))->canActivate())
    {
        m_time += is::getMSecond(DELTA_TIME);
        switch (m_type)
        {
            case COIN_FLY:
                m_frame += 0.15f * is::VALUE_CONVERSION * DELTA_TIME;
                setFrameLimit(8.f, 11.5f);
                if (m_time > 20) m_vsp = 4.f; else m_vsp = -4.f;
                if (m_y > m_yStart - 16.f && m_vsp > 0.f) m_frame = m_type = SCORE_200;
            break;

            case BLOCK_DEBRIS:
                m_frame = 12.f;
                m_x += m_hsp * is::VALUE_CONVERSION * DELTA_TIME;
                if (m_time > 12.f) is::increaseVar(DELTA_TIME, m_vsp, 0.5f, 9.f, 9.f); // go down
                else is::decreaseVar(DELTA_TIME, m_vsp, m_speed, -7.f, -7.f); // go up
                m_imageAngle += (-20.f * is::VALUE_CONVERSION) * DELTA_TIME;
                if (m_y > m_scene->getViewY() + (m_scene->getViewH() / 2.f)) setDestroyed(); // destroy object
            break;

            case ONE_UP:
            case SCORE_100:
            case SCORE_200:
            case SCORE_800:
            case SCORE_1000:
            case SCORE_2000:
            case SCORE_5000:
                m_vsp = -1.2f;
                if (m_y < m_yStart - 64.f) setDestroyed(); // destroy object
            break;
        }
        m_y += m_vsp * is::VALUE_CONVERSION * DELTA_TIME;
        is::setFrame(m_sprParent, m_frame, 4, 32);
    }
}

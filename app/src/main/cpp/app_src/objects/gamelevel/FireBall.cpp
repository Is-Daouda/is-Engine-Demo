#include "FireBall.h"

int FireBall::instanceNumber = 0;

FireBall::FireBall(sf::Texture &tex, float x, float y, float hSpeed, is::GameDisplay *scene):
    MainObject(x, y),
    m_scene(scene),
    m_createExplosion(false)
{
    m_time = 10.f;
    m_strName = "FireBall";
    m_hsp = hSpeed;
    instanceNumber++;
    m_xOffset = 16.f;
    m_yOffset = 16.f;
    setCircleMask(16.f);
    is::createSprite(tex, m_sprParent, sf::IntRect(0, 0, 32, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));
}

void FireBall::step(float const &DELTA_TIME)
{
    if (static_cast<GameController*>(m_scene->SDMgetObject("GameController"))->canActivate())
    {
        if (!m_createExplosion)
        {
            m_imageAngle += (-20.f * is::VALUE_CONVERSION) * DELTA_TIME;
            m_frame = 0.f;
            m_time += is::getMSecond(DELTA_TIME);
            m_x += m_hsp * is::VALUE_CONVERSION * DELTA_TIME;

            // bounce the object
            if (m_time > 10.f) m_vsp = 5.f;
            else m_vsp = -5.f;
            m_y += m_vsp * is::VALUE_CONVERSION * DELTA_TIME;

            if (!m_scene->inViewRec(this)) m_destroy = true; // We destroy the object
        }
        else
        {
            m_frame += 0.15f * is::VALUE_CONVERSION * DELTA_TIME;
            if (m_frame < 3.5f) m_frame = 3.5f;
            else
			{
				m_imageAlpha -= static_cast<int>((10.f * is::VALUE_CONVERSION) * DELTA_TIME);
				if (m_imageAlpha < 12) m_destroy = true; // We destroy the object
			}
        }

        // update sprite and collision mask
        is::setFrame(m_sprParent, m_frame, 4, 32);
        centerCollisionMask(m_x + m_xOffset, m_y + m_yOffset);
    }
}

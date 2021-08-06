#include "HUD.h"

HUD::HUD(is::GameDisplay *scene) :
    m_scene(scene),
    m_score(0),
    m_timeScoreCount(0.f),
    m_showTime(false)
{
    m_strName = "HUD";
    m_depth = -7;
    m_imageAlpha = 0;

    int const TXT_SIZE(20);
    is::createText(scene->getFontSystem(), m_txtScore,     " ", 0.f, 0.f, sf::Color(255, 255, 255, 255), TXT_SIZE);
    is::createText(scene->getFontSystem(), m_txtLevelTime, " ", 0.f, 0.f, sf::Color(255, 255, 255, 255), TXT_SIZE);
    is::createText(scene->getFontSystem(), m_txtBonus,     " ", 0.f, 0.f, sf::Color(255, 255, 255, 255), TXT_SIZE);
    is::createText(scene->getFontSystem(), m_txtWorld,
                   "WORLD  \n   " +
                    is::numToStr((m_scene->getGameSystem().m_currentLevel + 1) % 8) + " - " +
                    is::numToStr(m_scene->getGameSystem().m_currentLevel + 1),
                    0.f, 0.f, sf::Color(255, 255, 255, 255), TXT_SIZE);
    is::createSprite(m_scene->GRMgetTexture("bonus"), m_sprParent, sf::IntRect(0, 32, 32, 32), sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    is::setSFMLObjScale(m_sprParent, 0.5f);
}

void HUD::step(float const &DELTA_TIME)
{
    // Do this animation only when the player is playing
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
        if (m_score < m_scene->getGameSystem().m_currentScore && gameCtrl->getCountLevelTime())
        {
            m_timeScoreCount += ((is::VALUE_CONVERSION * 0.6f) * DELTA_TIME);
            if (m_timeScoreCount > 1.f)
            {
                m_score += 5;
                m_timeScoreCount = 0.f;
            }
        }
        else m_score = m_scene->getGameSystem().m_currentScore;
    }
    float const TXT_Y_POS(16.f);

    // We update the game information and position it on the view
    m_txtScore.setString("MARIO\n" + is::writeZero(m_score, 5));
    is::setSFMLObjX_Y(m_txtScore, m_scene->getViewX() - 270.f, (m_scene->getViewY() - m_scene->getViewH() / 2.f) + TXT_Y_POS);

    m_txtBonus.setString("x " + is::writeZero(m_scene->getGameSystem().m_currentBonus));
    is::setSFMLObjX_Y(m_sprParent, m_scene->getViewX() - 110.f, (m_scene->getViewY() - (m_scene->getViewH() / 2.f) + TXT_Y_POS) +
                  #if defined(IS_ENGINE_SDL_2)
                      33.f
                  #else
                      28.f
                  #endif
                      );
    is::setSFMLObjX_Y(m_txtBonus, m_scene->getViewX() - 90.f, (m_scene->getViewY() -  (m_scene->getViewH() / 2.f) + TXT_Y_POS) + 22.f);

    // coin animation
    m_frame += 0.15f * is::VALUE_CONVERSION * DELTA_TIME;
    setFrameLimit(6.f, 11.5f);
    is::setFrame(m_sprParent, m_frame, 6, 32);

    is::setSFMLObjX_Y(m_txtWorld, m_scene->getViewX() + 30.f, (m_scene->getViewY() - m_scene->getViewH() / 2.f) + TXT_Y_POS);

    m_txtLevelTime.setString("   TIME\n" +
                             ((m_showTime) ? static_cast<GameController*>(m_scene->SDMgetObject("GameController"))->m_gameTime.getTimeString() : ""));
    is::setSFMLObjX_Y(m_txtLevelTime, m_scene->getViewX() + 180.f, (m_scene->getViewY() - m_scene->getViewH() / 2.f) + TXT_Y_POS);
}

void HUD::draw(is::Render &surface)
{
    if (m_scene->getIsPlaying())
    {
        surface.draw(m_sprParent);
        surface.draw(m_txtScore);
        surface.draw(m_txtLevelTime);
        surface.draw(m_txtWorld);
        surface.draw(m_txtBonus);
        surface.draw(m_sprParent);
    }
}

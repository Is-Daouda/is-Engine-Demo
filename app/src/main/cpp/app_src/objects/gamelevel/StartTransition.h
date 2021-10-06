#ifndef STARTTRANSITION_H_INCLUDED
#define STARTTRANSITION_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "HUD.h"

////////////////////////////////////////////////////////////
/// TRANSITION SCREEN THAT DISPLAYS LEVEL INFORMATION
/// It locks all game play objects (player, bonus, ...) as
/// long as it is active
////////////////////////////////////////////////////////////
class StartTransition : public is::MainObject
{
public:
    StartTransition(is::GameDisplay *scene) :
        MainObject(),
        m_scene(scene)
    {
        m_strName = "StartTransition";
        m_depth = -6; // It appears behind the HUD to display level information

        is::createText(m_scene->getFontSystem(), m_txtWorld,
                       "WORLD " + is::numToStr((m_scene->getGameSystem().m_currentLevel + 1) % 8) +
                       " - " + is::numToStr(m_scene->getGameSystem().m_currentLevel + 1),
                       m_scene->getViewX(), m_scene->getViewY() - 96.f, sf::Color::White, true, 24);

        is::createSprite(m_scene->GRMgetTexture("player"), m_sprParent, sf::IntRect(0, 0, 24, 48),
                         sf::Vector2f(m_scene->getViewX() - 26.f, m_scene->getViewY() - 41.f), sf::Vector2f(12.f, 24.f));

        is::createText(m_scene->getFontSystem(), m_txtLives, " x  " + is::numToStr(m_scene->getGameSystem().m_currentLives),
                       m_scene->getViewX() + 15.f, is::getSFMLObjY(m_sprParent) + 12.f, sf::Color::White, true, 24);

        is::createRectangle(m_rec, sf::Vector2f(m_scene->getViewW(), m_scene->getViewH()), sf::Color::Black,
                            m_scene->getViewX(), m_scene->getViewY(), true);
    }

    void step(float const &DELTA_TIME)
    {
		// Show the transition and make the animations
        if (m_time < 2.5f * is::SECOND)
        {
            if (m_scene->getWindowIsActive() && m_scene->getIsPlaying())
            {
                m_time += is::getMSecond(DELTA_TIME);

                if (m_time > is::SECOND * 1.5f)
                {
                    is::moveSFMLObjX(m_sprParent, DELTA_TIME * 6.f * is::VALUE_CONVERSION);
                    m_frame += 0.22f * is::VALUE_CONVERSION * DELTA_TIME;
                    setFrameLimit(1.f, 3.5f);
                    is::setFrame(m_sprParent, m_frame, 9, 24, 48, 24, 48);
                }
            }
        }
        else
        {
            static_cast<HUD*>(m_scene->SDMgetObject("HUD"))->setShowTime(true);
            auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController"));
            gameCtrl->setLaunchTransition(false);
            gameCtrl->setCountLevelTime(true);
            m_scene->GSMplayMusic("world_1");
            m_destroy = true; // from here we destroy it so that it is no longer active
        }
    }

    void draw(is::Render &surface)
    {
        surface.draw(m_rec);
        surface.draw(m_txtLives);
        surface.draw(m_txtWorld);
        surface.draw(m_sprParent);
    }

private:
    is::GameDisplay *m_scene;
    sf::Text m_txtLives, m_txtWorld;
    sf::RectangleShape m_rec;
};

#endif // STARTTRANSITION_H_INCLUDED

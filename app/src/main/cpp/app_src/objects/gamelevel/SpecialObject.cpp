#include "SpecialObject.h"

SpecialObject::SpecialObject(SpecialObjectType type, float x, float y, is::GameDisplay *scene) :
    MainObject(x, y),
    Type(type),
    m_scene(scene)
{
    m_isActive = false;
    switch (m_type)
    {
        case VIEW_CONTROLLER: m_strName = "VIEW_CONTROLLER"; break;

        case ENTER_TUNNEL_A :
            m_strName = "ENTER_TUNNEL_A";
            m_x += 16.f; // put it in the middle of the tunnel
        break;

        case TUNNEL_EXIT_A  : m_strName = "TUNNEL_EXIT_A";   break;

        case ENTER_TUNNEL_B : m_strName = "ENTER_TUNNEL_B";  break;

        case TUNNEL_EXIT_B  :
            m_strName = "TUNNEL_EXIT_B";
            m_x += 16.f; // put it in the middle of the tunnel
        break;
    }
    setRectangleMask(28, 32);
}

void SpecialObject::step(float const &DELTA_TIME)
{
    if (auto gameCtrl = static_cast<GameController*>(m_scene->SDMgetObject("GameController")); gameCtrl->canActivate())
    {
        // We call the GameKeyData object from the SDM container
        auto gameKey = static_cast<is::GameKeyData*>(m_scene->SDMgetObject("GameKeyData"));
        auto player = static_cast<Player*>(m_scene->SDMgetObject("Player"));

        switch (m_type)
        {
            case VIEW_CONTROLLER :
                gameCtrl->m_moveView = (is::pointDistance(m_x, m_y, m_scene->getViewX(), m_scene->getViewY()) > 400.f);
            break;

            case ENTER_TUNNEL_A:
                if (!m_isActive)
                {
                    // Enter the first tunnel
                    if (placeMetting(0, 0, player) && gameKey->m_keyDownPressed)
                    {
                        gameKey->m_disableAllKey = true;
                        m_scene->SDMsetObjDepth(player, Depth::VERY_BIG_DEPTH); // make the player appear behind the tiles
                        m_scene->GSMplaySound("pipe");
                        m_isActive = true;
                    }
                }
                else
                {
                    // Go down animation
                    if (player->getYOffset() < 72.f) player->setYOffset(player->getYOffset() + (is::VALUE_CONVERSION * DELTA_TIME));
                    else
                    {
                        // First tunnel exit
                        auto obj = m_scene->SDMgetObject("TUNNEL_EXIT_A");
                        player->setPosition(obj->getX(), obj->getY());
                        player->setVspAcc(-1.f);
                        player->setYOffset(24.f);
                        m_scene->setViewVarXY(320, 720.f);
                        m_scene->setWindowBgColor(sf::Color::Black); // change scene background color
                        m_scene->SDMsetObjDepth(player, -1.f); // reset the initial depth
                        m_scene->GSMplayMusic("underground");
                        m_scene->GSMgetMusic("world_1")->stop();
                        gameKey->m_disableAllKey = false;
                        m_destroy = true; // destroy object
                    }
                }
            break;

            case TUNNEL_EXIT_A: /* Used just to position the object */ break;

            case ENTER_TUNNEL_B:
                if (!m_isActive)
                {
                    // Enter the second tunnel
                    if (placeMetting(0, 0, player) && player->getSpriteX() > m_x + 15.f && gameKey->m_keyRightPressed && player->getOnGround())
                    {
                        gameKey->m_disableAllKey = true;
                        m_scene->SDMsetObjDepth(player, Depth::VERY_BIG_DEPTH); // make the player appear behind the tiles
                        m_scene->GSMplaySound("pipe");
                        m_isActive = true;
                    }
                }
                else
                {
                    auto obj = m_scene->SDMgetObject("TUNNEL_EXIT_B");
                    if (player->getX() < obj->getX()) // Allows you to do the action once
                    {
                        // entrance animation
                        if (player->getXOffset() < 48.f) player->setXOffset(player->getXOffset() + (is::VALUE_CONVERSION * DELTA_TIME));
                        else
                        {
                            // Second tunnel exit
                            player->setPosition(obj->getX(), obj->getY() - 16.f);
                            player->setXYOffset(12.f, 72.f);
                            m_scene->setViewVarXY(obj->getX(), 240.f);
                            m_scene->setWindowBgColor(sf::Color(36, 146, 255)); // change scene background color
                            m_scene->GSMplaySound("pipe");
                            m_scene->GSMplayMusic("world_1");
                            m_scene->GSMgetMusic("underground")->stop();
                        }
                    }
                    else
                    {
                        // Go up animation
                        if (player->getYOffset() > 24.f) player->setYOffset(player->getYOffset() - (is::VALUE_CONVERSION * DELTA_TIME));
                        else
                        {
                            m_scene->SDMsetObjDepth(player, -1.f); // reset the initial depth
                            gameKey->m_disableAllKey = false;
                            m_destroy = true; // destroy object
                        }
                    }
                }
            break;

            case TUNNEL_EXIT_B:  /* Used just to position the object */ break;
        }
    }
}

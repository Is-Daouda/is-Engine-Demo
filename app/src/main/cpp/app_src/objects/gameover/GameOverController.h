#ifndef GAMEOVERCONTROLLER_H_INCLUDED
#define GAMEOVERCONTROLLER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"

////////////////////////////////////////////////////////////
/// The controller which manages the two (2) types of screens:
/// - the Game Over screen
/// - the Game End screen
////////////////////////////////////////////////////////////
class GameOverController : public is::MainObject
{
public:
    enum OptionIndex
    {
        CONTINUE,
        QUIT
    };
    GameOverController(is::GameDisplay *scene);
    void step(float const &DELTA_TIME);
    void draw(is::Render &surface);

private:
    is::GameDisplay *m_scene;
    is::DisplayOption m_sceneIndex;
    sf::Sprite m_sprPad1, m_sprPad2;
    sf::Text m_txtRestartGame, m_txtResumeGame, m_txtQuitGame, m_txtTitle, m_txtEndMsg, m_txtGetEditor;
};

#endif // GAMEOVERCONTROLLER_H_INCLUDED

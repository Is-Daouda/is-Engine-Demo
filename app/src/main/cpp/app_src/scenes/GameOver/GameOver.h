#ifndef GAMEOVER_H_INCLUDED
#define GAMEOVER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../objects/gameover/GameOverController.h"

////////////////////////////////////////////////////////////
/// This scene  supports two (2) types of screens:
/// - the Game Over screen (when the player loses the game)
/// - the Game End screen (when the player completes all the
/// levels)
////////////////////////////////////////////////////////////
class GameOver : public is::GameDisplay
{
public:
    GameOver(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, is::GameSystemExtended& gameSystemExtended);
    void loadResources();

    //////////////////////////////////////////////////////
    // void step();
    //
    // This method is no longer used because the SDM is
    // responsible for update the events and objects of the scene
    // for us.
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    // void draw();
    //
    // This method is no longer used because the SDM is
    // responsible for displaying the objects of the scene
    // for us.
    //////////////////////////////////////////////////////

private:
    sf::Texture m_texPad;
    sf::Font m_fontTitle;
};

#endif // GAMEOVER_H_INCLUDED

#ifndef FINISHFLAG_H_INCLUDED
#define FINISHFLAG_H_INCLUDED

#include "../../../isEngine/system/entity/MainObject.h"
#include "../../../isEngine/system/entity/parents/Health.h"
#include "../../../isEngine/system/entity/parents/Step.h"
#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../language/GameLanguage.h"
#include "../widgets/GameDialog.h"
#include "Player.h"
#include "Effect.h"

////////////////////////////////////////////////////////////
/// Represents the end of the level. If the player touches
/// this object he wins the game
////////////////////////////////////////////////////////////
class FinishObject : public is::MainObject, public is::Step
{
public:
    FinishObject(sf::Texture &tex, float x, float y, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

private:
    is::GameDisplay *m_scene;
    sf::Sprite m_sprCastle, m_sprCastleFlag;
    int addScore;
};

#endif // FINISHFLAG_H_INCLUDED

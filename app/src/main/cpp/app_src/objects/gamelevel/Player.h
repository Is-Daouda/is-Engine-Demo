#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../../../isEngine/system/display/GameDisplay.h"
#include "../../../isEngine/system/entity/MainObject.h"
#include "../../../isEngine/system/entity/parents/Visibilty.h"
#include "../../../isEngine/system/entity/parents/Health.h"
#include "../../../isEngine/system/function/GameKeyData.h"
#include "../widgets/GameDialog.h"
#include "Block.h"
#include "FireBall.h"
#include "GroundDetection.h"

class Player : public is::MainObject, public is::Health, public GroundDetection
{
public:
    Player(bool &timeUp, is::GameDisplay *scene);

    void step(float const &DELTA_TIME);
    void setIsKO(bool val);
    void setVspAcc(float val);
    void setOnGround(bool val);
    void setIsJumping(bool val);
    void setCanMove(bool val);
    void makeJump(float vspLim, bool &_keyState);
    void playerHurt(bool canContinue = true);
    void setTimeFreezPlayer(int val) {m_timeFreezPlayer = val;}
    void setGoDown(bool val)         {m_goDown = val;}
    void setStopAnimation(bool val)  {m_stopAnimation = val;}
    void setIsDucking(bool val)      {m_isDuking = val;}
    void haveFireBall()
    {
        m_timeStarInvicibility += is::SECOND / 2;
        m_timeFreezPlayer = is::SECOND / 2;
        m_haveFireBall = true;
        m_health = 2;
    }
    void haveStarPower() {m_timeStarInvicibility = 18 * is::SECOND;}
    void draw(is::Render &surface);

    bool placeMetting(int x, int y, Block *other);
    bool getIsKO()             const;
    bool getIsJumping()        const;
    bool getIsHurt()           const;
    bool playerIsVulnerable()  const;
    bool getHaveFireBall()     const {return m_haveFireBall;}
    bool getHaveStarPower()    const {return m_timeStarInvicibility > 0;}
    bool getIsDucking()        const {return m_isDuking;}
    float getVspAcc()          const;
    float getMaxHsp()          const;
    float getHspAcc()          const;

private:
    is::GameDisplay *m_scene;
    bool &m_timeUp;
    float m_vAcc;
    float const HSP_ACC, HSP_MAX;
    float m_hspAcc, m_hspLim;
    float const VSP_LIM, VSP_ACC, VSP_MAX, VSP_FALL_ACC;
    float m_starFrame, m_starFrameIndex;
    int   m_timePlayerInvicibility;
    int   m_timeStarInvicibility;
    int   m_timeFreezPlayer;
    bool  m_canMove;
    bool  m_goDown;
    bool  m_stopAnimation;
    bool  m_moveRight;
    bool  m_isJumping;
    bool  m_playerIsKO;
    bool  m_isBraking;
    bool  m_isDuking;
    bool  m_haveFireBall;
};

#endif // PLAYER_H_INCLUDED

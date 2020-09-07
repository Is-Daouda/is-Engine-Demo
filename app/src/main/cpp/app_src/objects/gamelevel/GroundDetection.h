#ifndef GROUNDDETECTION_H_INCLUDED
#define GROUNDDETECTION_H_INCLUDED

class GroundDetection
{
public:
    GroundDetection():
        m_onGround(false) {}

    virtual void setOnGround(bool val)
    {
        m_onGround = val;
    }

    virtual bool getOnGround() const
    {
        return m_onGround;
    }

protected:
    bool m_onGround;
};

#endif // GROUNDDETECTION_H_INCLUDED

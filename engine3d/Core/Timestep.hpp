#pragma once

namespace engine3d{

    class Timerstep{
    public:
        Timerstep(float time = 0.f) : m_Time(time) {}

        float Seconds() const { return m_Time; }

        float Milliseconds() const { return m_Time * 1000.0f; }

        operator float() const { return m_Time; }
    private:
        float m_Time;
    };
};
#pragma once

namespace engine3d{

    // Timestep
    // - Helper class
    class Timestep{
    public:
        Timestep(float time = 0.f) : _time(time) {}

        float getSeconds() const { return _time; }

        float getMilliseconds() const { return _time * 1000.0f; }

        // This just returns a _time that is converted into a float
        // Basically allowing to use this as a float
        operator float() const { return _time; }
		
		// friend std::ostream& operator<<(std::ostream& outs, const Timestep& step){
		// 	return outs << step._time;
		// }

    private:
        float _time;
    };
};
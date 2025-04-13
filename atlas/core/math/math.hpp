#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>

namespace atlas {

    class interpolation {
    public:
        template<typename T>
        static T linear_interpolate(T start,
                                   T end,
                                   const std::function<float(float)>& function,
                                   float t) {
            float l_adjusted_time = 0.0f;
            if (!function) {
                l_adjusted_time = t;
            }
            else {
                const float f = function(t);
                l_adjusted_time = f;
            }
            if (l_adjusted_time < 0) {
                l_adjusted_time = 0.0f;
            }
            if (l_adjusted_time > 1.0f) {
                l_adjusted_time = 1.0f;
            }

            float time_dif = 1.0f - l_adjusted_time;

            return start * time_dif + end * l_adjusted_time;
        }

    private:
        interpolation() = default;
    };
}; // namespace atlas
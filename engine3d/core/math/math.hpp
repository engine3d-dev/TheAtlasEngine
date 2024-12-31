#include <concepts>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <type_traits>

namespace engine3d{

    class Interpolation{
    public:
        template<typename T>
        static T LinearInterpolate(T start, T end, std::function<float(float)> function, float t){
            float l_AdjustedTime = 0.0f;
            if(!function){
                l_AdjustedTime = t;
            }
            else {
                const float f = function(t);
                l_AdjustedTime = f;
            }
            

            if(l_AdjustedTime < 0){
                l_AdjustedTime = 0.0f;
            }
            if(l_AdjustedTime > 1.0f){
                l_AdjustedTime = 1.0f;
            }

            float timeDif = 1.0f - l_AdjustedTime;

            return start * timeDif + end * l_AdjustedTime;
        }

        template<typename T>
        constexpr bool IsMathTypeValid(){
            if constexpr (std::same_as<T, glm::vec3>){
                return true;
            }

        }

    private:
        Interpolation() = default;
    };
};
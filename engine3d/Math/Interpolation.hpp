#include <concepts>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <type_traits>

namespace engine3d
{

    //! @note think about this one for a little A function that takes a function
    // for time and interpolates based of that
    // template <typename Func>
    // concept InterpolateFunction = requires(Func function) 
    // {
    //     { f(std::declval<decltype(function)>()) } -> std::convertible_to<float>;
    // };

    class Interpolation
    {
        public:
            template<typename T>
            static T LinearInterpolate(T start, T end, std::function<float(float)> function, float t)
            {
                float l_AdjustedTime = 0.0f;
                if(!function)
                {
                    l_AdjustedTime = t;
                }
                else 
                {
                    const float f = function(t);
                    l_AdjustedTime = f;
                }
                

                if(l_AdjustedTime < 0)
                {
                    l_AdjustedTime = 0.0f;
                }
                if(l_AdjustedTime > 1.0f)
                {
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

    // Add a slerp version of this
};
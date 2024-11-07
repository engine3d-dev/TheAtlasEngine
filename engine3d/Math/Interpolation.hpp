#include <concepts>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>


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
            template<typename T, typename Func>
            static T& LinearInterpolate(T start, T end, Func function, float t)
            {
                float l_AdjustedTime = 0.0f;
                if(function == NULL)
                {
                    l_AdjustedTime = t;
                }
                else 
                {
                    l_AdjustedTime = function(t);
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
        private:
            Interpolation() = default;
    };

    // Add a slerp version of this
};
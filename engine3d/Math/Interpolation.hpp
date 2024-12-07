#pragma once
#include <concepts>
#include <glm/glm.hpp>
#include <functional>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <type_traits>
#include <glm/gtc/constants.hpp> // For glm::epsilon
#include <cmath>


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
            static T LinearInterpolate(T start, T end, std::function<float(float)> function, float dt)
            {
                float l_AdjustedTime = 0.0f;
                if(!function)
                {
                    l_AdjustedTime = dt;
                }
                else 
                {
                    const float f = function(dt);
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

            template <typename T>
            static T SphericalInterpolate(T start, T end, std::function<float(float)> function, float dt) 
            {
                float adjustedTime = 0.0f;

                if (!function) 
                {
                    adjustedTime = dt;
                } else {
                    const float f = function(dt);
                    adjustedTime = f;
                }

                if (adjustedTime < 0.0f) 
                {
                    adjustedTime = 0.0f;
                }
                if (adjustedTime > 1.0f) 
                {
                    adjustedTime = 1.0f;
                }

                start = glm::normalize(start);
                end = glm::normalize(end);

                float dot = glm::dot(start, end);

                dot = glm::clamp(dot, -1.0f, 1.0f);

                if (glm::abs(dot) > 1.0f - glm::epsilon<float>()) 
                {
                    return glm::mix(start, end, adjustedTime);
                }

                float theta = glm::acos(dot);

                float sinTheta = glm::sin(theta);
                float factorStart = glm::sin((1.0f - adjustedTime) * theta) / sinTheta;
                float factorEnd = glm::sin(adjustedTime * theta) / sinTheta;

                return factorStart * start + factorEnd * end;
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
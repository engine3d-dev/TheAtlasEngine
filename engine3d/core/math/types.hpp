#pragma once
#include <glm/glm.hpp>

namespace engine3d{

    template<typename T>
    concept IsVec2 = requires(T vec2){
        { vec2.x } -> std::convertible_to<float>;
        { vec2.y } -> std::convertible_to<float>;
    };

    template<typename T>
    concept IsVec3 = requires(T vec3){
        { vec3.x } -> std::convertible_to<float>;
        { vec3.y } -> std::convertible_to<float>;
        { vec3.z } -> std::convertible_to<float>;
    };

    template<typename T>
    concept IsVec4 = requires(T vec4){
        { vec4.x } -> std::convertible_to<float>;
        { vec4.y } -> std::convertible_to<float>;
        { vec4.z } -> std::convertible_to<float>;
        { vec4.w } -> std::convertible_to<float>;
    };

   /**
     * @note The reason why we do this is because if we have an arbitrary type
     *       of vec2, vec3, vec4. This allows us to still utilize glm::vec3 as our default vec3 types
     * @note The computation would still be of how you can do math can be implemented by the math library of your choice,
     *       the only thing you just to is make those changes at compile-time to glm::vec2, glm::vec3, or even glm::vec4.
     * @note Hence why you override these metadata classes for specifically vec2, 3, and 4
     * 
     * @note By doing this we can let the default we define be utilizing glm's mathematical data types.
     *       Then letting users define their own customizable types if they choose to utilize a different mathematical type
    */

    template<typename T>
    struct vector2;

    template<typename T>
    struct vector3{
        vector3() = default;
        
        vector3(const T& p_Other) : value(p_Other) {
        }

        operator glm::vec3() {
            return value;
        }

        glm::vec3 operator=(const T& p_Other) {
            return p_Other;
        }

        bool operator==(const glm::vec3& p_Other){
            return (value.x == p_Other.x and value.y == p_Other.y and value.z == p_Other.z);
        }

    private:
        glm::vec3 value;
    };

    template<typename T>
    struct vector4;


    template<typename T>
    struct math_generic;

    template<typename T>
    struct matrix2;

    template<typename T>
    struct matrix3;

    template<typename T>
    struct matrix4;

    //! @note By default we use glm
    template<size_t Size, typename T>
    using vec = math_generic<glm::vec<Size, T>>;
    using vec2 = vector2<glm::vec2>;
    using vec3 = vector3<glm::vec3>;
    using vec4 = vector4<glm::vec4>;

    template<size_t C, size_t R, typename T>
    using mat = math_generic<glm::mat<C, R, T>>;

    using mat2 = matrix2<glm::mat2>;
    using mat3 = matrix3<glm::mat3>;
    using mat4 = matrix4<glm::mat4>;
};
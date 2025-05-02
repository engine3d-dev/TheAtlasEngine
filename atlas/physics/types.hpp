#pragma once

#include <physics/jolt-cpp/jolt-imports.hpp>
#include <glm/glm.hpp>

namespace atlas::physics {

    template<typename T>
    struct vector3;

    template<>
    struct vector3<JPH::Vec3> {
        vector3() = default;

        vector3(const JPH::Vec3& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ() };
        }

        operator glm::vec3() const { return m_value; }

        glm::vec3 operator=(const JPH::Vec3& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ() };
            return m_value;
        }

        bool operator==(const glm::vec3& other) const { return m_value == other; }

    private:
        glm::vec3 m_value;
    };

    template<>
    struct vector3<JPH::Float3> {
        vector3() = default;

        vector3(const JPH::Float3& v) { m_value = { v.x, v.y, v.z }; }

        operator glm::vec3() const { return m_value; }

        glm::vec3 operator=(const JPH::Float3& v) {
            m_value = { v.x, v.y, v.z };
            return m_value;
        }

        bool operator==(const glm::vec3& other) const { return m_value == other; }

    private:
        glm::vec3 m_value;
    };

    template<>
    struct vector3<JPH::DVec3> {
        vector3() = default;

        vector3(const JPH::DVec3& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ() };
        }

        operator glm::dvec3() const { return m_value; }

        glm::dvec3 operator=(const JPH::DVec3& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ() };
            return m_value;
        }

        bool operator==(const glm::dvec3& other) const {
            return m_value == other;
        }

    private:
        glm::dvec3 m_value;
    };

    template<>
    struct vector3<JPH::Double3> {
        vector3() = default;

        vector3(const JPH::Double3& v) { m_value = { v.x, v.y, v.z }; }

        operator glm::dvec3() const { return m_value; }

        glm::dvec3 operator=(const JPH::Double3& v) {
            m_value = { v.x, v.y, v.z };
            return m_value;
        }

        bool operator==(const glm::dvec3& other) const {
            return m_value == other;
        }

    private:
        glm::dvec3 m_value;
    };

    // === VECTOR4 ADAPTER ===
    template<typename T>
    struct vector4;

    template<>
    struct vector4<JPH::Vec4> {
        vector4() = default;

        vector4(const JPH::Vec4& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ(), v.GetW() };
        }

        operator glm::vec4() const { return m_value; }

        glm::vec4 operator=(const JPH::Vec4& v) {
            m_value = { v.GetX(), v.GetY(), v.GetZ(), v.GetW() };
            return m_value;
        }

        bool operator==(const glm::vec4& other) const { return m_value == other; }

    private:
        glm::vec4 m_value;
    };

    template<>
    struct vector4<JPH::Float4> {
        vector4() = default;

        vector4(const JPH::Float4& v) { m_value = { v.x, v.y, v.z, v.w }; }

        operator glm::vec4() const { return m_value; }

        glm::vec4 operator=(const JPH::Float4& v) {
            m_value = { v.x, v.y, v.z, v.w };
            return m_value;
        }

        bool operator==(const glm::vec4& other) const { return m_value == other; }

    private:
        glm::vec4 m_value;
    };

    // === QUATERNION ADAPTER ===
    template<typename T>
    struct quaternion;

    // === MATRIX4 ADAPTER ===
    template<typename T>
    struct matrix4;

    template<>
    struct matrix4<JPH::Mat44> {
        matrix4() = default;

        matrix4(const JPH::Mat44& m) {
            for (int i = 0; i < 4; ++i) {
                const auto col = m.GetColumn4(i);
                m_value[i] =
                  glm::vec4(col.GetX(), col.GetY(), col.GetZ(), col.GetW());
            }
        }

        operator glm::mat4() const { return m_value; }

        glm::mat4 operator=(const JPH::Mat44& m) {
            for (int i = 0; i < 4; ++i) {
                const auto col = m.GetColumn4(i);
                m_value[i] =
                  glm::vec4(col.GetX(), col.GetY(), col.GetZ(), col.GetW());
            }
            return m_value;
        }

        bool operator==(const glm::mat4& other) const { return m_value == other; }

    private:
        glm::mat4 m_value;
    };
};

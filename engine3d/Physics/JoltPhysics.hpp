#pragma once
namespace VectorConversion
{
        template <typename Vector3>
        concept JPHVec3 = requires(Vector3 vector)
        {
            vector.GetX();
            vector.GetY();
            vector.GetZ();
        };
    template <typename Vector4>
        concept JPHVec4 = JPHVec3<Vector4> && requires(Vector4 vector)
        {
            vector.GetW();
        };

    template <typename T,JPHVec3 U>
    T To_Type(const U &type)
    {
        return T(type.GetX(), type.GetY(), type.GetZ());
    }

    template <typename T,JPHVec4 U>
    T To_Type(const U &type)
    {
        return T(type.GetX(), type.GetY(), type.GetZ(), type.GetW());
    }
}
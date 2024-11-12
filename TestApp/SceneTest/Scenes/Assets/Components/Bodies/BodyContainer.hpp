#include <Core/TimeManagement/GlobalUpdateManager.hpp>
// Jolt Includes
class BodyContainer
{
    public:
        BodyContainer();

        operator JPH::BodyID() { return m_BodyID; }
        JPH::BodyCreationSettings m_BodySettings;
        JPH::BodyID m_BodyID;
};